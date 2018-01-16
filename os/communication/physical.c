#include "physical.h"

#include "channel.h"

#include "../platform/e-puck/adc.h"
#include "../platform/e-puck/leds.h"
#include "../platform/e-puck/physical_HDI.h"
#include "../interrupts.h"
#include "../memory.h"

#define DEBUG_COM
#define SHOW_SENSOR_LED
//#define BODY_INDICATOR

#ifdef DEBUG_COM
#include "../platform/e-puck/uart.h"
#endif

int sensorReadings[8] ={0};
int minReadings = 0;

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
void ReadFromSensors_old(void);
void WriteToSensors_old(void);
void ReadFromSensors_2bits(void);
void WriteToSensors_2bits(void);
void WriteToSensors_1adc(void);

void apply_0_ToChannel(void);
void apply_1_ToChannel(void);
void clearChannel(void);

void Sys_Init_PhysicalLayer(){
   Sys_Init_PhysicalSendingChannel();
    Sys_Register_SendingFunction(WriteToSensors_1adc);
    
    sys_InMsg_List = 0;
    sys_InMsg_ListEnd = &sys_InMsg_List;

    sys_OutMsg_List = 0;
    sys_OutMsg_List_End = &sys_OutMsg_List;
    //make sure ADCs are initialised
    rxState = waiting;
}

void Sys_Start_PhysicalLayer(){
    
    clearIRs();
    Sys_Start_ChannelCalibration();
    clearLEDs();
    
    Sys_Start_PhysicalSendingChannel();
    
    Sys_Start_AtomicSection();
    Sys_Subscribe_ADCChannelProcessor(Prx0, ComSensor0);
    Sys_Subscribe_ADCChannelProcessor(Prx1, ComSensor1);
    Sys_Subscribe_ADCChannelProcessor(Prx2, ComSensor2);
    Sys_Subscribe_ADCChannelProcessor(Prx3, ComSensor3);
    Sys_Subscribe_ADCChannelProcessor(Prx4, ComSensor4);
    Sys_Subscribe_ADCChannelProcessor(Prx5, ComSensor5);
    Sys_Subscribe_ADCChannelProcessor(Prx6, ComSensor6);
    Sys_Subscribe_ADCChannelProcessor(Prx7, ComSensor7); 
    Sys_Subscribe_ADCFinish(ProxDone,ReadFromSensors_2bits);   
    Sys_End_AtomicSection(); 
    
}

void Sys_Stop_PhysicalLayer(){
    Sys_Subscribe_ADCFinish(ProxDone, 0);    
}

void ComSensor0(uint s){
    int bg = Sys_ComBackground(0);
    if(bg >= s){
        sensorReadings[0] = bg-s;
    }else{
        sensorReadings[0] = 0;
    }
#ifdef SHOW_SENSOR_LED
    if( ((int)bg)-s > Sys_GetThreshold()){
        LED0 = 1;
    }else{
        LED0 = 0;
    }
#endif
}

void ComSensor1(uint s){
    int bg = Sys_ComBackground(1);
    if(bg >= s){
        sensorReadings[1] = bg-s;
    }else{
        sensorReadings[1] = 0;
    }
#ifdef SHOW_SENSOR_LED
    if( ((int)bg)-s > Sys_GetThreshold()){
        LED1 = 1;
    }else{
        LED1 = 0;
    }
#endif
}

void ComSensor2(uint s){
    int bg = Sys_ComBackground(2);
    if(bg >= s){
        sensorReadings[2] = bg-s;
    }else{
        sensorReadings[2] = 0;
    }
#ifdef SHOW_SENSOR_LED
    if( ((int)bg)-s > Sys_GetThreshold()){
        LED2 = 1;
    }else{
        LED2 = 0;
    }
#endif
}

void ComSensor3(uint s){
    int bg = Sys_ComBackground(3);
    if(bg >= s){
        sensorReadings[3] = bg-s;
    }else{
        sensorReadings[3] = 0;
    }
#ifdef SHOW_SENSOR_LED
    if( ((int)bg)-s > Sys_GetThreshold()){
        LED3 = 1;
    }else{
        LED3 = 0;
    }
#endif
}

void ComSensor4(uint s){
    int bg = Sys_ComBackground(4);
    if(bg >= s){
        sensorReadings[4] = bg-s;
    }else{
        sensorReadings[4] = 0;
    }
#ifdef SHOW_SENSOR_LED
    if( ((int)bg)-s > Sys_GetThreshold()){
        LED5 = 1;
    }else{
        LED5 = 0;
    }
#endif
}

