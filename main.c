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
#include "os/platform/e-puck/selector.h"

#include "os/communication/channel.h"
#include "os/communication/communication.h"
#include "os/communication/physical.h"

#include "extern/platform/e-puck/library/motor_led/e_epuck_ports.h"
#include "extern/platform/e-puck/library/motor_led/e_init_port.h"


int16_t main(void)
{    
    e_init_port(); //Set all pins and ports
    INTCON1bits.NSTDIS = 0;
    
    Sys_Init_SystemTimer(Sys_Scheduler_RoundRobin);//start the system timer + interrupt = HDI - hardware dependent implementaion
    Sys_Init_Process_Management();
//    Sys_Init_IOManagement();
//    Sys_Init_SystemClock();
//    Sys_Init_Motors();
//    Sys_Init_UART1();
//    Sys_Init_RemoteControl();
//    Sys_Init_Camera();
    Sys_Init_Selector();
    Sys_Init_ADC();
    Sys_Init_Proximity();
    Sys_Init_SystemClock();
    Sys_Init_Communication();  
   
    Sys_Start_SystemTimer();
//    Sys_Start_IOManagement();
//    Sys_Start_SystemClock();
//    Sys_Start_UART1();
//    Sys_Start_RemoteControl();
//    Sys_Start_Camera();
//    Sys_Start_ADC();
//    Sys_Start_SystemClock();
//    Sys_Start_Communication();
    
    FRONT_LED = 0;
    FRONT_LED_DIR = OUTPUT_PIN;
    
    unsigned int value = 0;
    
    while(true){
        if(value == 0x000F){
            FRONT_LED = ~FRONT_LED;
            value = 0;
            continue;
        }
        value = value + 1;
    }
}
