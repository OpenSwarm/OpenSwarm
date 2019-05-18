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

//#include "os/system.h"        /* System funct/params, like osc/peripheral config */
//#include "os/memory.h"
//#include "os/interrupts.h"

//#include "os/platform/e-puck/adc.h"
//#include "os/platform/e-puck/selector.h"

//#include "os/communication/channel.h"
//#include "os/communication/communication.h"
//#include "os/communication/physical.h"

#include "extern/platform/e-puck/library/motor_led/e_epuck_ports.h"
#include "extern/platform/e-puck/library/motor_led/e_init_port.h"

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
//void bluetooth_reader(uint8 data);


//uint8 prox_readings[36] = {'@', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//                          0x00,  ':', 0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 
//                          0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 0x0F, 0xFF, 
//                          0x0F, 0xFF, '\r', '\n'};
//uint prox_median[9] = {4095,4095,4095,4095,4095,4095,4095,4095,'!'};

//uint8 prox_flag = 0xFF;
//inline void prox_reader(int index,uint data);
//void prox0_reader(uint data);
//void prox1_reader(uint data);
//void prox2_reader(uint data);
//void prox3_reader(uint data);
//void prox4_reader(uint data);
//void prox5_reader(uint data);
//void prox6_reader(uint data);
//void prox7_reader(uint data);

void clearLEDs();
void setLEDs();
void clearIRs();
void setIRs();


//void readbluetoothBuffer();
//void analyseBuffer(uint8 data);
//
//static int move_forward = 0;

int16_t main(void)
{    
    e_init_port();
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1;
    BODY_LED = 0;
    FRONT_LED = 0;
    
    while(true){
        SRbits.IPL = 0;
        LED0 = 1;
        LED1 = 1;
        LED2 = 1;
        LED3 = 1;
        LED4 = 1;
        LED5 = 1;
        LED6 = 1;
        LED7 = 1;
    }
}

//#define MAX_BUFFER 64

//static uint8 rx_BT_Buffer[MAX_BUFFER];
//static uint8 start_index = 0;
//static uint8 end_index = 0;

//typedef enum {
//    waiting,
//    starting_big,
//    reading_small,
//    reading_big,
//    reading_moving,
//    reading_threshold,
//    finishing
//} bluetooth_state;

/*
void analyseBuffer(uint8 data){
    static int counter = 0;
    static int value = 0;
    static unsigned int num_bytes = 0;
    static unsigned char xor_val = 0;
    static bluetooth_state state = waiting;
    
    switch(state){
        case waiting:
            num_bytes = 0;
            counter = 0;
            xor_val = 0;
            switch(data){
                case 't':
                    state = starting_big;
                    break;
                case 's':
                    state = reading_small;
                    break;
                case 'h'://threshold
                    state = reading_threshold;
                    break;
                case 'm'://move
                    state = reading_moving;
                    break;
                default:
                    break;
            }            
            return;
            
        case starting_big:
            if(data <= 20 ){
                num_bytes = data;
                state = reading_big;
                counter = 0;
                return;
            }
            
            state = waiting;
            
            return;
        case reading_small:
            rx_BT_Buffer[counter] = data;
            counter++;
            xor_val ^= data;
    
            if(counter < 5){
                return;
            }
    
            counter = 0;
    
            if( xor_val ==  rx_BT_Buffer[4]){
                Sys_Send_Data(0b101010, &rx_BT_Buffer[1], 4);
                xor_val = 0;
                state = waiting;
                return;
            }
            
            state = waiting;
            break;
        case reading_threshold:
            if(counter == 0){
                xor_val = data;
                value = 0;
                value = ((uint) data) << 8;
                counter++;
                return;
            }
            
            if(counter == 1){
                xor_val ^= data;
                value |= ((uint) data);
                counter++;
                return;
            }
            
            if(counter == 2){
                if(xor_val == data){
                    Sys_SetComThreshold(value);
                    Sys_Writeto_UART1("OK", 2);
                }
                value = 0;
                xor_val = 0;
                counter = 0;
                state = waiting;
                return;
            }
            
            state = waiting;
            break;
        case reading_moving:
            if(counter == 0){
                xor_val = data;
                value = 0;
                value = ((uint) data) << 8;
                counter++;
                return;
            }
            
            if(counter == 1){
                xor_val ^= data;
                value |= ((uint) data);
                counter++;
                return;
            }
            
            if(counter == 2){
                if(xor_val == data){
                    move_forward = value;
                    Sys_Writeto_UART1("START", 5);
                }
                value = 0;
                xor_val = 0;
                counter = 0;
                state = waiting;
                return;
            }
            
            state = waiting;
            break;
        case reading_big:
            rx_BT_Buffer[counter] = data;
            
            if(counter < (num_bytes)){
                xor_val ^= data;
                counter++;
                return;
            }
            
        case finishing:
            counter = 0;
                        
            if(data == xor_val){
                Sys_Send_Data(0b101010, &rx_BT_Buffer[0], num_bytes);
                xor_val = 0;
                num_bytes = 0;
                state = waiting;
                return;
            }
            
            state = waiting;
        default:
            break;
    }
}

void bluetooth_reader(uint8 data){
    rx_BT_Buffer[end_index] = data;
    end_index = ( (end_index+1) % MAX_BUFFER);
}

void readbluetoothBuffer(){
    if(end_index == start_index){
        return;
    }
    
    while(start_index != end_index){
        uint8 value = rx_BT_Buffer[start_index];
        start_index = ( (start_index+1) % MAX_BUFFER);
        
        analyseBuffer(value);
    }
}
/*
 
uint8 rx_BT_Buffer[21];

typedef enum {
    waiting,
    starting,
    reading,
            finishing
} bluetooth_state;

void bluetooth_reader(uint8 data){
    static int counter = 0;
    static unsigned int num_bytes = 0;
    static bluetooth_state state = waiting;
    
    switch(state){
        case waiting:
            if(data == 's'){
                state = starting;
            }
            
            return;
        case starting:
            num_bytes = data;
            state = reading;
            counter = 0;
            
            return;
        case reading:
            rx_BT_Buffer[counter] = data;
            
            if(counter < num_bytes){
                counter++;
                return;
            }
            
        case finishing:
            counter = 0;
            
            int i = 0;
            unsigned char xor_char = 0;
            for(i = 0; i < num_bytes; i++){
                xor_char ^= rx_BT_Buffer[i];
            }
            
            if(data == xor_char){
                Sys_Send_Data(0b101010, &rx_BT_Buffer[1], num_bytes);
                return;
            }
        default:
            break;
    }
    
    Sys_Writeto_UART1("error", 5);
}
 */