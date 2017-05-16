#include "physical.h"

#include "channel.h"

#include "../platform/e-puck/adc.h"
#include "../platform/e-puck/leds.h"
#include "../interrupts.h"
#include "../memory.h"

#define DEBUG_COM
//#define SHOW_SENSOR_LED
#define BODY_INDICATOR

#ifdef DEBUG_COM
#include "../platform/e-puck/uart.h"
#endif

uint sensorReadings[8] ={0};
uint minReadings = 0;

typedef enum rx_state{
    waiting,
    receiving,
    sending
} Sys_RX_state;

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
void ComReceived3(void);

void CombineSensors(void);
void ReadFromSensors(void);
void WriteToSensors(void);


void Sys_Init_PhysicalLayer(){
    //make sure ADCs are initialised
}

void Sys_Start_PhysicalLayer(){
    
    clearIRs();
    Sys_Start_ChannelCalibration();
    
    Sys_Start_AtomicSection();
    Sys_Subscribe_ADCChannelProcessor(Prx0, ComSensor0);
    Sys_Subscribe_ADCChannelProcessor(Prx1, ComSensor1);
    Sys_Subscribe_ADCChannelProcessor(Prx2, ComSensor2);
    Sys_Subscribe_ADCChannelProcessor(Prx3, ComSensor3);
    Sys_Subscribe_ADCChannelProcessor(Prx4, ComSensor4);
    Sys_Subscribe_ADCChannelProcessor(Prx5, ComSensor5);
    Sys_Subscribe_ADCChannelProcessor(Prx6, ComSensor6);
    Sys_Subscribe_ADCChannelProcessor(Prx7, ComSensor7); 
    Sys_Subscribe_ADCFinish(ProxDone,CombineSensors);   
    Sys_End_AtomicSection(); 
    
    clearLEDs();
}

void Sys_Stop_PhysicalLayer(){
    Sys_Subscribe_ADCFinish(ProxDone, 0);    
}

void ComSensor0(uint s){
    sensorReadings[0] = s;
#ifdef SHOW_SENSOR_LED
    if( s < Sys_ComThreshold()){
        LED0 = 1;
    }else{
        LED0 = 0;
    }
#endif
}

void ComSensor1(uint s){
    sensorReadings[1] = s;
#ifdef SHOW_SENSOR_LED
    if( s < Sys_ComThreshold()){
        LED1 = 1;
    }else{
        LED1 = 0;
    }
#endif
}

void ComSensor2(uint s){
    sensorReadings[2] = s;
#ifdef SHOW_SENSOR_LED
    if( s < Sys_ComThreshold()){
        LED2 = 1;
    }else{
        LED2 = 0;
    }
#endif
}

void ComSensor3(uint s){
    sensorReadings[3] = s;
#ifdef SHOW_SENSOR_LED
    if( s < Sys_ComThreshold()){
        LED3 = 1;
    }else{
        LED3 = 0;
    }
#endif
}

void ComSensor4(uint s){
    sensorReadings[4] = s;
#ifdef SHOW_SENSOR_LED
    if( s < Sys_ComThreshold()){
        LED5 = 1;
    }else{
        LED5 = 0;
    }
#endif
}

void ComSensor5(uint s){
    sensorReadings[5] = s;
#ifdef SHOW_SENSOR_LED
    if( s < Sys_ComThreshold()){
        LED6 = 1;
    }else{
        LED6 = 0;
    }
#endif
}

void ComSensor6(uint s){
    sensorReadings[6] = s;
#ifdef SHOW_SENSOR_LED
    if( s < Sys_ComThreshold()){
        LED7 = 1;
    }else{
        LED7 = 0;
    }
#endif
}

void ComSensor7(uint s){
    sensorReadings[7] = s;
#ifdef SHOW_SENSOR_LED
    if( s < Sys_ComThreshold()){
        LED0 = 1;
    }else{
        LED0 = 0;
    }
#endif
}

#define MAX_SIGNAL_DELAY 1

void CombineSensors(void){
    ReadFromSensors();
    WriteToSensors();
}

