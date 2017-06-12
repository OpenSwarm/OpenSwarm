#include "communication.h"

#include "../events/events.h"
#include "../platform/e-puck/adc.h"
#include "../interrupts.h"
#include "../memory.h"
#include "../platform/e-puck/motors.h"
#include "../platform/e-puck/leds.h"

#include "coder.h"
#include "communicationInit.h"

typedef struct comm_device{
    uint8 address;
    uint8 msg_id;
}Sys_CommDevice;

static Sys_CommDevice sys_com_device;

uint sensorReadings[8] ={0};
uint minReadings = 0;


typedef enum rx_state{
    waiting,
    receiving
} Sys_RX_state;

/*
typedef enum init_comm_state_s{
            lightsOffNoTurning = 0,
            lightsOffTurning = 1,
            lightsOnTurning = 2,
            lightsOnNoTurning = 3,
            initialised = 4
                    
} Sys_Comm_InitState;

Sys_Comm_InitState sys_init_state = lightsOffNoTurning;
*/
Sys_RX_state rxState = waiting;

Sys_RawMessageList  *sys_InMsg_List = 0;
Sys_RawMessageList **sys_InMsg_ListEnd = &sys_InMsg_List;

Sys_RawMessageList *sys_OutMsg_List = 0;
Sys_RawMessageList **sys_OutMsg_List_End = &sys_OutMsg_List;

#define THRESHOLD 5

void ComSensor0(uint);
void ComSensor1(uint);
void ComSensor2(uint);
void ComSensor3(uint);
void ComSensor4(uint);
void ComSensor5(uint);
void ComSensor6(uint);
void ComSensor7(uint);
void ComReceived(void);

//bool doesLEDsEffect(uint8 pos);
//bool doesMotorsEffect();

Sys_RawMessageList *Sys_convert_WordToPackage(uint8 address, uint32 dvalue, uint8 type);

uint Sys_ListLength(Sys_RawMessageList *list);
//void Sys_SetInitStateEnvironm(Sys_Comm_InitState state);

uint sys_com_Init_state = 0;

void Sys_Init_Communication(){
    Sys_Init_ADC();//make sure ADCs are initialised
    
    sys_com_device.address = SYS_LOCAL_ADDRESS;
    //Sys_SetInitStateEnvironm(sys_com_Init_state = lightsOffNoTurning);    
    
    Sys_Register_Event(SYS_EVENT_COM_RX_MSG);
    Sys_Register_Event(SYS_EVENT_COM_TX_MSG);
     
    
    rxState=waiting;
}

void Sys_Start_Communication(){
    
    Sys_Start_ComInit();
    /*
    Sys_Start_AtomicSection();
    Sys_Subscribe_ADCChannelProcessor(Prx0, InitSensor0);
    Sys_Subscribe_ADCChannelProcessor(Prx1, InitSensor1);
    Sys_Subscribe_ADCChannelProcessor(Prx2, InitSensor2);
    Sys_Subscribe_ADCChannelProcessor(Prx3, InitSensor3);
    Sys_Subscribe_ADCChannelProcessor(Prx4, InitSensor4);
    Sys_Subscribe_ADCChannelProcessor(Prx5, InitSensor5);
    Sys_Subscribe_ADCChannelProcessor(Prx6, InitSensor6);
    Sys_Subscribe_ADCChannelProcessor(Prx7, InitSensor7); 
    Sys_Subscribe_ADCFinish(ProxDone,InitAllSensors); 
    Sys_End_AtomicSection(); 
        
    while(sys_init_state != initialised){
        ;
    }
    */    
    Sys_Start_AtomicSection();
    Sys_Subscribe_ADCChannelProcessor(Prx0, ComSensor0);
    Sys_Subscribe_ADCChannelProcessor(Prx1, ComSensor1);
    Sys_Subscribe_ADCChannelProcessor(Prx2, ComSensor2);
    Sys_Subscribe_ADCChannelProcessor(Prx3, ComSensor3);
    Sys_Subscribe_ADCChannelProcessor(Prx4, ComSensor4);
    Sys_Subscribe_ADCChannelProcessor(Prx5, ComSensor5);
    Sys_Subscribe_ADCChannelProcessor(Prx6, ComSensor6);
    Sys_Subscribe_ADCChannelProcessor(Prx7, ComSensor7); 
    Sys_Subscribe_ADCFinish(ProxDone,ComReceived);   
    Sys_End_AtomicSection();  
}

void Sys_Stop_Communication(){
    Sys_Subscribe_ADCFinish(ProxDone, 0);    
}

void ComSensor0(uint s){
    sensorReadings[0] = s;
    if( s < Sys_ComThreshold()){
        LED0 = 1;
    }else{
        LED0 = 0;
    }
}

void ComSensor1(uint s){
    sensorReadings[1] = s;
    if( s < Sys_ComThreshold()){
        LED1 = 1;
    }else{
        LED1 = 0;
    }
}

