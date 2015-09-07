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


/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */

#include "system.h"          /* variables/params used by system.c             */
#include "system_Timer_HDI.h"    /* functions to start the periodic timer interval */
#include "system_Scheduler.h"    /* the implementation of the scheduler */
#include "system_Process_Management_HDI.h"

#include "system_IO.h"
#include "system_IO_motors.h"
#include "system_IO_uart.h"
#include "system_IO_camera.h"

#include "HDI_init_port.h"
#include "definitions.h"


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
    Sys_Init_MCU_HDI();
    
    //Init Scheduling
    Sys_Init_SystemTimer_HDI(Sys_Scheduler_RoundRobin);//start the system timer + interrupt = HDI - hardware dependent implementaion
    Sys_Init_Process_Management_HDI();

    //Init Events
    Sys_Register_Event(SYS_EVENT_TERMINATION);

    //init I/O Devices
    Sys_Init_IOManagement();
    Sys_Init_Motors();
    Sys_Init_UART1();
    Sys_Init_Camera();
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

    Sys_Start_SystemTimer_HDI();

    Sys_Start_IOManagement();
    //Sys_Start_UART1();
    Sys_Start_Camera();
}

