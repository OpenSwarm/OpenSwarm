/**
 * @file system.c
 * @author  Stefan M. Trenkwalder <stefan.markus.trenkwalder@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; http://www.gnu.org/copyleft/gpl.html
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



/* Refer to the device Family Reference Manual Oscillator section for
information about available oscillator configurations.  Typically
this would involve configuring the oscillator tuning register or clock
switching useing the compiler's __builtin_write_OSCCON functions.
Refer to the C Compiler for PIC24F MCUs and dsPIC DSCs User Guide in the
compiler installation directory /doc folder for documentation on the
__builtin functions.  Refer to the XC16 C Compiler User's Guide appendix G
 for a list of the XC16 compiler __builtin functions */

/* TODO Add clock switching code if appropriate.  An example stub is below.   */
void ConfigureOscillator(void)
{
#if 0
        /* Disable Watch Dog Timer */
        RCONbits.SWDTEN = 0;

        /* When clock switch occurs switch to Pri Osc controlled by FPR<4:0> */
        __builtin_write_OSCCONH(0x03);  /* Set OSCCONH for clock switch */
        __builtin_write_OSCCONL(0x01);  /* Start clock switching */
        while(OSCCONbits.COSC != 0b011);

        /* Wait for Clock switch to occur */
        /* Wait for PLL to lock, if PLL is used */
        /* while(OSCCONbits.LOCK != 1); */
#endif

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