void ComSensor2(uint s){
    sensorReadings[2] = s;
    if( s < Sys_ComThreshold()){
        LED2 = 1;
    }else{
        LED2 = 0;
    }
}

void ComSensor3(uint s){
    sensorReadings[3] = s;
    if( s < Sys_ComThreshold()){
        LED3 = 1;
    }else{
        LED3 = 0;
    }
}

void ComSensor4(uint s){
    sensorReadings[4] = s;
    if( s < Sys_ComThreshold()){
        LED5 = 1;
    }else{
        LED5 = 0;
    }
}

void ComSensor5(uint s){
    sensorReadings[5] = s;
    if( s < Sys_ComThreshold()){
        LED6 = 1;
    }else{
        LED6 = 0;
    }
}

void ComSensor6(uint s){
    sensorReadings[6] = s;
    if( s < Sys_ComThreshold()){
        LED7 = 1;
    }else{
        LED7 = 0;
    }
}

void ComSensor7(uint s){
    sensorReadings[7] = s;
    if( s < Sys_ComThreshold()){
        LED0 = 1;
    }else{
        LED0 = 0;
    }
}

#define MAX_SIGNAL_DELAY 1
void ComReceived2(void){
    static uint signalPropergationDelay = 0;
    if(signalPropergationDelay != MAX_SIGNAL_DELAY){
        signalPropergationDelay++;
        return;
    }
    signalPropergationDelay = 0;
    
    Sys_Start_AtomicSection();
    if(sys_OutMsg_List != 0){
        uint seg = sys_OutMsg_List->position / 15;
        uint bit_pos = sys_OutMsg_List->position % 15;
        if(sys_OutMsg_List->message[seg] & (1 << bit_pos)){ // == 1
            setIRs();
            LED4 = 1;
        }else{
            clearIRs();
        }
        sys_OutMsg_List->position++;
    }else{
        clearIRs();
    }
    
    if(sys_OutMsg_List->position >= 75){
         Sys_RawMessageList  *msg_list = sys_OutMsg_List;
        sys_OutMsg_List = sys_OutMsg_List->next;
        if(sys_OutMsg_List == 0){
            sys_OutMsg_List_End = &sys_OutMsg_List;
        }
        Sys_Free(msg_list);
    }
    Sys_End_AtomicSection();
    
}
void ComReceived(void){
    static Sys_RawMessageList  *currentMsg=0;
    static uint signalPropergationDelay = 0;
    uint min = 0xFFFF;
    uint i_min = 0;
    int i = 0;
    
    if(signalPropergationDelay != MAX_SIGNAL_DELAY){
        signalPropergationDelay++;
        return;
    }
    
    signalPropergationDelay = 0;
    
    Sys_Start_AtomicSection();
    if(sys_OutMsg_List != 0){
        uint seg = sys_OutMsg_List->position / 15;
        uint bit_pos = sys_OutMsg_List->position % 15;
        if(sys_OutMsg_List->message[seg] & (1 << bit_pos)){ // == 1
            setIRs();
            LED4 = 1;
        }else{
            clearIRs();
            LED4 = 0;
        }
        sys_OutMsg_List->position++;
    }else{
        clearIRs();
            LED4 = 0;
    }
    
    if(sys_OutMsg_List->position >= 75){
         Sys_RawMessageList  *msg_list = sys_OutMsg_List;
        sys_OutMsg_List = sys_OutMsg_List->next;
        if(sys_OutMsg_List == 0){
            sys_OutMsg_List_End = &sys_OutMsg_List;
        }
        Sys_Free(msg_list);
    }
    Sys_End_AtomicSection();
    
    Sys_Start_AtomicSection();
    for(i = 0; i < 8; i++){
        if(sensorReadings[i] < min){
            min = sensorReadings[i];
            i_min = i;
        }
    }
    
    uint8 new_bit = (min <= Sys_ComThreshold());
    
    if(new_bit){
        LED5 = 1;
    }
    
    Sys_End_AtomicSection();
    
    if(currentMsg == 0){
        if(new_bit == 0){
            return;
        }
        
        currentMsg = Sys_Malloc(sizeof(Sys_RawMessageList));
        Sys_Memset(currentMsg,sizeof(Sys_RawMessageList),0);        
    }
        
    uint pos = currentMsg->position / 15;
    currentMsg->message[pos] |= new_bit << (15-(currentMsg->position % 15));
    currentMsg->position++;
    
    if(currentMsg->position >= 75){
    Sys_Start_AtomicSection();
        *sys_InMsg_ListEnd = currentMsg;
        sys_InMsg_ListEnd = &(currentMsg->next);
        currentMsg = 0;
    Sys_End_AtomicSection();
    }
}

