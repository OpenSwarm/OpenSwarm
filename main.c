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

#include "os/system.h"        /* System funct/params, like osc/peripheral config */
#include "os/memory.h"
#include "os/interrupts.h"

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

//static bool run_clustering = false;

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void thread1();
void thread2();
void thread3();
void thread4();
void thread5();
void thread6();
void thread7();
void log_me();
bool toggle_frontLED(uint16 PID, uint16 EventID, sys_event_data *data);
bool toggle_frontLED2(uint16 PID, uint16 EventID, sys_event_data *data);

int16_t main(void)
{
    //turn all LEDs off
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
    
    Sys_Init_Kernel(); //initialise OpenSwarm
    
    if(   !Sys_Start_Process(thread1) ||
          !Sys_Start_Process(thread2) ||
          !Sys_Start_Process(thread3) ||
          !Sys_Start_Process(thread4) ||
          !Sys_Start_Process(thread5) ||
          !Sys_Start_Process(thread6) ||
          !Sys_Start_Process(thread7)
      ){//start 7 extra threads. If an error occurred -> turn on the FRONT_LED
        FRONT_LED = 1;
    }
       
    //execute toggle_frontLED every time when the selector was changed
    Sys_Subscribe_to_Event(SYS_EVENT_IO_SELECTOR_CHANGE, 0, toggle_frontLED, 0);
    
    Sys_Start_Kernel();//start OpenSwarm -> From now on all threads are running concurrently
      
    Sys_Writeto_UART1("OS Started\r\n", 12);//send via Bluetooth
    
    uint32 time = 1000;
    while(true){//DO Nothing (do only things for testing)
        
        uint32 time_now = Sys_Get_SystemClock();//get the current time
        if(time_now >= time){//if one second passed
            time += 1000;//calculate next second
            LED0 = ~LED0;//toggle LED0
        }
    }
}

/**
 * Here is an example how you can send data via Bluetooth to your PC
 * 
 */
void log_me(){
    static char message[30];//your string
    
    uint length = 0;//length of the string
    length = sprintf(message, "%07ld;%2u;%4u;%4u\r\n", Sys_Get_SystemTime(),// 
                                                    Sys_Get_Number_Processes(),// 
                                                    Sys_Get_InterruptCounter(),//
                                                    Sys_Get_EventCounter());//convert numbers into a string
    
    Sys_Writeto_UART1(message, length);//send via Bluetooth
    
    Sys_Reset_InterruptCounter();
    Sys_Reset_EventCounter();
}

/**
 * These functions represent 7 threads. Each thread toggles a different LED after the 65534th increment of i
 * 
 */
void thread1(){
    uint i = 0;    
    while(true){//DO Nothing (do only things for testing)
        if(i == 0xFFFE){
            i = 0;
            LED1 = ~LED1; 
        }
        i++;
    } 
}
void thread2(){
    uint i = 0;    
    while(true){//DO Nothing (do only things for testing)
        if(i == 0xFFFE){
            i = 0;
            LED2 = ~LED2; 
        }
        i++;
    } 
}
void thread3(){
    uint i = 0;    
    while(true){//DO Nothing (do only things for testing)
        if(i == 0xFFFE){
            i = 0;
            LED3 = ~LED3; 
        }
        i++;
    } 
}
void thread4(){
    uint i = 0;    
    while(true){//DO Nothing (do only things for testing)
        if(i == 0xFFFE){
            i = 0;
            LED4 = ~LED4; 
        }
        i++;
    } 
}
void thread5(){
    uint i = 0;    
    while(true){//DO Nothing (do only things for testing)
        if(i == 0xFFFE){
            i = 0;
            LED5 = ~LED5; 
        }
        i++;
    } 
}
void thread6(){
    uint i = 0;    
    while(true){//DO Nothing (do only things for testing)
        if(i == 0xFFFE){
            i = 0;
            LED6 = ~LED6; 
        }
        i++;
    } 
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
void thread7(){
    uint i = 0;    
    while(true){//DO Nothing (do only things for testing)
        if(i == 0xFFFE){
            i = 0;
            LED7 = ~LED7; 
        }
        i++;
    } 
}

/**
 *
 * This function toggles the front LED whenever an event occurred to which this function has been subscribed 
 *
 * @para[in] PID        identifier of the process to which this function was subscribed
 * @para[in] eventID    identifier of the event to which this function was subscribed
 * @para[in] data       the data which was sent with the event
 * @return   bool       was this function successful?
 */
bool toggle_frontLED(uint16 PID, uint16 eventID, sys_event_data *data){
    FRONT_LED = ~FRONT_LED;
    return true;
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
bool toggle_frontLED2(uint16 PID, uint16 eventID, sys_event_data *data){
    uint8 selector_value;
    if(data->size <= sizeof(uint8)){//does the data fit into an uint32 value
        selector_value =  *((uint8*) data->value);//store the value
    }
    
    if(selector_value % 2 == 0){//is it even=true or odd=false
       FRONT_LED = 1;
    }else{
        FRONT_LED = 0;
    }
    
    return true;
}