void ComSensor5(uint s){
    int bg = Sys_ComBackground(5);
    if(bg >= s){
        sensorReadings[5] = bg-s;
    }else{
        sensorReadings[5] = 0;
    }
#ifdef SHOW_SENSOR_LED
    if( ((int)bg)-s > Sys_GetThreshold()){
        LED6 = 1;
    }else{
        LED6 = 0;
    }
#endif
}

void ComSensor6(uint s){
    int bg = Sys_ComBackground(6);
    if(bg >= s){
        sensorReadings[6] = bg-s;
    }else{
        sensorReadings[6] = 0;
    }
#ifdef SHOW_SENSOR_LED
    if( ((int)bg)-s > Sys_GetThreshold()){
        LED7 = 1;
    }else{
        LED7 = 0;
    }
#endif
}

void ComSensor7(uint s){
    int bg = Sys_ComBackground(7);
    if(bg >= s){
        sensorReadings[7] = bg-s;
    }else{
        sensorReadings[7] = 0;
    }
#ifdef SHOW_SENSOR_LED
    if( ((int)bg)-s > Sys_GetThreshold()){
        LED0 = 1;
    }else{
        LED0 = 0;
    }
#endif
}

#define ADC_PER_BIT 1

void CombineSensors(void){
    ReadFromSensors_2bits();
    WriteToSensors_2bits();
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
    element->message[0] = 0x7FFF;
    element->message[1] = 0x00FF;
    element->message[2] = 0x0F0F;
    element->message[3] = 0x5555;
    element->message[4] = 0x7FFF;
    
#ifdef DEBUG_COM
    Sys_Writeto_UART1(element->message, 10);
#endif
    Sys_AddOutMessage(element);  
}


void apply_0_ToChannel(void){
        clearIRs();
#ifdef BODY_INDICATOR
        BODY_LED = 0;
#endif
    
}
void apply_1_ToChannel(void){
        setIRs();
#ifdef BODY_INDICATOR
        BODY_LED = 1;
#endif
    
}
void clearChannel(void){
        clearIRs();
#ifdef BODY_INDICATOR
        BODY_LED = 0;
#endif
    
}
#define MAX_FULLMESSAGE 15

void ReadFromSensors_2bits(void){ 
    static Sys_RawMessageList  *current_inMsg = 0; 
    static uint measurement_counter = 0;
    static uint threshold_read = 0;
     
    uint max_value = 0; 
    uint max_sensor = -1;
    
    uint i = 0;
    for(i = 0; i < 8; i++){
        if( max_value < sensorReadings[i]){
            max_value = sensorReadings[i];
            max_sensor = i;
        }
    }
    
    if(max_sensor == -1){
        max_value = 0;
        max_sensor = 0;
    }
     
    uint new_bit = 0;
//    uint noise_temp =  0;       
            
    switch(rxState){ 
        case sending: 
            return; 
        case waiting:
            if( max_value > ((int) Sys_GetThreshold()) ){ 
                threshold_read = (max_value+Sys_GetThreshold())/2;
            }else{ 
                Sys_UpdateBackground(max_sensor, max_value);
                measurement_counter = 0;
                return; 
            } 
                        
            measurement_counter++;
            if(measurement_counter < ADCs_PER_BIT){
                return;
            } 
            
            current_inMsg = (Sys_RawMessageList  *) Sys_Malloc(sizeof(Sys_RawMessageList)); 
            Sys_Memset(current_inMsg,sizeof(Sys_RawMessageList),0);  
            
             
            current_inMsg->position = 0; 
            measurement_counter = 0;
            rxState = receiving;    
            return;
        case receiving: 
            if(max_value > threshold_read){ 
                new_bit = 1; 
            }else{
                new_bit = 0;
            }
        default: 
            break; 
    } 
    
    measurement_counter++;
    if(measurement_counter < ADCs_PER_BIT){
        return;
    } 
        
    uint seg        = current_inMsg->position / 15; 
    uint bit_pos    = current_inMsg->position % 15; 
    current_inMsg->message[seg] |= new_bit << bit_pos; 
    current_inMsg->position++;    
    
    if( ((current_inMsg->position == 16) && (current_inMsg->message[0] <= 3))|| ((current_inMsg->position == 31) && (current_inMsg->message[1] == 0)) ){
        rxState = waiting; 
        Sys_Free(current_inMsg);
        current_inMsg = 0;
    }
       
    if(current_inMsg->position >= 75){ 
       
        
        static uint full_counter = 0;
        if(     (current_inMsg->message[0] == 0xEFFF || current_inMsg->message[0] == 0xFFFF) && 
                (current_inMsg->message[1] == 0xEFFF || current_inMsg->message[1] == 0xFFFF) && 
                (current_inMsg->message[2] == 0xEFFF || current_inMsg->message[2] == 0xFFFF) && 
                (current_inMsg->message[3] == 0xEFFF || current_inMsg->message[3] == 0xFFFF) && 
                (current_inMsg->message[4] == 0xEFFF || current_inMsg->message[4] == 0xFFFF) ){
        //IS the message just 1s?
            full_counter++;
            FRONT_LED = 1;
        }else{
            full_counter = 0;
            FRONT_LED = 0;
        }
        
        if(full_counter >= MAX_FULLMESSAGE){
            Sys_SetComBackground(max_sensor, max_value);
            full_counter = 0;
        }
       
             
#ifdef DEBUG_COM
        Sys_Writeto_UART1(current_inMsg->message, 10);
#endif
        
        Sys_Start_AtomicSection(); 
            *sys_InMsg_ListEnd = current_inMsg; 
            sys_InMsg_ListEnd = &(current_inMsg->next); 
            
            current_inMsg = 0; 
            rxState = waiting; 
        Sys_End_AtomicSection(); 
         
    }
    measurement_counter = 0;
} 

