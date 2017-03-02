/*
 * File:   main.c
 * Author: Stefan M. Trenkwalder
 *
 * Created on 07 July 2014, 14:15
 *
 * LICENSE: adapted FreeBSD License
 * Copyright (c) 2015, Stefan M. Trenkwalder
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. If this or parts of this source code (as code or binary) is, in any form, used for an commercial product or service (in any form), this product or service must provide a clear notice/message to any user/customer that OpenSwarm was used in this product.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdlib.h>
#include <stdbool.h>       /* Includes true/false definition                  */
#include <stdio.h>
#include <math.h>

#include "os/system.h"        /* System funct/params, like osc/peripheral config */
#include "os/memory.h"
#include "os/interrupts.h"
#include "os/platform/e-puck/adc.h"

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void bluetooth_reader(uint8 data);


//uint8 prox_readings[36] = {'@', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//                          0x00,  ':', 0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 
//                          0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 
//                          0x0F, 0xFF, '\r', '\n'};
//uint prox_median[9] = {4095,4095,4095,4095,4095,4095,4095,4095,'!'};

uint8 prox_flag = 0xFF;
inline void prox_reader(int index,uint data);
void prox0_reader(uint data);
void prox1_reader(uint data);
void prox2_reader(uint data);
void prox3_reader(uint data);
void prox4_reader(uint data);
void prox5_reader(uint data);
void prox6_reader(uint data);
void prox7_reader(uint data);

void clearLEDs();
void setLEDs();
void clearIRs();
void setIRs();

bool doNotSend = false;

int16_t main(void)
{    
    Sys_Init_Kernel(); 
     
    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
    BODY_LED = 0;
    FRONT_LED = 0;
    
    
    Sys_SetReadingFunction_UART1(bluetooth_reader);
    
    Sys_Subscribe_ADCChannelProcessor(Prx0, prox0_reader);
    Sys_Subscribe_ADCChannelProcessor(Prx1, prox1_reader);
    Sys_Subscribe_ADCChannelProcessor(Prx2, prox2_reader);
    Sys_Subscribe_ADCChannelProcessor(Prx3, prox3_reader);
    Sys_Subscribe_ADCChannelProcessor(Prx4, prox4_reader);
    Sys_Subscribe_ADCChannelProcessor(Prx5, prox5_reader);
    Sys_Subscribe_ADCChannelProcessor(Prx6, prox6_reader);
    Sys_Subscribe_ADCChannelProcessor(Prx7, prox7_reader); 
    
    Sys_Start_Kernel();
    
    uint32 time = Sys_Get_SystemClock();
    time += (uint32) 1000;
 
//    uint counter = 0;
    
    while(true){
       
        
        uint32 time_now = Sys_Get_SystemClock();
        if(time_now >= time){//wait the refresh_rate time
            //random_change++;
        doNotSend = false;    
        switch(Sys_Get_Selector()){
            case 0:
                Sys_Set_LeftWheelSpeed(0);
                Sys_Set_RightWheelSpeed(0);
                clearLEDs();
                clearIRs();
                doNotSend = true;
                break;
            case 1:
                Sys_Set_LeftWheelSpeed(0);
                Sys_Set_RightWheelSpeed(0);
                clearLEDs();
                clearIRs();
                break;
            case 2:
                Sys_Set_LeftWheelSpeed(MAX_WHEEL_SPEED_MM_S);
                Sys_Set_RightWheelSpeed(-MAX_WHEEL_SPEED_MM_S);
                clearLEDs();
                clearIRs();
                break;
            case 3:
                Sys_Set_LeftWheelSpeed(-MAX_WHEEL_SPEED_MM_S);
                Sys_Set_RightWheelSpeed(MAX_WHEEL_SPEED_MM_S);
                clearLEDs();
                clearIRs();
                break;
            case 4:
                Sys_Set_LeftWheelSpeed(MAX_WHEEL_SPEED_MM_S);
                Sys_Set_RightWheelSpeed(-MAX_WHEEL_SPEED_MM_S);
                setLEDs();
                clearIRs();
                break;
            case 5:
            case 6:
                Sys_Set_LeftWheelSpeed(0);
                Sys_Set_RightWheelSpeed(0);
                setLEDs();
                clearIRs();
                break;
            case 7:
            case 8:
                Sys_Set_LeftWheelSpeed(0);
                Sys_Set_RightWheelSpeed(0);
                clearLEDs();
                setIRs();
                break;
            case 10:
                Sys_Set_LeftWheelSpeed(MAX_WHEEL_SPEED_MM_S);
                Sys_Set_RightWheelSpeed(-MAX_WHEEL_SPEED_MM_S);
                clearLEDs();
                setIRs();
                break;
            case 11:
                Sys_Set_LeftWheelSpeed(-MAX_WHEEL_SPEED_MM_S);
                Sys_Set_RightWheelSpeed(MAX_WHEEL_SPEED_MM_S);
                clearLEDs();
                setIRs();
                break;
            case 12:
            case 13:
                Sys_Set_LeftWheelSpeed(MAX_WHEEL_SPEED_MM_S);
                Sys_Set_RightWheelSpeed(-MAX_WHEEL_SPEED_MM_S);
                setLEDs();
                setIRs();
                break;
            case 14:
            case 15:
                Sys_Set_LeftWheelSpeed(0);
                Sys_Set_RightWheelSpeed(0);
                setLEDs();
                setIRs();
                break;
            default:
                break;
        }
            
            time += (uint32) 100;
        }
    }
}

