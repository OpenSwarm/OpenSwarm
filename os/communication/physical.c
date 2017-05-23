#include "physical.h"

#include "channel.h"

#include "../platform/e-puck/adc.h"
#include "../platform/e-puck/leds.h"
#include "../interrupts.h"
#include "../memory.h"

//#define DEBUG_COM
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

#define THRESHOLD 0

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

void apply_0_ToChannel(void);
void apply_1_ToChannel(void);
void clearChannel(void);

void Sys_Init_PhysicalLayer(){
    
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

#define ADC_PER_BIT 2

void CombineSensors(void){
    ReadFromSensors_2bits();
    WriteToSensors_2bits();
}

void ReadFromSensors(void){
    static Sys_RawMessageList  *current_Msg = 0;
    uint new_bit = 0;
    
    uint min_value = 0xFFFF;
    
    uint i = 0;
    for(i = 0; i < 8; i++){
        if( min_value > sensorReadings[i]){
            min_value = sensorReadings[i];
        }
    }
    
    switch(rxState){
        case sending:
            return;
        case waiting:
            if(min_value > Sys_ComThreshold() - THRESHOLD){//bit = 0
                return;
            }
            if( (current_Msg ) == 0 ){
               current_Msg = (Sys_RawMessageList  *) Sys_Malloc(sizeof(Sys_RawMessageList));
            }
            
            Sys_Memset(current_Msg,sizeof(Sys_RawMessageList),0); 
            
            rxState = receiving;
            break;
        case receiving:
            break;
        default:
            break;
    }
    
    if( min_value > (Sys_ComThreshold() - THRESHOLD)){// bit_value > ADC_PER_BIT*(Sys_ComThreshold() - THRESHOLD)){
        new_bit = 0;
    }else{
        new_bit = 1;
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

void ReadFromSensors2(void){
    static Sys_RawMessageList  *current_Msg = 0;
//    static uint readings[ADC_PER_BIT*2+1] = {0};
//    static uint synch_pos = 0;
    uint new_bit = 0;
    
    uint min_value = 0xFFFF;
    uint i = 0;
    
    for(i = 0; i < 8; i++){
        if( min_value > sensorReadings[i]){
            min_value = sensorReadings[i];
        }
    }
    
    
    switch(rxState){
        case sending:
            return;
        case waiting:
            //readings[0] = min_value;
            //synch_pos = 1;
            
            if(min_value > Sys_ComThreshold() - THRESHOLD){//bit = 0
                return;
            }
            
            current_Msg = (Sys_RawMessageList  *) Sys_Malloc(sizeof(Sys_RawMessageList));
            Sys_Memset(current_Msg,sizeof(Sys_RawMessageList),0); 
            rxState = receiving;
            break;
        
//        case synching:
            /*
            rxState = synching;
            readings[synch_pos] = min_value;
            synch_pos++;
            
            if(synch_pos < ADC_PER_BIT*2+1){
                return;
            }
            
            int left = 0;
            int thres = 3*(Sys_ComThreshold() - THRESHOLD);
            bool fault = false;

            uint  value_r_1 = readings[1] + readings[2];
            uint  value_l_1 = readings[0] + value_r_1;
            value_r_1 += readings[3];

            uint  value_r_0 = readings[5] + readings[6];
            uint  value_l_0 = readings[4] + value_r_0;
            value_r_0 += readings[7];

            if(value_l_1 < value_r_1){
                if(value_l_1 < thres){
                    left++;
                }else{
                    fault = true;
                }
            }else if(value_l_1 > value_r_1){
                if(value_r_1 < thres){
                    left--;
                }else{
                    fault = true;
                }
            }

            if(value_l_0 < value_r_0){
                if(value_r_0 > thres){
                    left--;
                }else{
                    fault = true;
                }
            }else if(value_l_0 > value_r_0){
                if(value_l_0 > thres){
                    left++;
                }else{
                    fault = true;
                }
            }

            if(fault){
                uint j = 1;
                uint new_zero = 1;
                uint thes_1 = (Sys_ComThreshold() - THRESHOLD);

                for(j = 1; j < ADC_PER_BIT*2+1; j++ ){
                    if(readings[j] < thes_1){
                        new_zero = j;
                        break;
                    }
                }

                for(j = new_zero; j < ADC_PER_BIT*2+1; j++ ){
                    readings[j-new_zero] = readings[j];
                }
                synch_pos = ADC_PER_BIT*2+1 - new_zero;

                if(synch_pos == 0){
                    rxState = waiting;
                }

                return;
            }

            if(left == 0){
                if(readings[7] > (Sys_ComThreshold() - THRESHOLD)){//bit == 0
                    left--;
                }else{
                    left++;
                }
            }

            if(left > 0){//is it left
                readings[0] = readings[7];
                synch_pos = 1;
            }else{
                synch_pos = 0;
            }
            */
            return;
        case receiving:
            //readings[synch_pos] = min_value;
            //synch_pos++;
        default:
            break;
    }
    
    /*
    readings[synch_pos] = min_value;
    synch_pos++;
    
    if(synch_pos < ADC_PER_BIT){
        return;
    }else{
        synch_pos=0;
    }
    
    synch_pos = 0;
    uint bit_value = 0;
    for(i = 0; i < ADC_PER_BIT; i++){
        bit_value += readings[i];
    }
    */
    if( min_value > (Sys_ComThreshold() - THRESHOLD)){// bit_value > ADC_PER_BIT*(Sys_ComThreshold() - THRESHOLD)){
        new_bit = 0;
    }else{
        new_bit = 1;
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
        clearChannel();
        return;
    }
    
    if(current_Msg == 0){
        if(sys_OutMsg_List == 0){
            clearChannel();
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
        apply_1_ToChannel();
    }else{
        apply_0_ToChannel();
    }
    
    current_Msg->position++; 
    
    if(current_Msg->position >= 75){
        Sys_Free(current_Msg);
        current_Msg = 0;
        rxState = waiting;
    }
}

void WriteToSensors2(void){
    static Sys_RawMessageList  *current_Msg = 0;
    static uint bit_counter = 0;
    
    if(rxState == receiving){
        clearChannel();
        return;
    }
    
    if(current_Msg == 0){
        if(sys_OutMsg_List == 0){
            clearChannel();
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
        apply_1_ToChannel();
    }else{
        apply_0_ToChannel();
    }
        
    if(bit_counter < ADC_PER_BIT-1){
        bit_counter++;
    }else{
        current_Msg->position++;  
        bit_counter = 0;
    }
    
    if(current_Msg->position > 75){
        Sys_Free(current_Msg);
        current_Msg = 0;
        rxState = waiting;
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
void ReadFromSensors_old(void){ 
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
void WriteToSensors_old(void){ 
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
        apply_1_ToChannel();
    }else{ 
        apply_0_ToChannel();
    } 
         
    current_Msg->position++;   
     
    if(current_Msg->position >= 75){ 
        Sys_Free(current_Msg); 
        current_Msg = 0; 
        rxState = waiting; 
        clearChannel();
    } 
}

#define ADCs_PER_BIT 2

void ReadFromSensors_2bits(void){ 
    static Sys_RawMessageList  *current_Msg = 0; 
    static uint measurement_counter = 0;
     
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
            measurement_counter = 1;
            return;
        case receiving: 
        default: 
            break; 
    } 
    
    
    measurement_counter++;
    if(measurement_counter < ADCs_PER_BIT){
        return;
    }
    
    
    uint seg        = current_Msg->position / 15; 
    uint bit_pos    = current_Msg->position % 15; 
    current_Msg->message[seg] |= new_bit << bit_pos; 
    current_Msg->position++;    
     
    if(current_Msg->position >= 75){ 
                 
#ifdef DEBUG_COM
        Sys_Writeto_UART1("R", 1);
        Sys_Writeto_UART1(current_Msg->message, 10);
        Sys_Writeto_UART1("%", 1);
#endif
        Sys_Start_AtomicSection(); 
            *sys_InMsg_ListEnd = current_Msg; 
            sys_InMsg_ListEnd = &(current_Msg->next); 
        Sys_End_AtomicSection(); 
         
        current_Msg = 0; 
        rxState = waiting; 
    }
    measurement_counter = 0;
} 
void WriteToSensors_2bits(void){ 
    static Sys_RawMessageList  *current_Msg = 0; 
    static uint measurement_counter = 0;
     
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
         
#ifdef DEBUG_COM
        Sys_Writeto_UART1("W", 1);
        Sys_Writeto_UART1(current_Msg->message, 10);
        Sys_Writeto_UART1("M", 1);
#endif
        current_Msg->next = 0; 
        current_Msg->position = 0; 
    } 
         
    rxState = sending; 
     
    uint seg        = current_Msg->position / 15; 
    uint bit_pos    = current_Msg->position % 15; 
     
    if(current_Msg->message[seg] & (1 << bit_pos)){ // == 1 
        apply_1_ToChannel();
    }else{ 
        apply_0_ToChannel();
    } 
        
    measurement_counter++;
    if((measurement_counter % ADCs_PER_BIT) == 0){
        current_Msg->position++;   
    }
     
    if(current_Msg->position >= 75){ 
        Sys_Free(current_Msg); 
        current_Msg = 0; 
        rxState = waiting; 
        clearChannel();
    } 
}
