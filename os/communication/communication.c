#include "communication.h"
#include "physical.h"

#include "../events/events.h"
#include "../platform/e-puck/adc.h"
#include "../interrupts.h"
#include "../memory.h"
#include "../platform/e-puck/motors.h"
#include "../platform/e-puck/leds.h"

#include "coder.h"
#include "channel.h"

#define BODY_INDICATOR
#define FLOODING

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
    sys_com_device.msg_id = 0;
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



#ifdef FLOODING
    static Sys_RawMessageList old[5];
    static uint oldIndex = 0;
#endif

void Sys_Send_Data(uint8 address, void *data, uint length){
    uint32 dvalue = 0;
    
    while(length > 4){
        uint32* dpointer = (uint32 *)data;
        
        dvalue = *dpointer;
        data = &dpointer[1];//next element
        length -= 4;
        
        Sys_RawMessageList *element = Sys_convert_WordToPackage(address, dvalue, 1);
        element->next = 0;
        
#ifdef DEBUG_COM
        Sys_Writeto_UART1(element->message, 10);
#endif

#ifdef FLOODING
        Sys_Start_AtomicSection(); 
            Sys_Memcpy(element, &old[oldIndex], sizeof(Sys_RawMessageList));
            oldIndex = (oldIndex + 1) % 5;
        Sys_End_AtomicSection();
#endif
        
        Sys_AddOutMessage(element);
    }
    
    if(length > 0){
        dvalue = 0;
        Sys_Memcpy(data,&dvalue,length);
              
        Sys_RawMessageList *element = Sys_convert_WordToPackage(address, dvalue, 0);
        element->next = 0;
        
#ifdef DEBUG_COM
        Sys_Writeto_UART1(element->message, 10);
#endif
        
#ifdef FLOODING
        Sys_Start_AtomicSection(); 
            Sys_Memcpy(element, &old[oldIndex], sizeof(Sys_RawMessageList));
            oldIndex = (oldIndex + 1) % 5;
        Sys_End_AtomicSection();
#endif
        Sys_AddOutMessage(element); 
    }
}
    
Sys_Message *getNewMessage(){
    static Sys_Message out;
    
    uint16 error_num = 0;
    bool error = false;
    uint16 buffer = 0;
    
    Sys_RawMessageList *raw_msg = Sys_GetNextInMessage();
    if(raw_msg == 0){
        return 0;
    }
    
    raw_msg->next       = 0;
    raw_msg->position   = 0;
    
#ifdef FLOODING
    uint oi = 0;
    bool found = FALSE;
    for(oi = 0; oi < 5; oi++){
        char *oldMsg = (char *) &old[oi];
        char *newMsg = (char *) raw_msg;
        
        uint c = 0;
        bool found_c = TRUE;
        for(c = 0; c < sizeof(Sys_RawMessageList); c++){
            if(oldMsg[c] != newMsg[c]){
                found_c = FALSE;
                break;
            }
        }
        
        if(found_c){
            found = TRUE;
            break;
        }
    }
    
    if(found){//found a repeated msg
        Sys_Free(raw_msg);
        return 0;
    }

    Sys_Start_AtomicSection(); 
        Sys_Memcpy(raw_msg, &old[oldIndex], sizeof(Sys_RawMessageList));
        oldIndex = (oldIndex + 1) % 5;
    Sys_End_AtomicSection(); 

#endif
                
    Sys_Memset(&out, sizeof(Sys_Message), 0);
   
    uint16 data = decodeBCH(raw_msg->message[0], &error);//first 11 bits
    if(error){
        error_num++;
    }
    
    // ID 7-bit [0-7] != 0
    buffer = data & 0x007F;
    if(buffer == 0){// != 0
        error_num++;
    }
   
    out.id = (uint8) buffer;
    
    // Source address 0:[8-11]+1:[0-1] != 0
    buffer = (data & 0x0780) >> 7; //0:[3-1]
    data = decodeBCH(raw_msg->message[1], &error);
    if(error){
        error_num++;
    }
    
    buffer |= (data & 0x0003) << 4;//1:[11-9]
    if(buffer == 0){// != 0
        
        error_num++;
    }
    
    out.address_source = (uint8) buffer;
    if(buffer == 0){
        error_num++;
    }
    
    // destination address 1:[2-8]
    buffer = (0x00FC & data) >> 2;
    out.address_destination = (uint8) buffer;
    
    // Type 1:[9-11]+2:[1]
    buffer = (0x0700 & data) >> 8;//1:[2-1]
    
    data = decodeBCH(raw_msg->message[2], &error);
    if(error){
        error_num++;
    }
    
    buffer |= (data & 0x0001);
    out.type |= buffer;//2:[1]
    
    // Data 2:[10-1]+3:[11-1]+4:[11-1]
    uint32 longData = ( (uint32) (data & 0x07FE) ) << 21;//2:[10-1]
    
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
   
    
#ifdef DEBUG_COM
    //if(error_num == 0){
        Sys_Writeto_UART1(raw_msg->message, 10);
    //}
    
#endif
    
#ifdef FLOODING
    if(error_num == 0){
        Sys_AddOutMessage(raw_msg);
    }else{
        Sys_Free(raw_msg);     
    }
#else
    Sys_Free(raw_msg);
#endif    
    
    return &out;
}

static uint pkg_id = 0;

Sys_RawMessageList *Sys_convert_WordToPackage(uint8 address, uint32 dvalue, uint8 type){
    
    Sys_RawMessageList *out = 0;
    
    out = (Sys_RawMessageList *) Sys_Malloc(sizeof(Sys_RawMessageList));
    out->next = 0;
    
    //uint16 buffer = 0x0001; Synch bit not in data
    pkg_id = (pkg_id+1) & 0x7F;
    uint16 buffer = ((uint16) (SYS_LOCAL_ADDRESS & 0x0F)) << 7;
    buffer |= (uint16) (((uint) pkg_id) & 0x007F);
    out->message[0] = encodeBCH(buffer);
    
    buffer  = ((uint16) (SYS_LOCAL_ADDRESS & 0x30)) >> 4;
    buffer |= ((uint16) address & 0x3F) << 2;
    buffer |= ((uint16) (type & 0x07)) << 8;
    out->message[1] = encodeBCH(buffer);
    
    buffer  = ((uint16) (type & 0x08)) >> 3;
    buffer |= ((uint16) (dvalue >> 22)) << 1;
    out->message[2] = encodeBCH(buffer);
    
    buffer = (uint16) ((dvalue >> 11) & 0x07FF);
    out->message[3] = encodeBCH(buffer);
    
    buffer = (uint16) (dvalue & 0x07FF);        
    out->message[4] = encodeBCH(buffer);
    
    return out;
}
