/**
 * @file system.c
 * @author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Created on 07 July 2014
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
 * This file includes all basic system calls, which are nesessary to initialise, configure and the operating system.
 */

#include "definitions.h"
#include "platform/e-puck/library/motor_led/e_init_port.h"

#include "system.h"          /* variables/params used by system.c             */
#include "processes/system_Timer.h"    /* functions to start the periodic timer interval */
#include "processes/scheduler.h"    /* the implementation of the scheduler */
#include "processes/process_Management.h"

/*
#include "system_IO.h"
#include "system_IO_clock.h"
    
#ifdef SYS_MOTOR_USED
#include "system_IO_motors.h"
#endif
#ifdef SYS_UART1_USED
#include "system_IO_uart.h"
#endif
#ifdef SYS_REMOTECONTROL_USED
#include "system_IO_remoteControl.h"
#endif
#ifdef SYS_CAMERA_USED
#include "system_IO_camera.h"
#endif

#include "HDI_init_port.h"
#include "definitions.h"
*/

/**
 * Function to initialise the hardware
 *
 * This Function sets the system Timer (Timer0) and sets an scheduling algorithm.
 *
 * @param void
 * @return void
 */
void Sys_Init_Kernel(){

    //Set all pins and ports
    e_init_port();
    
    //Init Scheduling
    Sys_Init_SystemTimer(Sys_Scheduler_RoundRobin);//start the system timer + interrupt = HDI - hardware dependent implementaion
    Sys_Init_Process_Management();

    //Init Events
    //Sys_Register_Event(SYS_EVENT_TERMINATION);
/*
    //init I/O Devices
    Sys_Init_IOManagement();
    Sys_Init_Clock();
    
#ifdef SYS_MOTOR_USED
    Sys_Init_Motors();
#endif
#ifdef SYS_UART1_USED
    Sys_Init_UART1();
#endif
#ifdef SYS_REMOTECONTROL_USED
    Sys_Init_RemoteControl();
#endif
#ifdef SYS_CAMERA_USED
    Sys_Init_Camera();
#endif
    //
*/
}

/**
 * Function to start the  the system timer
 *
 * This Function starts all functions of the operating system. The system MUST HAVE BEEN INITIALISED before.
 *
 * @param void
 * @return void
 */
void Sys_Start_Kernel(void){

    Sys_Start_SystemTimer();
/*
    Sys_Start_IOManagement();
    
#ifdef SYS_MOTOR_USED
#endif
#ifdef SYS_UART1_USED
    Sys_Start_UART1();
#endif
#ifdef SYS_REMOTECONTROL_USED
    Sys_Start_RemoteControl();
#endif
#ifdef SYS_CAMERA_USED
    Sys_Start_Camera();
#endif
    //
*/
}

