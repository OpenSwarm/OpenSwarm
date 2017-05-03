#include "communication.h"

void Sys_Init_Communication(){
    Sys_Init_ADC();//make sure ADCs are initialised
}

void Sys_Start_Communication();
void Sys_Stop_Communication();
    
void Sys_Send_Message(void *data, size_t length);
