
#include "adc.h"
#include "../../definitions.h"
#include "../../events/events.h"

uint Sys_Prox_trasform(uint);
void Sys_Prox0_preprocessor(uint);
void Sys_Prox1_preprocessor(uint);
void Sys_Prox2_preprocessor(uint);
void Sys_Prox3_preprocessor(uint);
void Sys_Prox4_preprocessor(uint);
void Sys_Prox5_preprocessor(uint);
void Sys_Prox6_preprocessor(uint);
void Sys_Prox7_preprocessor(uint);

uint sys_distances[8] = {0};

void Sys_Init_Proximity(void){

    Sys_Init_ADC();

    Sys_Register_Event(SYS_EVENT_IO_PROX_0);
    Sys_Register_Event(SYS_EVENT_IO_PROX_1);
    Sys_Register_Event(SYS_EVENT_IO_PROX_2);
    Sys_Register_Event(SYS_EVENT_IO_PROX_3);
    Sys_Register_Event(SYS_EVENT_IO_PROX_4);
    Sys_Register_Event(SYS_EVENT_IO_PROX_5);
    Sys_Register_Event(SYS_EVENT_IO_PROX_6);
    Sys_Register_Event(SYS_EVENT_IO_PROX_7);

    Sys_Subscribe_ADCChannelProcessor(Prx0, Sys_Prox0_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx1, Sys_Prox1_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx2, Sys_Prox2_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx3, Sys_Prox3_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx4, Sys_Prox4_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx5, Sys_Prox5_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx6, Sys_Prox6_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx7, Sys_Prox7_preprocessor);    
       
}

void Sys_Start_Proximity(void){
    
    
}

uint Sys_Prox_trasform(uint i){
    return i;
}

void Sys_Prox0_preprocessor(uint value){
    uint dist = Sys_Prox_trasform(value);
    
    sys_distances[0] = dist;
    Sys_Send_IntEvent(SYS_EVENT_IO_PROX_0, dist);
    
}
void Sys_Prox1_preprocessor(uint value){
    
    uint dist = Sys_Prox_trasform(value);
    
    sys_distances[1] = dist;
    Sys_Send_IntEvent(SYS_EVENT_IO_PROX_1, dist);
    
}
void Sys_Prox2_preprocessor(uint value){
    
    uint dist = Sys_Prox_trasform(value);
    
    sys_distances[2] = dist;
    Sys_Send_IntEvent(SYS_EVENT_IO_PROX_2, dist);
    
}
void Sys_Prox3_preprocessor(uint value){
    
    uint dist = Sys_Prox_trasform(value);
    
    sys_distances[3] = dist;
    Sys_Send_IntEvent(SYS_EVENT_IO_PROX_3, dist);
    
}
void Sys_Prox4_preprocessor(uint value){
    
    uint dist = Sys_Prox_trasform(value);
    
    sys_distances[4] = dist;
    Sys_Send_IntEvent(SYS_EVENT_IO_PROX_4, dist);
    
}
void Sys_Prox5_preprocessor(uint value){
    
    uint dist = Sys_Prox_trasform(value);
    
    sys_distances[5] = dist;
    Sys_Send_IntEvent(SYS_EVENT_IO_PROX_5, dist);
    
}
void Sys_Prox6_preprocessor(uint value){
    
    uint dist = Sys_Prox_trasform(value);
    
    sys_distances[6] = dist;
    Sys_Send_IntEvent(SYS_EVENT_IO_PROX_6, dist);
    
}
void Sys_Prox7_preprocessor(uint value){
    
    uint dist = Sys_Prox_trasform(value);
    
    sys_distances[7] = dist;
    Sys_Send_IntEvent(SYS_EVENT_IO_PROX_7, dist);
    
}

uint Sys_Get_Prox(uint index){
    return sys_distances[index];
}