/*!
 * \file
 * \ingroup io
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 28 July 2015
 * 
 * \brief  It declares functions to control the IO timer and to (un)register IO Handler.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

/*! \defgroup io I/O Management
 * \brief I/O Management controls the input (e.g. sensors or camera), output (e.g. motors), and communication (e.g. UART, CAN, Bluetooth, ...) devices. For robots, I/O devices are important to interact with its environment, which is the main purpose in robotics. Consequently, this module provides functions and mechanisms to use these I/O devices.
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * \section io_intro Introduction
 * The I/O Management is started by initialising & starting of the \ref base
 * 
 * I/O devices build the interface between environment and robot. In general, I/O devices can be divided into two sets:
 *      - interrupt-driven I/O devices are dedicated hardware modules on the microcontroller that signal by emitting hardware interrupts at defined changes of the device. 
 *        As a result, these devices work concurrently and independently of the main OpenSwarm processes.
 *        For instance, the UART signals a received byte; sent byte; empty sending/receiving buffer; and full buffer. Interrupt-driven I/O devices on a microcontroller are usually UART, I2C, ADC, External Interrupt pins, ...  
 *      - non-interrupt-driven I/O devices has to be checked regularly if its state has changed. This is done by the I/O handler that are executed by occurrence of the I/O timer. 
 *        non-interrupt-driven I/O devices on a microcontroller are usually General purpose IO (GPIO) pins and any device that is connected to them.
 * 
 * 
 * Commonly used on robots, I/O devices might be a camera, motors, or gripper. 
 *  
 * 
 * \section io_usage Usage
 * The I/O management is initialised with Sys_Init_IOManagement(), which initialises the System Timer (100us) and a list of I/O handlers that need to be executed periodically. After starting the timer with Sys_Start_IOManagement(void), it can be the stopped with Sys_Stop_IOManagement(void).
 * 
 * The I/O Timer can be manipulated as follows
 *   - Stop: Sys_Stop_IOTimer(void)
 *   - Continue: Sys_Continue_IOTimer(void)
 *   - Reset: Sys_Reset_IOTimer(void)
 *   - Disable: Sys_Disable_IOTimerInterrupt(void)
 *   - Enable: Sys_Enable_IOTimerInterrupt(void)
 *   - Force an I/O Timer interrupt: Sys_Force_IOTimerInterrupt(void)
 * 
 * New I/O devices can be added and removed during run-time by (un)registering with Sys_Register_IOHandler(pFunction func) and Sys_Unregister_IOHandler(pFunction func).
 * 
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

#include "io_clock.h"
    
#ifdef EPUCK_USED
#include "e-puck/camera.h"
#include "e-puck/i2c.h"
#include "e-puck/motors.h"
#include "e-puck/remoteControl.h"
#include "e-puck/uart.h"
#endif
    
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

