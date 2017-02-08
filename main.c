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


uint8 prox_readings[36] = {'@', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                          0x00,  ':', 0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 
                          0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 
                          0x0F, 0xFF, '\r', '\n'};
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

int16_t main(void)
{    
    Sys_Init_Kernel(); 
     
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
            time += (uint32) 1000;
        }
    }
}


uint prox_median[8] = {4095,4095,4095,4095,4095,4095,4095,4095};
uint prox_values[8] = {0};
inline void prox_reader(int index,uint data){
    prox_values[index] = data;
    
    prox_flag = prox_flag & ~(1 << index);
    
    if(prox_median[index] < data){
        prox_median[index] = prox_median[index]-1;
    }else{
        prox_median[index] = prox_median[index]+1;
    }
    
    if(prox_flag == 0){//All readings collected
        prox_flag = 0xFF;        
        
        Sys_Writeto_UART1("@", 1);// 2*2*8+3*2 elements of 2 bytes
        Sys_Writeto_UART1(prox_values, 16);// 2*2*8+3*2 elements of 2 bytes
        Sys_Writeto_UART1(":", 1);// 2*2*8+3*2 elements of 2 bytes
        Sys_Writeto_UART1(prox_median, 16);// 2*2*8+3*2 elements of 2 bytes
        Sys_Writeto_UART1("\r\n",2);// 2*2*8+3*2 elements of 2 bytes
    }
    
}

void bluetooth_reader(uint8 data){
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
