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

#include "os/io/e-puck/proximity.h"

/******************************************************************************/
/* Variable Declaration                                                */
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
int proximity_values[PROXIMITIES];

const int prox_transform_x[] = {96, 70,   0, -88, -88,    0,  70,  96};// divide by 100
const int prox_transform_y[] = {30, 72, 100,  48, -48, -100, -72, -30};// divide by 100

vector proximity_pointer;
    motor_speeds motors;

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
void calculateMotorSpeed(motor_speeds *);

int16_t main(void)
{
    Sys_Init_Kernel();
    
   // Sys_Subscribe_to_Event(SYS_EVENT_IO_CAMERA, 0, object_clustering, 0);   
    
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
    
//     if( // !Sys_Start_Process(thread1) ||
//         //   !Sys_Start_Process(thread2) ||
//         //   !Sys_Start_Process(thread3) ||
//         //   !Sys_Start_Process(thread4) ||
//         //   !Sys_Start_Process(thread5) ||
//         //   !Sys_Start_Process(thread6) ||
//            !Sys_Start_Process(thread7)
//      ){
//        FRONT_LED = 1;
//    } 
   
    uint32 time = Sys_Get_SystemClock();
    time += (uint32) 1000;
    
    while(true){
        
        if(SR & 0x00E0){
            SRbits.IPL = 0;
        }
            
        if(CORCON & 0x08){
            //LED6 = 1;
            CORCONbits.IPL3 = 0;
        }else{
            //LED6 = 0;
        }
        
        if(Sys_Get_IRQNestingLevel() != 0){
            //LED2 = 1;
        }
        
        uint32 time_now = Sys_Get_SystemClock();
        if(time_now >= time){
            time += 100;
            getProximityValues();
            calculateProxPointer();
            calculateMotorSpeed(&motors);
            log_me();
            Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_LEFT,  motors.left);
            Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_RIGHT, motors.right);
            LED0 = ~LED0;
        }
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

*/
void bluetooth_reader(uint8 data){    
   // Sys_Writeto_UART1(&data,1);
    ;
}

void log_me(){
    static char message[32];
    
    uint length = 0;
    length = sprintf(message, "(%05d;%05d;%05ld) > (%03d,%03d)\r\n", proximity_pointer.x, proximity_pointer.y,proximity_pointer.length, motors.left, motors.right);
    
    Sys_Writeto_UART1(message, length);//send via Bluetooth
 
    Sys_Reset_InterruptCounter();
    Sys_Reset_EventCounter();
    fps = 0;
}

void thread7(){
    uint i = 0;    
    while(true){//DO Nothing (do only things for testing)
        if(i == 0xFFFE){
            i = 0;
            //LED7 = ~LED7; 
        }
        i++;
    } 
}

bool toggleLED(uint16 PID, uint16 EventID, sys_event_data *data){
    LED6 = ~LED6;
    return true;
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

bool selector2LEDs(uint16 PID, uint16 EventID, sys_event_data *data){
    
    uint8 value = *((uint8*) data->value);
    
    Sys_ClearLEDs();
    //Sys_SetLED(value & 0x07);
    
    return true;
}

bool prox2motors(uint16 PID, uint16 EventID, sys_event_data *data){
    
    uint16 value = *((uint16*) data->value);
    
    value = MAX_WHEEL_SPEED_MM_S/(value/10+1);
    
    Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_LEFT,  value);
    Sys_Send_IntEvent(SYS_EVENT_IO_MOTOR_RIGHT,-value);
    
    return true;
}


void getProximityValues(){
    int i;
    for(i = 0; i < PROXIMITIES; i++){
        proximity_values[i] = Sys_Get_Prox(i);
    }
}

void calculateProxPointer(){
    int prox_x = 0;
    int prox_y = 0;
    int i;
    
    for(i = 0; i < PROXIMITIES; i++){
        if(proximity_values[i] < 0xFFFF){
            prox_x += ((100 - proximity_values[i]) * prox_transform_x[i])/100;
            prox_y += ((100 - proximity_values[i]) * prox_transform_y[i])/100;
        }
    }
    
    proximity_pointer.x = prox_x;
    proximity_pointer.y = prox_y;
    proximity_pointer.length = sqrt((long) prox_x*prox_x + (long) prox_y*prox_y);
}

#define MAX_SPEED 256

void calculateMotorSpeed(motor_speeds *speeds){
    sint sinMax = sinVectorTimes(&proximity_pointer, MAX_SPEED);
    sint cosMax = cosVectorTimes(&proximity_pointer, MAX_SPEED);
    
    if(proximity_pointer.x > 0){//is the target in front 
        if(proximity_pointer.x >= 200){//if too close -> only turn
            speeds->left  =  -sinMax;
            speeds->right =  sinMax;
        }else{//get slower when getting closer
            speeds->left  = (cosMax*(200-proximity_pointer.x))/200 - sinMax;
            speeds->right = (cosMax*(200-proximity_pointer.x))/200 + sinMax;
        }
    }else{//go always max speed because the object is behind you
       speeds->left  = cosMax - sinMax;
       speeds->right = cosMax + sinMax;
    } 
}

int sinVectorTimes(vector *v, int x){
    if(v->length == 0){
        return 0;
    }
    
    return (((v->y * 100)/v->length) * x)/100;
}

int cosVectorTimes(vector *v, int x){
    if(v->length == 0){
        return 0;
    }
    
    return (((v->x * 100)/v->length) * x)/100;
}
void initProxPointer(){
    proximity_pointer.x = 0;
    proximity_pointer.y = 0;
    proximity_pointer.length = 0;
    
}