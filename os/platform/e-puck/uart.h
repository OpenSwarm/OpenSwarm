/*!
 * \file
 * \ingroup uart
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 27 August 2015
 * 
 * \brief It declares functions to transmit bytes via UART.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

/*! \defgroup uart UART 1&2
 * \ingroup epuck
 * \ingroup io
 * 
 * \brief Functions to control the message flow of the UART interface.
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * A UART (Universal Asynchronous Receiver Transmitter) interface is common on microcontroller to communicate with other devices on a serial bus (https://en.wikipedia.org/wiki/Universal_asynchronous_receiver/transmitter)
 * The UART 1 is used on the epuck to communicate with the Bluetooth transceiver. 
 * 
 * \section uart_usage Usage
 * After the initialisation with Sys_Init_UART1() (same applies to UART2), the UART interface needs to be started to be able to receive or transmit bytes.
 * This can be done by sending the bytes via event to SYS_EVENT_IO_TO_BLUETOOTH (UART1) or by handing over the bytes directly by calling Sys_Writeto_UART1(void *, uint length) and Sys_Writeto_UART2(void *, uint).
 * Incoming bytes can be received by defining a reading function with Sys_SetReadingFunction_UART1(pUART_reader) and Sys_SetReadingFunction_UART2(pUART_reader).
 * This function is executed every time a new byte arrives.
 * 
 * \section uart_license License
 * LICENSE: adapted FreeBSD License (see http://openswarm.org/license)\n
 * Copyright (c) 2015, Stefan M. Trenkwalder\n
 * All rights reserved. 
 */

#ifndef SYSTEM_IO_UART_H
#define	SYSTEM_IO_UART_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../../definitions.h"

inline void Sys_Init_UART1(void);
inline void Sys_Init_UART2(void);

inline void Sys_Start_UART1(void);
inline void Sys_Start_UART2(void);

void Sys_SetReadingFunction_UART1(pUART_reader func);
void Sys_SetReadingFunction_UART2(pUART_reader func);

void Sys_Writeto_UART1(void *data, uint length);
void Sys_Writeto_UART2(void *data, uint length);


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_UART_H */

