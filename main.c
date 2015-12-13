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

#include "os/definitions.h"


#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdlib.h>
#include <stdbool.h>       /* Includes true/false definition                  */
#include <stdio.h>

#include "os/system.h"        /* System funct/params, like osc/peripheral config */

/*
#include "system_Events.h"

#include "system_IO_motors.h"
#include "system_IO_uart.h"
#include "system_IO_remoteControl.h"
#include "system_IO_clock.h"
*/
/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

//static bool run_clustering = false;

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

/*
void bluetooth_reader(uint8 data);
bool remotecontrol_reader(uint16 pid, uint16 eventID, sys_event_data *data);

void loggingThread();
bool wait250times(void *data);
bool object_clustering(uint16 pid, uint16 eventID, sys_event_data *data);
*/

int16_t main(void)
{

    //Sys_Init_MCU_HDI();
    /* Configure the oscillator for the device */
    Sys_Init_Kernel();
    
    //Sys_SetReadingFunction_UART1(bluetooth_reader);
    //Sys_Start_Process(loggingThread);
    //Sys_Subscribe_to_Event(SYS_EVENT_IO_REMOECONTROL, 0, remotecontrol_reader, 0);
    //Sys_Subscribe_to_Event(SYS_EVENT_IO_CAMERA, 0, object_clustering, 0);
    
    Sys_Start_Kernel();

    /*
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
    */
    //sys_event_data * data = Sys_Wait_For_Event(SYS_EVENT_TERMINATION);
    //Sys_Clear_EventData(&data);
    
    unsigned int i = 0;
    
    while(1){//DO Nothing (do yonly things for testing)
        if(i == 0xFFFE){
            i = 0;
            //LED7 = ~LED7; 
        }
        i++;
    }
}

/*
bool remotecontrol_reader(uint16 pid, uint16 eventID, sys_event_data *data){
    char msg[24] = {0};
    uint8 length = 0;
    uint8 *value = data->value;
    length = sprintf(msg, "rc:%u\r\n", (int) value[0] );
    Sys_Writeto_UART1(msg, length);

    
    run_clustering = false;
    sint16 speed = 0;
    
    switch(*value){
        case RC_BUTTON_SWAP:
            srand(Sys_Get_SystemClock() & 0xFF);
            speed = (rand() % (2*MAX_WHEEL_SPEED_MM_S))-MAX_WHEEL_SPEED_MM_S;//speed \in [-MAX_WHEEL_SPEED_MM_S, MAX_WHEEL_SPEED_MM_S]
            Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_LEFT, speed);
            Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_RIGHT, -speed);
            BODY_LED = 1;
            FRONT_LED = 0;
            break;
        case RC_BUTTON_OK:
            speed = 0;
            BODY_LED = 0;
            FRONT_LED = 0;
            Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_LEFT, 0);
            Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_RIGHT, 0);
            run_clustering = true;
            break;
        case RC_BUTTON_STANDBY:
        case RC_BUTTON_SLEEP:
            speed = 0;
            BODY_LED = 0;
            FRONT_LED = 0;
            Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_LEFT, 0);
            Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_RIGHT, 0);
        default:
            break;
    }
    return true;
}

void bluetooth_reader(uint8 data){    
   // Sys_Writeto_UART1(&data,1);
    ;
}

#define ROBOT_SPEED_L   (MAX_WHEEL_SPEED_MM_S * 89)/100
#define ROBOT_SPEED_R   (MAX_WHEEL_SPEED_MM_S * 53)/100

#define OBJECT_SPEED_L  (MAX_WHEEL_SPEED_MM_S * 98)/100
#define OBJECT_SPEED_R  (MAX_WHEEL_SPEED_MM_S * 50)/100

#define NOTHING_SPEED_L (MAX_WHEEL_SPEED_MM_S * 55)/100
#define NOTHING_SPEED_R (MAX_WHEEL_SPEED_MM_S * 99)/100
bool object_clustering(uint16 PID, uint16 EventID, sys_event_data *data){
    
    LED4 = ~LED4;
    
    if(!run_clustering){
        return true;
    }
    
    sys_colour rx_colour = *((sys_colour *)data->value);
   
    static char message[24];
        uint16 length = 0;
        length = sprintf(message, "%i (%i,%i,%i)\r\n", rx_colour, NOTHING_SPEED_L, NOTHING_SPEED_R, WHITE);
        Sys_Writeto_UART1(message, length);//send via Bluetooth
    
    switch(rx_colour){//detection of
    case GREEN://other robot
        Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_LEFT, ROBOT_SPEED_L);
        Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_RIGHT,ROBOT_SPEED_R);
        LED5 = ~LED5;
        break;
    case RED://object
        Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_LEFT, OBJECT_SPEED_L);
        Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_RIGHT, OBJECT_SPEED_R);
        LED6 = ~LED6;
        break;
    case WHITE://nothing/wall
        Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_LEFT, NOTHING_SPEED_L);
        Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_RIGHT, NOTHING_SPEED_R);
        LED7 = ~LED7;
        break;
    default://anything else
        LED1 = ~LED1;
        break;//keep on
    }
    
    return true;
}

void loggingThread(){
    static char message[24];
    while(true){
        uint16 length = 0;
        length = sprintf(message, "\%u:(\%3u,\%3u)\r\n", Sys_Get_SystemTime(), Sys_Get_LeftWheelSpeed(), Sys_Get_RightWheelSpeed());
        Sys_Writeto_UART1(message, length);//send via Bluetooth
    
        sys_event_data *data = Sys_Wait_For_Condition(SYS_EVENT_1ms_CLOCK, &wait250times);
        Sys_Clear_EventData(&data);
    } 
}

bool wait250times(void *data){
    static uint8 counter = 0;
    if(counter++ < 250){
        return false;
    }
    counter = 0;
    return true;
}
 * */