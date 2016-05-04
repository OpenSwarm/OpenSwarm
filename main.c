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

static uint fps = 0;

int sinVectorTimes(vector *, int);
int cosVectorTimes(vector *, int);
int sign(int);

void getProximityValues();
void calculateProxPointer();
void initProxPointer();
void calculateMotorSpeed();
bool prox_sensors(uint16 PID, uint16 eventID, sys_event_data *data);

void log_me();
void thread();

#define MAX_SPEED 128
#define REFRESH_RATE 100 //ms
#define CHANGE_BEHAVIOUR 5000 //ms

int16_t main(void)
{
    Sys_Init_Kernel(); 
    
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_0, 0, prox_sensors, 0);
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_1, 0, prox_sensors, 0);
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_2, 0, prox_sensors, 0);
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_3, 0, prox_sensors, 0);
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_4, 0, prox_sensors, 0);
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_5, 0, prox_sensors, 0);
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_6, 0, prox_sensors, 0);
    Sys_Subscribe_to_Event(SYS_EVENT_IO_PROX_7, 0, prox_sensors, 0);
    
    Sys_Start_Process(thread);
    
    Sys_Start_Kernel();

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
    
    initProxPointer();
   
    uint32 time = Sys_Get_SystemClock();
    time += (uint32) 1000;
    
    default_speed.left = MAX_SPEED;
    default_speed.right = MAX_SPEED;
    uint random_change = 0;
    
    while(true){
        
        if(SR & 0x00E0){
            SRbits.IPL = 0;
        }
            
        if(CORCON & 0x08){
            //LED6 = 1;
            CORCONbits.IPL3 = 0;
        }
        
        uint32 time_now = Sys_Get_SystemClock();
        if(time_now >= time){//wait the refresh_rate time
            random_change++;
            time += REFRESH_RATE;
            
            if(random_change >= CHANGE_BEHAVIOUR/REFRESH_RATE){//change the direction after the Change_behaviour time
                default_speed.left = MAX_SPEED/2 + (MAX_SPEED * (Sys_Rand8() % 50)) / 100;
                default_speed.right = MAX_SPEED/2 + (MAX_SPEED * (Sys_Rand8() % 50)) / 100;
                random_change = 0;
            }            
            LED0 = ~LED0;
        }
    }
}

void thread(){
    uint32 time = Sys_Get_SystemClock();
    while(true){
        uint32 time_now = Sys_Get_SystemClock();
        if(time_now >= time){//wait the refresh_rate time
            time += REFRESH_RATE;
            
            LED1 = ~LED1;
        }
    }
}

void log_me(){
    static char message[32];
    
    uint length = 0;
    length = sprintf(message, "%03d;%03d;%03d,%03d,%03d\n\r", Sys_Rand8(),Sys_Rand8(),Sys_Get_Prox(3),Sys_Get_Prox(5),Sys_Get_Prox(7));
    
    Sys_Writeto_UART1(message, length);//send via Bluetooth
 
    Sys_Reset_InterruptCounter();
    Sys_Reset_EventCounter();
    fps = 0;
}

