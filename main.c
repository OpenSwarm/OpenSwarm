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

#include "os/communication/communication.h"
#include "os/communication/physical.h"

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void bluetooth_reader(uint8 data);


//uint8 prox_readings[36] = {'@', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//                          0x00,  ':', 0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 
//                          0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 
//                          0x0F, 0xFF, '\r', '\n'};
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

void clearLEDs();
void setLEDs();
void clearIRs();
void setIRs();

int16_t main(void)
{    
    Sys_Init_Kernel(); 
    
    Sys_SetReadingFunction_UART1(bluetooth_reader);
    
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
    
    uint32 time = Sys_Get_SystemClock();
    time += (uint32) 1000;
 
//    uint counter = 0;
    
    while(true){
        
    //setIRs();
        Sys_Message *msg;
        while( (msg = getNewMessage())){ //!= 0 
            //char back[] ={'r',0,0,0,0,0};
            //Sys_Memcpy(&(msg->data), &back[1], 4);
            //back[5] = back[1] ^ back[2] ^  back[3] ^  back[4];
            //Sys_Writeto_UART1(back, 6);
            
        }
        
        uint32 time_now = Sys_Get_SystemClock();
        if(time_now >= time){//wait the refresh_rate time
        
            time += (uint32) 100;
        }
    }
}

uint8 rx_BT_Buffer[6];

void bluetooth_reader(uint8 data){
    static int counter = 0;
    
    if(counter == 0 && data != 's'){
        return;
    }
    
    rx_BT_Buffer[counter] = data;
    counter++;
    
    if(counter < 6){
        return;
    }
    
    counter = 0;
    
    if(rx_BT_Buffer[0] == 's'){
        if( (rx_BT_Buffer[1] ^ rx_BT_Buffer[2] ^  rx_BT_Buffer[3] ^  rx_BT_Buffer[4]) ==  rx_BT_Buffer[5]){
            //Sys_Send_Data(0b101010, &rx_BT_Buffer[1], 4);
            
            Sys_SendTestPattern();
            
            //char ack[] ={'a',0,0,0,0,0};
            //Sys_Memcpy(&rx_BT_Buffer[1], &ack[1],5);
            //Sys_Writeto_UART1(ack, 6);
            return;
        }
    }
    
    char nack[] ={'e',0,0,0,0,0};
    Sys_Memcpy(&rx_BT_Buffer[0], &nack[1], 5);
    Sys_Writeto_UART1(nack, 6);
}