void clearLEDs(){
    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
    BODY_LED = 0;
    FRONT_LED = 0;
}

void setLEDs(){
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1;
    BODY_LED = 1;
    FRONT_LED = 1;
}

void clearIRs(){  
    PULSE_IR0 = 0;
    PULSE_IR1 = 0;
    PULSE_IR2 = 0;
    PULSE_IR3 = 0;
}
void setIRs(){  
    PULSE_IR0 = 1;
    PULSE_IR1 = 1;
    PULSE_IR2 = 1;
    PULSE_IR3 = 1;
}

//uint prox_median[8] = {4095,4095,4095,4095,4095,4095,4095,4095};
uint prox_values[8] = {0};
inline void prox_reader(int index,uint data){
    prox_values[index] = data;
    
    prox_flag = prox_flag & ~(1 << index);
    uint max = 0xFFFF;
    
    if(doNotSend){
        return;
    }
    
    if(prox_flag == 0){//All readings collected
        
        unsigned char msg[22];   
        
        uint xor_v = 0;
        char *p_xor_v = (char*) &xor_v;
        
        msg[0] = 'P';
        int i = 0;
        for(i=0; i < 16; i++){
            if(i < 8){
                xor_v ^= prox_values[i];
            }
            
            char* values = (char *) prox_values;
            
            msg[i+1] = values[i];
        }
        
        msg[17] =  p_xor_v[0];
        msg[18] =  p_xor_v[1];
                
        msg[19] = '<';
        msg[20] = ':';
        msg[21] = '>';
        
        Sys_Writeto_UART1(msg, 22);// 2*2*8+3*2 elements of 2 bytes
        
        prox_flag = 0xFF;     
    }
    
}

void bluetooth_reader(uint8 data){
    ;
}

void prox0_reader(uint data){
    prox_reader(0, data);
}
void prox1_reader(uint data){
    prox_reader(1, data);
}

void prox2_reader(uint data){
    prox_reader(2, data);
}

void prox3_reader(uint data){
    prox_reader(3, data);
}

void prox4_reader(uint data){
    prox_reader(4, data);
}

void prox5_reader(uint data){
    prox_reader(5, data);
}

void prox6_reader(uint data){
    prox_reader(6, data);
}

void prox7_reader(uint data){
    prox_reader(7, data);
}