void WriteToSensors_2bits(void){ 
    static Sys_RawMessageList  *current_outMsg = 0; 
    static uint measurement_counter = 0;
     
    if(rxState == receiving){ 
        clearChannel();
        measurement_counter = 0;
        
        return; 
    } 
     
    if(current_outMsg == 0){ 
        if(sys_OutMsg_List == 0){ 
            clearChannel();
            measurement_counter = 0;
        
            return; 
        } 
         
        
        Sys_Start_AtomicSection(); 
            current_outMsg = sys_OutMsg_List; 
            sys_OutMsg_List = sys_OutMsg_List->next; 
            if(sys_OutMsg_List == 0){ 
                sys_OutMsg_List_End = &sys_OutMsg_List; 
            } 
        Sys_End_AtomicSection(); 
         
            
#ifdef DEBUG_COM
        Sys_Writeto_UART1(current_outMsg->message, 10);
#endif
        
        rxState = sending;
            
        apply_1_ToChannel();
        current_outMsg->next = 0; 
        current_outMsg->position = 0;
        
        return;
    }
        
    measurement_counter++;
    if((measurement_counter % ADCs_PER_BIT) != 0){
        return;
    }
     
    measurement_counter = 0;
    uint seg        = current_outMsg->position / 15; 
    uint bit_pos    = current_outMsg->position % 15; 
     
    if(current_outMsg->message[seg] & (1 << bit_pos)){ // == 1 
        apply_1_ToChannel();
    }else{ 
        apply_0_ToChannel();
    } 
        
//   measurement_counter++;
//    if((measurement_counter % ADCs_PER_BIT) == 0){
        current_outMsg->position++;   
//    }
     
    if(current_outMsg->position > 75){ 
        clearChannel();
        
        Sys_Free(current_outMsg); 
        current_outMsg = 0; 
        
        rxState = waiting; 
        
    } 
}
void WriteToSensors_1adc(void){ 
    static Sys_RawMessageList  *current_outMsg = 0; 
//    static uint measurement_counter = 0;
     
    if(rxState == receiving){ 
        clearChannel();
//        measurement_counter = 0;      
        return; 
    } 
     
    if(current_outMsg == 0){ 
        if(sys_OutMsg_List == 0){ 
            clearChannel();
//            measurement_counter = 0;      
            return; 
        } 
        
        Sys_Start_AtomicSection(); 
            current_outMsg = sys_OutMsg_List; 
            
            sys_OutMsg_List = sys_OutMsg_List->next; 
            
            if(sys_OutMsg_List == 0){ 
                sys_OutMsg_List_End = &sys_OutMsg_List; 
            } 
            
            current_outMsg->next = 0; 
            current_outMsg->position = 0;
            rxState = sending;
        Sys_End_AtomicSection(); 
            
#ifdef DEBUG_COM
        Sys_Writeto_UART1(current_outMsg->message, 10);
#endif 
        
        apply_1_ToChannel();
      
        return;
    }
        
//    measurement_counter++;
//    if((measurement_counter % ADCs_PER_BIT) != 0){
//        return;
//    }
     
//    measurement_counter = 0;
    uint seg        = current_outMsg->position / 15; 
    uint bit_pos    = current_outMsg->position % 15; 
     
    if(current_outMsg->message[seg] & (1 << bit_pos)){ // == 1 
        apply_1_ToChannel();
    }else{ 
        apply_0_ToChannel();
    } 
        
//   measurement_counter++;
//    if((measurement_counter % ADCs_PER_BIT) == 0){
    current_outMsg->position++;   
//    }
     
    if(current_outMsg->position > 75){ 
        clearChannel();
        
        Sys_Start_AtomicSection();
            Sys_Free(current_outMsg); 
            current_outMsg = 0; 
        
           rxState = waiting;
        Sys_End_AtomicSection();   
    } 
}
