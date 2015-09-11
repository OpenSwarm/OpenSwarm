/*
 * File:   main.c
 * Author: Stefan trenkwalder
 *
 * Created on 07 July 2014, 14:15
 *
 * LICENCE: adapted FreeBSD Licence
 * Copyright (c) 2015, Stefan Trenkwalder
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. If this or parts of this source code or binary code in any form used for an commercial product or service in any form, this product or service must provide a clear notice/message readable for any user or customer that this product or service uses OpenSwarm.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif


#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <stdio.h>

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "definitions.h"

#include "user.h"          /* User funct/params, such as InitApp              */

#include "HDI_epuck_ports.h"
#include "HDI_init_port.h"

#include "system_Events.h"

#include "system_IO_motors.h"
#include "system_IO_uart.h"
#include "system_IO_remoteControl.h"

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void task1();
void task2();
void task3();

void frontLED();

void bluetooth_reader(uint8 data);
bool remotecontrol_reader(uint16 pid, uint16 eventID, sys_event_data *data);

int16_t main(void)
{

    //Sys_Init_MCU_HDI();
    /* Configure the oscillator for the device */
    Sys_Init_Kernel();
    
    //Sys_SetReadingFunction_UART1(bluetooth_reader);
    //Sys_Start_Process_HDI(task1);
    Sys_Subscribe_to_Event(SYS_EVENT_IO_REMOECONTROL, 0, remotecontrol_reader, 0);
    
    Sys_Start_Kernel();

    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;

    //sys_event_data * data = Sys_Wait_For_Event(SYS_EVENT_TERMINATION);
    //Sys_Clear_EventData(&data);
    
    unsigned int i = 0;
    
    while(1){//DO Nothing (do yonly things for testing)
        if(i == 0xFFFE){
            i = 0;
            LED7 = ~LED7; 
        }
        i++;
    }
}

bool remotecontrol_reader(uint16 pid, uint16 eventID, sys_event_data *data){
    char msg[24] = {0};
    uint8 length = 0;
    uint8 *value = data->value;
    length = sprintf(msg, "e:%u\r\n", (int) value[0] );
    Sys_Writeto_UART1(msg, length);

    
static bool toggle = true;

    sint16 speed = 0;

    if(toggle){
        speed = 50;
        Sys_Set_LeftWheelSpeed(speed);
        speed = -50;
        Sys_Set_RightWheelSpeed(speed);
    }else{
        speed = 0;
        Sys_Send_Event(SYS_EVENT_IO_MOTOR_LEFT, &speed, 2);
        Sys_Send_Event(SYS_EVENT_IO_MOTOR_RIGHT, &speed, 2);
    }

    toggle = !toggle;
    return true;
}

void bluetooth_reader(uint8 data){
    BODY_LED = ~BODY_LED;
    
   // Sys_Writeto_UART1(&data,1);
}