void Sys_ClearLEDs(){
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

/**
 * 
 * Loads all the proximity sensor values into an buffer and transforms the distance to weightings.
 * 100-poxValue
 * 
 */
void getProximityValues(){
    int i;
    int new_value;
    for(i = 0; i < PROXIMITIES; i++){
        if(Sys_Get_Prox(i) > 100){
            new_value = 0;
        }else{//was something measured?
            new_value = 100-Sys_Get_Prox(i);
        }
        
        proximity_values[i] = new_value;///4 + proximity_values[i]/4 + proximity_values[i]/2;
    }
}

/**
 * 
 * This function composes the proximity vector.
 * 
 */
void calculateProxPointer(){
    int prox_x = 0;
    int prox_y = 0;
    int i;
    
    for(i = 0; i < PROXIMITIES; i++){
        if(proximity_values[i] < 0xFFFF){//add all proximity sensors that measured an object
            prox_x += ((long) proximity_values[i] * (long) prox_transform_x[i])/100;//calculate the X component
            prox_y += ((long) proximity_values[i] * (long) prox_transform_y[i])/100;//calculate the Y component
        }
    }
    
    proximity_pointer.x = prox_x;
    proximity_pointer.y = prox_y;
    proximity_pointer.length = sqrt((long) prox_x*prox_x + (long) prox_y*prox_y);
}

/**
 * 
 * This function calculates the motor speeds from the composed proximity vector. The selector defines which algorithm is applied.
 * 
 * @param[out] motor_speeds* the struct to put in the motor speeds. 
 */
void calculateMotorSpeed(){
    sint sinMax = sinVectorTimes(&proximity_pointer, MAX_SPEED);
    sint cosMax = cosVectorTimes(&proximity_pointer, MAX_SPEED);
  
    //calculates the forward speed + rotation
    if(proximity_pointer.x > 0){//is the target in front 
        if(proximity_pointer.x >= 200){//if too close -> only turn
            robot_speed.left  =  -sinMax;
            robot_speed.right =   sinMax;
        }else{//get slower when getting closer
            robot_speed.left  = (-cosMax*(200-proximity_pointer.x))/200 - sinMax;
            robot_speed.right = (-cosMax*(200-proximity_pointer.x))/200 + sinMax;
        }
    }else{//go always max speed because the object is behind you
       robot_speed.left  = -cosMax - sinMax;
       robot_speed.right = -cosMax + sinMax;
    }
    
    switch(Sys_Get_Selector()){
        case 0://stop doing anything
            robot_speed.left = 0;
            robot_speed.right = 0;
            return;
        case 0x01://object avoidance
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
            if(proximity_pointer.length < 25){//if you don't see anything -> do random behaviour
                robot_speed.left  =  default_speed.left;
                robot_speed.right = default_speed.right;
            }
            return;
        case 0x08://chasing
        case 0x09:
        case 0x0A:
        case 0x0B:
        case 0x0C:
        case 0x0D:
        case 0x0E:
        case 0x0F:
            if(proximity_pointer.length < 25){//if you don't see anything -> stop
                robot_speed.left =  0;
                robot_speed.right = 0;
            }else{
                robot_speed.left =  -robot_speed.left;
                robot_speed.right = -robot_speed.right;
            }           
            return;
    }
}

/**
 * 
 * multiplies the sine of the vector v angle with X. This function has a 1% accuracy.
 * 
 * @param[in] vector the proximity vector
 * @param[in] int    value that needs to be multiplied
 */
int sinVectorTimes(vector *v, int x){
    if(v->length == 0){
        return 0;
    }
    
    return (((v->y * 100)/v->length) * x)/100;
}

/**
 * 
 * multiplies the cosine of the vector v angle with X. This function has a 1% accuracy.
 * 
 * @param[in] vector the proximity vector
 * @param[in] int    value that needs to be multiplied
 */
int cosVectorTimes(vector *v, int x){
    if(v->length == 0){
        return 0;
    }
    
    return (((v->x * 100)/v->length) * x)/100;
}

/**
 * 
 * This function initialises the vector that is composed of all the proximity values with 0
 * 
 */
void initProxPointer(){
    proximity_pointer.x = 0;
    proximity_pointer.y = 0;
    proximity_pointer.length = 0;
    
}

/**
 * This is a nice alternative to the other toggle_frontLED function
 * This function turns on the front_LED, if the selector was set to an even number
 *
 * @para[in] PID        identifier of the process to which this function was subscribed
 * @para[in] eventID    identifier of the event to which this function was subscribed
 * @para[in] data       the data which was sent with the event
 * @return   bool       was this function successful?
 */
bool prox_sensors(uint16 PID, uint16 eventID, sys_event_data *data){
    
    static uint8 sensor_flags = 0;//each bit is for a new sensor value
        
    switch(eventID){
        case SYS_EVENT_IO_PROX_0:
            proximity_values[0] = 100-Sys_Get_Prox(0);
            sensor_flags |= 0x01;
            break;
        case SYS_EVENT_IO_PROX_1:
            proximity_values[1] = 100-Sys_Get_Prox(1);
            sensor_flags |= 0x02;
            break;
        case SYS_EVENT_IO_PROX_2:
            proximity_values[2] = 100-Sys_Get_Prox(2);
            sensor_flags |= 0x04;
            break;
        case SYS_EVENT_IO_PROX_3:
            proximity_values[3] = 100-Sys_Get_Prox(3);
            sensor_flags |= 0x08;
            break;
        case SYS_EVENT_IO_PROX_4:
            proximity_values[4] = 100-Sys_Get_Prox(4);
            sensor_flags |= 0x10;
            break;
        case SYS_EVENT_IO_PROX_5:
            proximity_values[5] = 100-Sys_Get_Prox(5);
            sensor_flags |= 0x20;
            break;
        case SYS_EVENT_IO_PROX_6:
            proximity_values[6] = 100-Sys_Get_Prox(6);
            sensor_flags |= 0x40;
            break;
        case SYS_EVENT_IO_PROX_7:
            proximity_values[7] = 100-Sys_Get_Prox(7);
            sensor_flags |= 0x80;
            break;
    }
    
    if(sensor_flags == 0xFF){//is it even=true or odd=false
        calculateProxPointer();//calculate the overall vector
        calculateMotorSpeed();//calculate the motor speed
        
        //apply motor speed
        Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_LEFT,  robot_speed.left);
        Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_RIGHT, robot_speed.right);
    }
    
    return true;
}