/*!
 * \file
 * \ingroup io
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 28 July 2015
 * 
 * \brief  declares functions to control the IO timer and to (un)register IO Handler.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

/*! \defgroup io I/O Management
 * \brief Functions and mechanisms to use I/O devices (e.g. sensors and actuators) to interact with the environment.
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * \section io_intro Introduction
 * I/O device are managed by this module. I/O devices interfacing and interacting with the environment of the robot. 
 * These sensors and actuators might be a camera, motors, or gripper. 
 * 
 * In general I/O devices might be independent and uses their own interrupts - such as UART, ADC, I2C. These functions act independently and only need to be initialised and started. No further interaction is needed.
 * 
 * Many I/O devices however need periodic interactions - such as remote control receiver, motor controller, or system clock.
 * 
 * 
 * \section io_usage Usage
 * The I/O management is initialised with Sys_Init_IOManagement(void), which initialised the System Timer (100us) and initialises a list of I/O devices that need to be executed periodically. After starting the timer with Sys_Start_IOManagement(void), it can be the stopped with Sys_Stop_IOManagement(void).
 * 
 * The I/O Timer can be manipulated as follows
 *   - Stop: Sys_Stop_IOTimer(void)
 *   - Continue: Sys_Continue_IOTimer(void)
 *   - Reset (starts the 100us again): Sys_Reset_IOTimer(void)
 *   - Disable: Sys_Disable_IOTimerInterrupt(void)
 *   - Enable: Sys_Enable_IOTimerInterrupt(void)
 *   - Force an I/O Timer interrupt: Sys_Force_IOTimerInterrupt(void)
 * 
 * New I/O devices can be added and removed by (un)registering with Sys_Register_IOHandler(pFunction func) and Sys_Unregister_IOHandler(pFunction func).
 * 
 * The I/O management is started by initialising & starting of the kernel \sa base
 * 
 * \section io_license License
 * LICENSE: adapted FreeBSD License (see http://openswarm.org/license)\n
 * Copyright (c) 2015, Stefan M. Trenkwalder\n
 * All rights reserved. 
 */


#ifndef SYSTEM_IO_H
#define	SYSTEM_IO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../definitions.h"
    
inline void Sys_Init_IOManagement(void);   //initialises the system timer and sets a scheduling algorithm
inline void Sys_Start_IOManagement(void);
inline void Sys_Stop_IOManagement(void);

inline void Sys_Stop_IOTimer(void);
inline void Sys_Continue_IOTimer(void);
inline void Sys_Reset_IOTimer(void);
inline void Sys_Disable_IOTimerInterrupt(void);
inline void Sys_Enable_IOTimerInterrupt(void);
inline void Sys_Force_IOTimerInterrupt(void);

bool Sys_Register_IOHandler(pFunction func);
void Sys_Unregister_IOHandler(pFunction func);

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_H */

