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

void log_me();
void thread();

#define MAX_SPEED 128
#define REFRESH_RATE 1000 //ms
#define CHANGE_BEHAVIOUR 5000 //ms

int16_t main(void)
{    
    Sys_Init_Kernel(); 
    
    Sys_Subscribe_to_Event(SYS_EVENT_10ms_CLOCK, led_toggler, 0, 0);
    
    Sys_Start_Process(thread);
    
    Sys_Start_Kernel();
    
    uint32 time = Sys_Get_SystemClock();
    time += (uint32) 1000;
    
    while(true){
       
        uint32 time_now = Sys_Get_SystemClock();
        if(time_now >= time){//wait the refresh_rate time
            //random_change++;
            time += REFRESH_RATE;
            
            LED0 = ~LED0;
        }
    }
}

bool led_toggler(uint16 eventID, sys_event_data *data, void *udata){
    static uint32 counter = 0;
    
    uint32 time_now = *((uint32 *) data->value);
    if( counter < time_now){
        counter = time_now + 1000;
        LED7 = ~LED7;
    } 
    
    return true;
}

bool oneSecCondition(uint16 eventID, sys_event_data *data, void *user_data){
    static int counter = 0;
    
    if(++counter >= 100){
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
            time += REFRESH_RATE;
            
            LED3 = ~LED3;
        }
    }
}

