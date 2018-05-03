#include "physical.h"

#include "channel.h"

#include "../platform/e-puck/adc.h"
#include "../platform/e-puck/leds.h"
#include "../platform/e-puck/physical_HDI.h"
#include "../interrupts.h"
#include "../memory.h"

//#define SHOW_SENSOR_LED
//#define BODY_INDICATOR

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
    sensorReadings[0] = s;
#ifdef SHOW_SENSOR_LED
    if( s <= Sys_ComThreshold()){
        LED0 = 1;
    }else{
        LED0 = 0;
    }
#endif
}

void ComSensor1(uint s){
    sensorReadings[1] = s;
#ifdef SHOW_SENSOR_LED
    if( s <= Sys_ComThreshold()){
        LED1 = 1;
    }else{
        LED1 = 0;
    }
#endif
}

void ComSensor2(uint s){
    sensorReadings[2] = s;
#ifdef SHOW_SENSOR_LED
    if( s <= Sys_ComThreshold()){
        LED2 = 1;
    }else{
        LED2 = 0;
    }
#endif
}

void ComSensor3(uint s){
    sensorReadings[3] = s;
#ifdef SHOW_SENSOR_LED
    if( s <= Sys_ComThreshold()){
        LED3 = 1;
    }else{
        LED3 = 0;
    }
#endif
}

void ComSensor4(uint s){
    sensorReadings[4] = s;
#ifdef SHOW_SENSOR_LED
    if( s <= Sys_ComThreshold()){
        LED5 = 1;
    }else{
        LED5 = 0;
    }
#endif
}

void ComSensor5(uint s){
    sensorReadings[5] = s;
#ifdef SHOW_SENSOR_LED
    if( s <= Sys_ComThreshold()){
        LED6 = 1;
    }else{
        LED6 = 0;
    }
#endif
}

void ComSensor6(uint s){
    sensorReadings[6] = s;
#ifdef SHOW_SENSOR_LED
    if( s <= Sys_ComThreshold()){
        LED7 = 1;
    }else{
        LED7 = 0;
    }
#endif
}

void ComSensor7(uint s){
    sensorReadings[7] = s;
#ifdef SHOW_SENSOR_LED
    if( s <= Sys_ComThreshold()){
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
     
    uint min_value = 0xFFFF; 
    uint min_sensor = -1;
    
    uint i = 0;
    for(i = 0; i < 8; i++){
        if( min_value > sensorReadings[i]){
            min_value = sensorReadings[i];
            min_sensor = i;
        }
    }
     
    uint new_bit = 0;
    uint noise_temp =  0;       
            
    switch(rxState){ 
        case sending: 
            return; 
        case waiting:
            noise_temp =  Sys_ComBackground(min_sensor);
            if( min_value < noise_temp-Sys_GetThreshold() ){ 
                threshold_read = (min_value+noise_temp-Sys_GetThreshold())/2 + 1;
            }else{ 
                if(min_value < noise_temp){//adapting to the light conditions
                    Sys_SetComBackground(min_sensor, noise_temp-1);
                }else if(min_value > noise_temp){
                    Sys_SetComBackground(min_sensor, noise_temp+1);
                }
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
            if(min_value < threshold_read){ 
                new_bit = 1; 
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
        
        Sys_Start_AtomicSection(); 
        int m = 0;
        for(m = 0; m < 5; m++){
               if(current_inMsg->message[m] == 0xEFFF || current_inMsg->message[m] == 0xFFFF){
                   full_counter++;
               }else{
                   full_counter = 0;
               }
        }
        Sys_End_AtomicSection();
        
        if(full_counter >= MAX_FULLMESSAGE){
            Sys_SetComBackground(min_sensor, min_value);
             
        Sys_Start_AtomicSection(); 
            Sys_Free(current_inMsg);
            current_inMsg = 0;
            rxState = waiting;
        Sys_End_AtomicSection(); 
            
            BODY_LED = 1;
        }else{      
        
            Sys_Start_AtomicSection(); 
                *sys_InMsg_ListEnd = current_inMsg; 
                sys_InMsg_ListEnd = &(current_inMsg->next); 

                current_inMsg = 0; 
                rxState = waiting; 
            Sys_End_AtomicSection(); 
            
            BODY_LED = 0;
        }
         
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