void Sys_Send_Data(uint8 address, void *data, uint length){
    Sys_RawMessageList *list = 0;
    Sys_RawMessageList **list_end = &list;
    uint32 dvalue = 0;
    
    //printf("Data - [%p,%p,%p] -\r\n", list, *list_end, list_end );
    
    while(length > 4){
        uint32* dpointer = (uint32 *)data;
        
        dvalue = *dpointer;
        data = &dpointer[1];//next element
        length -= 4;
        
        Sys_RawMessageList *element = Sys_convert_WordToPackage(address, dvalue, 1);
        element->next = 0;
        *list_end = element;
        list_end = &(element->next);        
    }
    
    if(length > 0){
        dvalue = 0;
        Sys_Memcpy(data,&dvalue,length);
        
        Sys_RawMessageList *element = Sys_convert_WordToPackage(address, dvalue, 0);
        element->next = 0;
        *list_end = element;
        list_end = &(element->next);  
    }
    
    //printf("Data - [%p,%p,%p] -\r\n", list, *list_end, list_end );
    //queue all packages to be sent
    
    Sys_Start_AtomicSection();
        *sys_OutMsg_List_End = list;
        sys_OutMsg_List_End = list_end;
    Sys_End_AtomicSection();
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

uint Sys_ListLength(Sys_RawMessageList *list){
    uint nelements = 0;
    
    while(list != 0){
        nelements++;
        list = list->next;
    }
    
    return nelements;
}

Sys_Message *getNewMessage(){
    static Sys_Message out;
    int error_num = 0;
    uint buffer = 0;
    bool error;
    
    Sys_Start_AtomicSection();
        if(sys_InMsg_List == 0){
            Sys_End_AtomicSection();
            return 0;
        }

    //    printf("--[%p,%p,%p,%p,%x]--", sys_InMsg_List, sys_InMsg_List->next,sys_InMsg_List_End, *sys_InMsg_List_End,  *sys_InMsg_List_End);
        Sys_RawMessageList  *msg_list = sys_InMsg_List;
        sys_InMsg_List = sys_InMsg_List->next;
        if(sys_InMsg_List == 0){
            sys_InMsg_ListEnd = &sys_InMsg_List;
        }

        msg_list->next = 0;
    
    Sys_End_AtomicSection();
    
    uint data = decodeBCH(msg_list->message[0], &error);//first 11 bits
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
    data = decodeBCH(msg_list->message[1], &error);
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
    
    data = decodeBCH(msg_list->message[2], &error);
    if(error){
        error_num++;
    }
    
    buffer = data >> 10;
    out.type |= buffer & 0x07;//2:[11]
    
    // Data 2:[10-1]+3:[11-1]+4:[11-1]
    uint32 longData = ( (uint32) (data & 0x03FF) ) << 22;//2:[10-1]
    
    data = decodeBCH(msg_list->message[3], &error);
    if(error){
        error_num++;
    }
    
    longData |= ((uint32) data) << 11;//3:[11-1]
    
    data = decodeBCH(msg_list->message[4], &error);
    if(error){
        error_num++;
    }
       
    longData |= (uint32) data;  //4:[11-1]
    out.data = longData;
    out.error = error_num;
        
    Sys_Free(msg_list);
    return &out;
}


/*
bool doesLEDsEffect(uint8 pos){
    
    if(BODY_LED == 1){
        return true;
    }
    
    switch(pos){
        case 0:
           if(LED0 == 1 || FRONT_LED == 1 ){
               return true;
           } 
           break;
        case 1:
           if(LED1 == 1 || FRONT_LED == 1 ){
               return true;
           } 
           break;
        case 2:
           if(LED2 == 1 ){
               return true;
           } 
           break;
        case 3:
           if(LED3 == 1 ||  LED4 == 1){
               return true;
           } 
           break;
        case 4:
           if(LED5 == 1 ||  LED4 == 1){
               return true;
           } 
           break;
        case 5:
           if(LED6 == 1 ){
               return true;
           } 
           break;
        case 6:
           if(LED7 == 1 ){
               return true;
           } 
           break;
        case 7:
           if(LED0 == 1 || FRONT_LED == 1 ){
               return true;
           } 
           break;
        default:
            break;
    }
    
    return false;
}

void stopMotors(){
    Sys_Set_LeftWheelSpeed(0);
    Sys_Set_RightWheelSpeed(0);
}
void runMotorsL(){
    Sys_Set_LeftWheelSpeed(128);
    Sys_Set_RightWheelSpeed(-128);
}
void runMotorsR(){
    Sys_Set_LeftWheelSpeed(-128);
    Sys_Set_RightWheelSpeed(128);
}


void Sys_SetInitStateEnvironm(Sys_Comm_InitState state){
    clearIRs();
    switch(state){
        case lightsOffNoTurning:
            clearLEDs();
            stopMotors();
            break;
        case lightsOffTurning:
            clearLEDs();
            runMotorsL();
            break;
        case lightsOnTurning:
            setLEDs();
            runMotorsR();
            break;
        case lightsOnNoTurning:
            setLEDs();
            stopMotors();
            break;
        case initialised:
        default:
            break;
    }
}

bool doesMotorsEffect(){
    return (Sys_Get_LeftWheelSpeed() != 0 || Sys_Get_RightWheelSpeed() != 0);
}
 */