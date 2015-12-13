/**
 * @file uart.c
 * @author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Created on 27 August 2015
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
 * 
 * @section DESCRIPTION
 *
 * This file includes functions needed to transmit data via uart(1 & 2).
 */

#include "uart.h"
#include "../platform/e-puck/system_IO_uart_HDI.h"
#include <stdlib.h>
#include <string.h>

#include "../definitions.h"

#include "../interrupts.h"
#include "../memory.h"

#define SYS_UART1_BAUDRATE 115000
#define SYS_UART2_BAUDRATE 115000

inline void Sys_Init_UART1(void){
    Sys_Init_UART1_HDI();
}

inline void Sys_Init_UART2(void){
    Sys_Init_UART2_HDI();
}

inline void Sys_Start_UART1(void){
    Sys_Start_UART1_HDI();
}

inline void Sys_Start_UART2(void){
    Sys_Start_UART2_HDI();
}

void Sys_SetReadingFunction_UART1(pUART_reader func){
    read_uart_1 = func;
}

void Sys_SetReadingFunction_UART2(pUART_reader func){
    read_uart_2 = func;
}

void Sys_Writeto_UART1(void *data, uint16 length){

    sys_uart_txdata *element = Sys_Malloc(sizeof(sys_uart_txdata));
    if(element == 0){//not enough memory
        return;
    }

    element->data = Sys_Malloc(length);
    if(element->data == 0){//not enough memory
        Sys_Free(element);
        return;
    }
    Sys_Memcpy(data,element->data,length);
    
    element->length = length;
    element->next = 0;

    if(sys_UART1_TX_data == 0){//root
        sys_UART1_TX_data = element;
        Sys_Write_UART1_ISR();
        return;
    }

    sys_uart_txdata *list = sys_UART1_TX_data;
    while(list->next != 0){
        list = list->next;
    }
    list->next = element;
}

void Sys_Writeto_UART2(void *data, uint16 length){

    sys_uart_txdata *element = Sys_Malloc(sizeof(sys_uart_txdata));
    if(element == 0){//not enough memory
        return;
    }

    element->data = Sys_Malloc(length);
    if(element->data == 0){//not enough memory
        Sys_Free(element);
        return;
    }
    Sys_Memcpy((uint8 *) data,element->data,length);

    element->length = length;
    element->next = 0;

    if(sys_UART2_TX_data == 0){//root
        sys_UART2_TX_data = element;
        Sys_Write_UART2_ISR();
        return;
    }

    sys_uart_txdata *list = sys_UART2_TX_data;
    while(list->next != 0){
        list = list->next;
    }
    list->next = element;
}
