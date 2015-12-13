/*
 * File:   system_IO_uart_HDI.h
 * Author: Stefan M. Trenkwalder
 *
 * Created on 27 August 2015, 13:37
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

#ifndef SYSTEM_IO_UART_HDI_H
#define	SYSTEM_IO_UART_HDI_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../../../os/definitions.h"

#define UART1_RX _RF2
#define UART1_TX _RF3
#define UART2_RX _RF4
#define UART2_TX _RF5

#define UART1_RX_DIR _TRISF2
#define UART1_TX_DIR _TRISF3
#define UART2_RX_DIR _TRISF4
#define UART2_TX_DIR _TRISF5
    
#define SYS_UART1_BAUDRATE 115000
#define SYS_UART2_BAUDRATE 115000
    
typedef struct uart_tx_data_s{
    uint8 *data;
    uint16 length;

    struct uart_tx_data_s *next;
}sys_uart_txdata;

extern sys_uart_txdata *sys_UART1_TX_data;
extern sys_uart_txdata *sys_UART2_TX_data;

extern uint16 byte_counter_uart1;
extern uint16 byte_counter_uart2;

extern pUART_reader read_uart_1;
extern pUART_reader read_uart_2;

void Sys_Init_UART1_HDI(void);
void Sys_Init_UART2_HDI(void);

void Sys_Start_UART1_HDI(void);
void Sys_Start_UART2_HDI(void);

inline void Sys_Read_UART1_ISR();
inline void Sys_Write_UART1_ISR();
inline void Sys_Read_UART2_ISR();
inline void Sys_Write_UART2_ISR();

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_UART_H */

