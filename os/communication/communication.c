#include "communication.h"

#include "../events/events.h"
#include "../platform/e-puck/adc.h"
#include "../interrupts.h"
#include "../memory.h"
#include "../platform/e-puck/motors.h"
#include "../platform/e-puck/leds.h"

#include "coder.h"
#include "channel.h"
#include "physical.h"

#define BODY_INDICATOR

typedef struct comm_device{
    uint8 address;
    uint8 msg_id;
}Sys_CommDevice;

static Sys_CommDevice sys_com_device;

Sys_RawMessageList *Sys_convert_WordToPackage(uint8 address, uint32 dvalue, uint8 type);
uint Sys_ListLength(Sys_RawMessageList *list);

uint sys_com_Init_state = 0;

void Sys_Init_Communication(){
    
    Sys_Init_PhysicalLayer();
    
    sys_com_device.address = SYS_LOCAL_ADDRESS;
    //Sys_SetInitStateEnvironm(sys_com_Init_state = lightsOffNoTurning);    
    
    Sys_Register_Event(SYS_EVENT_COM_RX_MSG);
    Sys_Register_Event(SYS_EVENT_COM_TX_MSG);
}

void Sys_Start_Communication(){
    Sys_Start_PhysicalLayer();
}

void Sys_Stop_Communication(){
    Sys_Stop_PhysicalLayer();
}

void Sys_Send_Data(uint8 address, void *data, uint length){
    uint32 dvalue = 0;
    
    while(length > 4){
        uint32* dpointer = (uint32 *)data;
        
        dvalue = *dpointer;
        data = &dpointer[1];//next element
        length -= 4;
        
        Sys_RawMessageList *element = Sys_convert_WordToPackage(address, dvalue, 1);
        element->next = 0;
        
        Sys_AddOutMessage(element);
    }
    
    if(length > 0){
        dvalue = 0;
        Sys_Memcpy(data,&dvalue,length);
        
        Sys_RawMessageList *element = Sys_convert_WordToPackage(address, dvalue, 0);
        element->next = 0;
        
        //(element->message, 10);
        
        Sys_AddOutMessage(element); 
    }
}

Sys_Message *getNewMessage(){
    static Sys_Message out;
    uint error_num = 0;
    bool error = false;
    uint buffer = 0;
    
    Sys_RawMessageList *raw_msg = Sys_GetNextInMessage();
    if(raw_msg == 0){
        return 0;
    }
    
    raw_msg->next       = 0;
    raw_msg->position   = 0;
    
    Sys_Writeto_UART1(raw_msg->message, 10);
    
    
    Sys_Memset(&out, sizeof(Sys_Message), 0);
      
    uint data = decodeBCH(raw_msg->message[0], &error);//first 11 bits
    if(error){
        error_num++;
    }
    
    
    if((data & 0x400) == 0){// != 0 //is the start bit '1' there -> if not increase error
        error_num++;
    }
    
    // ID 7-bit [10-4] != 0
    buffer = (data >> 3) & 0x7F;
    if(buffer == 0){// != 0
        error_num++;
    }
    
    out.id = (uint8) buffer;
    
    // Source address 0:[3-1]+1:[11-9] != 0
    buffer = (data & 0x07) << 3; //0:[3-1]
    data = decodeBCH(raw_msg->message[1], &error);
    if(error){
        error_num++;
    }
    
    buffer |= data >> 8;//1:[11-9]
    if(buffer == 0){// != 0
        error_num++;
    }
    
    out.address_source = (uint8) buffer;
    
    // destination address 1:[8-3]
    buffer = (0xFC & data) >> 2;
    out.address_destination = (uint8) buffer;
    
    // Type 1:[2-1]+2:[11]
    buffer = (0x03 & data) << 1;//1:[2-1]
    
    data = decodeBCH(raw_msg->message[2], &error);
    if(error){
        error_num++;
    }
    
    buffer = data >> 10;
    out.type |= buffer & 0x07;//2:[11]
    
    // Data 2:[10-1]+3:[11-1]+4:[11-1]
    uint32 longData = ( (uint32) (data & 0x03FF) ) << 22;//2:[10-1]
    
    data = decodeBCH(raw_msg->message[3], &error);
    if(error){
        error_num++;
    }
    
    longData |= ((uint32) data) << 11;//3:[11-1]
    
    data = decodeBCH(raw_msg->message[4], &error);
    if(error){
        error_num++;
    }
       
    longData |= (uint32) data;  //4:[11-1]
    out.data = longData;
    out.error = error_num;
        
    Sys_Free(raw_msg);
    return &out;
    
    return 0;
}

Sys_RawMessageList *Sys_convert_WordToPackage(uint8 address, uint32 dvalue, uint8 type){
    Sys_RawMessageList *out = 0;
    
    out = (Sys_RawMessageList *) Sys_Malloc(sizeof(Sys_RawMessageList));
    out->next = 0;
    
    uint16 buffer = 0x0400;
    buffer |= ((uint16) (sys_com_device.msg_id+1)) << 3;
    sys_com_device.msg_id = (sys_com_device.msg_id+1) % 0x7E;
    
    buffer |= ((uint16) sys_com_device.address) >> 3;
    out->message[0] = encodeBCH(buffer);
    
    buffer  = ((uint16) (sys_com_device.address & 0x07)) << 8;
    buffer |= ((uint16) address) << 2;
    buffer |= ((uint16) (type & 0x07)) >> 1;
    out->message[1] = encodeBCH(buffer);
    
    buffer  = ((uint16) (type & 0x01)) << 10;
    buffer |= (uint16) (dvalue >> 22);
    out->message[2] = encodeBCH(buffer);
    
    buffer = (uint16) ((dvalue >> 11) & 0x07FF);
    out->message[3] = encodeBCH(buffer);
    
    buffer = (uint16) (dvalue & 0x07FF);        
    out->message[4] = encodeBCH(buffer);
    
    return out;
}