void ReadFromSensors(void){
    static Sys_RawMessageList  *current_Msg = 0;
    
    uint min_value = 0xFFFF;
    uint i = 0;
    
    for(i = 0; i < 8; i++){
        if( min_value > sensorReadings[i]){
            min_value = sensorReadings[i];
        }
    }
    
    uint new_bit = 0;
    if(min_value < Sys_ComThreshold() - THRESHOLD){
        new_bit = 1;
    }
    
    switch(rxState){
        case sending:
            return;
        case waiting:
            if(new_bit == 0){
                return;
            }
            
            current_Msg = (Sys_RawMessageList  *) Sys_Malloc(sizeof(Sys_RawMessageList));
            Sys_Memset(current_Msg,sizeof(Sys_RawMessageList),0); 
            rxState = receiving;
        case receiving:
        default:
            break;
    }
    
    uint seg        = current_Msg->position / 15;
    uint bit_pos    = current_Msg->position % 15;
    current_Msg->message[seg] |= new_bit << bit_pos;
    current_Msg->position++;   
    
    if(current_Msg->position >= 75){
        Sys_Start_AtomicSection();
            *sys_InMsg_ListEnd = current_Msg;
            sys_InMsg_ListEnd = &(current_Msg->next);
        Sys_End_AtomicSection();
        
        current_Msg = 0;
        rxState = waiting;
    }
}
void WriteToSensors(void){
    static Sys_RawMessageList  *current_Msg = 0;
    
    if(rxState == receiving){
        return;
    }
    
    if(current_Msg == 0){
        if(sys_OutMsg_List == 0){
            return;
        }
        
        Sys_Start_AtomicSection();
            current_Msg = sys_OutMsg_List;
            sys_OutMsg_List = sys_OutMsg_List->next;
            if(sys_OutMsg_List == 0){
                sys_OutMsg_List_End = &sys_OutMsg_List;
            }
        Sys_End_AtomicSection();
        
        current_Msg->next = 0;
        current_Msg->position = 0;
    }
        
    rxState = sending;
    
    uint seg        = current_Msg->position / 15;
    uint bit_pos    = current_Msg->position % 15;
    
    if(current_Msg->message[seg] & (1 << bit_pos)){ // == 1
        setIRs();
#ifdef BODY_INDICATOR
        BODY_LED = 1;
#endif
    }else{
        clearIRs();
#ifdef BODY_INDICATOR
        BODY_LED = 0;
#endif
    }
        
    current_Msg->position++;  
    
    if(current_Msg->position >= 75){
        Sys_Free(current_Msg);
        current_Msg = 0;
        rxState = waiting;
        clearIRs();
    }
}

void Sys_AddOutMessage(Sys_RawMessageList *element){
    if(element == 0){
        return;
    }
    
    Sys_Start_AtomicSection();
        *sys_OutMsg_List_End = element;
        sys_OutMsg_List_End  = &(element->next);
    Sys_End_AtomicSection();    
}

Sys_RawMessageList *Sys_GetNextInMessage(){
    if(sys_InMsg_List == 0){
        return 0;
    }
    
    Sys_Start_AtomicSection();
    Sys_RawMessageList *oldest_Msg = sys_InMsg_List;
    sys_InMsg_List = sys_InMsg_List->next;
    if(sys_InMsg_List == 0){
        sys_InMsg_ListEnd = &sys_InMsg_List;
    }
    Sys_End_AtomicSection(); 
    
    return oldest_Msg;
}

uint Sys_ListLength(Sys_RawMessageList *list){
    uint nelements = 0;
    
    while(list != 0){
        nelements++;
        list = list->next;
    }
    
    return nelements;
}

void Sys_SendTestPattern(void){
    Sys_RawMessageList *element = (Sys_RawMessageList *) Sys_Malloc(sizeof(Sys_RawMessageList));
    
    element->next = 0;
    element->position = 0;
    element->message[0] = 0x5555;
    element->message[1] = 0x2aaa;
    element->message[2] = 0x5555;
    element->message[3] = 0x2aaa;
    element->message[4] = 0x5555;
    
#ifdef DEBUG_COM
    Sys_Writeto_UART1(element->message, 10);
#endif
    Sys_AddOutMessage(element);  
}