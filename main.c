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

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

typedef struct vector_s{
    sint x;
    sint y;
    signed long length;
} proximity_vector, prox_vec, vector; 

typedef struct motor_s{
    sint left;
    sint right;
} motor_speeds; 
//static bool run_clustering = false;

#define PROXIMITIES 8
int proximity_values[PROXIMITIES];//buffers the proximity values

const int prox_transform_x[] = {96, 70,   0, -88, -88,    0,  70,  96};// divide by 100
const int prox_transform_y[] = {30, 72, 100,  48, -48, -100, -72, -30};// divide by 100

vector proximity_pointer;//compost vector containing all the x & y components of the sensor values
motor_speeds default_speed;//default behaviour when nothing is seen
motor_speeds robot_speed;//actual robot speed that should be applied to the motors

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int sinVectorTimes(vector *, int);
int cosVectorTimes(vector *, int);
int sign(int);

void getProximityValues();
void calculateProxPointer();
void initProxPointer();
void calculateMotorSpeed();
bool prox_sensors(uint16 eventID, sys_event_data *data, void *user_data);
bool led_toggler(uint16 eventID, sys_event_data *data, void *user_data);
bool oneSecCondition(uint16 eventID, sys_event_data *data, void *user_data);


bool remote_controlHandler(uint16 eventID, sys_event_data *data, void *user_data);
bool selectorHandler(uint16 eventID, sys_event_data *data, void *udata);
bool proximityHandler(uint16 eventID, sys_event_data *data, void *udata);

void log_me();
void thread();
void bluetooth_reader(uint8 data);

#define MAX_SPEED 128
#define REFRESH_RATE 500 //ms

int16_t main(void)
{    
    Sys_Init_Kernel(); 
     
    LED0 = 1;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
    
    Sys_Subscribe_to_Event(SYS_EVENT_10ms_CLOCK, led_toggler, oneSecCondition, 0);
    Sys_Subscribe_to_Event(SYS_EVENT_IO_REMOECONTROL, remote_controlHandler, 0, 0);
    Sys_Subscribe_to_Event(SYS_EVENT_IO_SELECTOR_CHANGE, selectorHandler, 0, 0);
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_0, proximityHandler, 0, 0 );
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_1, proximityHandler, 0, 0 );
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_2, proximityHandler, 0, 0 );
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_3, proximityHandler, 0, 0 );
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_4, proximityHandler, 0, 0 );
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_5, proximityHandler, 0, 0 );
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_6, proximityHandler, 0, 0 );
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_7, proximityHandler, 0, 0 );
    
    //Sys_Start_Process(thread);
    Sys_SetReadingFunction_UART1(bluetooth_reader);
//  void Sys_Writeto_UART1(void *data, uint length);
    
    Sys_Start_Kernel();
    
    uint32 time = Sys_Get_SystemClock();
    time += (uint32) 1000;
    
    char string[50] = {0};
    
    while(true){
       
        uint32 time_now = Sys_Get_SystemClock();
        if(time_now >= time){//wait the refresh_rate time
            //random_change++;
            LED4 = ~LED4;
            string[0] = 0;
            int len = sprintf(string, "%4u;%4u;%4u;%4u;%4u;%4u;%4u;%4u\r\n", \
                    Sys_Get_Raw(0),\
                    Sys_Get_Raw(1),\
                    Sys_Get_Raw(2),\
                    Sys_Get_Raw(3),\
                    Sys_Get_Raw(4),\
                    Sys_Get_Raw(5),\
                    Sys_Get_Raw(6),\
                    Sys_Get_Raw(7));
            
            Sys_Writeto_UART1(string, len);
            time += REFRESH_RATE;
        }
    }
}

void bluetooth_reader(uint8 data){
    Sys_Writeto_UART1(&data, 1);
}

bool led_toggler(uint16 eventID, sys_event_data *data, void *udata){
    //LED6 = ~LED6;
    
    return true;
}

bool remote_controlHandler(uint16 eventID, sys_event_data *data, void *udata){
    //LED4 = ~LED4;
    return true;
}


bool selectorHandler(uint16 eventID, sys_event_data *data, void *udata){
    //LED3 = ~LED3;
    return true;
}

uint isClose(uint value){
    if(value < 50){
        return 1;
    }
  
    return 0;
}

bool proximityHandler(uint16 eventID, sys_event_data *data, void *udata){
    
        
    return true;
}

bool oneSecCondition(uint16 eventID, sys_event_data *data, void *user_data){
    static int counter = 0;
    
    if(++counter >= 50){
        counter = 0;
        return true;
    }
    
    return false;
}

void thread(){
    uint32 time = Sys_Get_SystemClock();
    
    while(true){
    
        uint32 time_now = Sys_Get_SystemClock();
        if(time_now >= time){//wait the refresh_rate time
            time += REFRESH_RATE*2;
            
            //LED2 = ~LED2;
        }
    }
}

