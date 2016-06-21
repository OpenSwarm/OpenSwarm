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
int proximity_values[PROXIMITIES];//buffers the proximity values

const int prox_transform_x[] = {96, 70,   0, -88, -88,    0,  70,  96};// divide by 100
const int prox_transform_y[] = {30, 72, 100,  48, -48, -100, -72, -30};// divide by 100

vector proximity_pointer;//compost vector containing all the x & y components of the sensor values
motor_speeds default_speed;//default behaviour when nothing is seen
motor_speeds robot_speed;//actual robot speed that should be applied to the motors

#define proxThres 70
#define proxCountThres 5
#define rotMax 25

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

void log_me();
void ledsOn();
void ledsOff();

bool toggle_frontLED(uint16 PID, uint16 eventID, sys_event_data *data);

#define MAX_SPEED 128
#define CONTROL_STEP_TIME 25 //ms
#define INIT_TIME 1000
#define EPS 1
#define TAU 2000
//1842
#define LED_THRES 100
#define REF_THRES 400

int16_t main(void)
{
    Sys_Init_Kernel();     
    Sys_Start_Kernel();
    
    Sys_Set_LeftWheelSpeed(128);
    Sys_Set_RightWheelSpeed(128);
    while(true){
        ;
    }
    
    uint16 phase = 0;
    uint32 phaseStart = 0;
    char message[32];
    
    uint length = 0;
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
   // Sys_Get_Selector(void);
    initProxPointer();
   
    uint32 time = Sys_Get_SystemClock();
    calculateMotorSpeed(&robot_speed);//calculate the motor speed
    time += (uint32) INIT_TIME;
    
    while(true){
            
        uint32 time_now = Sys_Get_SystemClock();
        if(time_now >= time){//wait the refresh_rate time
            time += CONTROL_STEP_TIME;
            
            getProximityValues();//get the values
            calculateProxPointer();//calculate the overall vector
            calculateMotorSpeed(&robot_speed);//calculate the motor speed

            phase = time_now - phaseStart;
            
            if (phase > LED_THRES) {
                ledsOff();
            }
           if (phase > REF_THRES) {
                if (seeFlash()) {
                    phase += (phase*EPS)/10;
                    BODY_LED = 1;
//                    BODY_LED = ~BODY_LED;
                }
                else BODY_LED = 0;
            }

            if (phase >= TAU) {
                ledsOn();
                phase = 0;
                phaseStart = time_now;
                BODY_LED = 0;
            }
            

            Sys_Set_LeftWheelSpeed(robot_speed.left);
            Sys_Set_RightWheelSpeed(robot_speed.right);
            
        }
    }
}

void ledsOff(){
    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
}

void ledsOn(){
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1;
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
    int i,j;
    
    for(i = 5; i < 11; i++){
        j = i % 8;
        if(proximity_values[j] < 0xFFFF){//add all proximity sensors that measured an object
            prox_x += ((long) proximity_values[j] * (long) prox_transform_x[j])/100;//calculate the X component
            prox_y += ((long) proximity_values[j] * (long) prox_transform_y[j])/100;//calculate the Y component
        }
    }
    
    proximity_pointer.x = prox_x;
    proximity_pointer.y = prox_y;
}

/**
 * 
 * This function calculates the motor speeds from the composed proximity vector. The selector defines which algorithm is applied.
 * 
 * @param[out] motor_speeds* the struct to put in the motor speeds. 
 */
void calculateMotorSpeed(motor_speeds *speeds){
    static int rotTime = 0;
    static int proxCount = 0;
    int vel = 0;
    int max = MAX_SPEED;
    int rotDir;
     
    switch(Sys_Get_Selector()){
        case 0://stop doing anything
            vel = 0;
            max = 0;
            break;
        case 0x01:
            vel = 5;
            LED0 = 1;
            break;
        case 0x02:
            vel = 7;
            LED1 = 1;
            break;
        case 0x03:
            vel = 10;
            LED0 = 1;
            LED1 = 1;
            break;
        case 0x04:
            vel = 14;
            LED2 = 1;
            break;
        case 0x05:
            vel = 21;
            LED2 = 1;
            LED0 = 1;
            break;
        case 0x06:
            vel = 30;
            LED2 = 1;
            LED1 = 1;
            break;
        case 0x07:
            vel = 43;
            LED2 = 1;
            LED1 = 1;
            LED0 = 1;
            break;
        case 0x08:
            vel = 62;
            LED3 = 1;
            break;
        case 0x09:
            vel = 89;
            LED3 = 1;
            LED0 = 1;
            break;
        case 0x0A:
            vel = 128;
            LED3 = 1;
            LED1 = 1;
            break;
        case 0x0B:
        case 0x0C:
        case 0x0D:
        case 0x0E:
        case 0x0F:
            break;
    }
    
    if(rotTime > 0 || proxCount > 0){
         if (proximity_pointer.x*proximity_pointer.x > proxThres*proxThres || proximity_pointer.y*proximity_pointer.y > proxThres*proxThres) proxCount++;
         else proxCount = 0;
    }

    // If contact with obstacles for several consecutive cycles do random reorient (it's probably a wall)
    if(proxCount > proxCountThres){
        if(proximity_pointer.y > 0) rotDir = 1;
        else rotDir = -1;
        speeds->left = -max*rotDir;
        speeds->right = max*rotDir;
        rotTime += 1 +rand() % rotMax;
        proxCount = 0;
     }

    
     // If obstacle ahead turn to avoid it
     if(proximity_pointer.x > proxThres){          
        if(proximity_pointer.y > 0) rotDir = 1;
        else rotDir = -1;
        speeds->left = -max*rotDir;
        speeds->right = max*rotDir;
        rotTime = 1;
     }
     // Continue straight if no obstacle
     else
         if(rotTime ==0) {
         speeds->right = speeds->left = vel;
     }
     else rotTime -= 1;   
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
}

bool toggle_frontLED(uint16 PID, uint16 eventID, sys_event_data *data){ 
    FRONT_LED = ~FRONT_LED; 
    return true; 
}
