/**
 * @file system_Timer_HDI.c
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

#include "system_Timer_HDI.h"

#include "../../../os/processes/system_Timer.h"
#include "../../../os/processes/process_Management.h"

#include "DSPIC30F6014A_HDI.h"
#include "../../../os/interrupts.h"

pFunction sys_process_scheduler = 0; //points to the task scheduling algorithm

/**
 * Function to initialise the system timer
 *
 * This Function sets the Timer0 of the DSPIC 30F6014A for timer interfvals of 10 ms. The timer will be startet with Start_SystemTimer_HDI()
 *
 * @param[in,out] scheduler This is a pointer t an callback function, which schuld becalled whenever a timer interrupt is emmitted.
 * @return void
 */
void Sys_Init_SystemTimer_HDI(pFunction scheduler){

    sys_process_scheduler = scheduler;
    
    TMR2 = 0; //sets countervalue to 0
    PR2 = 10*MILLISEC/256; // 16MIPS for 50ms

    // T1CON
    // [TON] [-] [TSIDL] [-] [-] [-] [-] [-] [-] [TGATE] [TCKPS1] [TCKPS0] [-] [TSYNC] [TCS] [-]
    // TON          = enables the Timer1
    // TSIDL        = Timer1 goes to sleep/idle (when processor goes sleeping)
    // TGATE        = Timer gets triggert from external source
    // TCKPS<0:1>   = sets timer prescaler [1, 8, 64, 256]
    // TSYNC        = enables the timer to be synchronised with external source (rising edge)
    // TCS          = sets clock source to external (1) or internal (0)
    T2CON = 0; //timer is turned off but set
    T2CONbits.TCKPS = 3; //Prescaler 256
}


/**
 * Function to starts the initialised system timer
 *
 * This Function starts the Timer0 of the DSPIC 30F6014A for timer interfvals of 10 ms. The MUST be initialised first with Init_SystemTimer_HDI()
 *
 * @param void
 * @return void
 */
void Sys_Start_SystemTimer_HDI(){

    IPC1bits.T2IP = SYS_IRQP_SYSTEM_TIMER; //set Timer1 interrupt priority level to 5 \in [0,7] where 7 is the highest priority and 0 is disabled

    Sys_Continue_SystemTimer_HDI();
    T2CONbits.TON = 1;//enable timer -> TON = 1
}

/**
 * Activates the Timer1 Interrupt
 *
 * This Function activated the Timer1 Interrupt
 *
 * @param void
 * @return void
 */
inline void Sys_Stop_SystemTimer_HDI(){
    IFS0bits.T2IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T2IE = 0; //disable Timer1 interrupt -> T1IE = 0

    T2CONbits.TON = 0; //stops counting
}

/**
 * Deactivates the Timer1 Interrupt
 *
 * This Function deactivated the Timer1 Interrupt
 *
 * @param void
 * @return void
 */
inline void Sys_Continue_SystemTimer_HDI(){
    IFS0bits.T2IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T2IE = 1; //enable Timer1 interrupt -> T1IE = 1

    T2CONbits.TON = 1; //starts counting
}

/**
 * Resets the Timer1 value to the initial value
 *
 * This Function resets the Timer1 value
 *
 * @param void
 * @return void
 */
inline void Sys_Reset_SystemTimer_HDI(){
    TMR2 = 0; //sets countervalue to 0
}

/**
 * Interrupt Service Rutine for the Timer1 HDI
 *
 * This Function starts the task-scheduling algorithm
 *
 * @param void
 * @return void
 */
void __attribute__((interrupt,no_auto_psv)) _T2Interrupt(void){
    Sys_todo_SystemTimer();
}

/**
 * Interrupt Service Rutine for the Timer1 HDI (alternate)
 *
 * This Function starts the task-scheduling algorithm
 *
 * @param void
 * @return void
 */
void __attribute__((interrupt,no_auto_psv)) _AltT2Interrupt(void){
    Sys_todo_SystemTimer();
}

/**
 * Disables the Timer1 interrupt
 *
 * Disables the Timer1 interrupt and sets the interrupt flag to 0
 *
 * @param void
 * @return void
 */
inline void Sys_Disable_TimerInterrupt_HDI(void){
    IFS0bits.T2IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T2IE = 0; //enable Timer1 interrupt -> T1IE = 1
}

/**
 * Enables the Timer1 interrupt
 *
 * Enables the Timer1 interrupt and leaves the interrupt flag to its value. If the flag was set, the Timer1 interrupt will be emitted after executing this function.
 *
 * @param void
 * @return void
 */
inline void Sys_Enable_TimerInterrupt_HDI(void){
    IEC0bits.T2IE = 1; //enable Timer1 interrupt -> T1IE = 1
}

/**
 * Enables the Timer1 interrupt
 *
 * Enables the Timer1 interrupt and leaves the interrupt flag to its value. If the flag was set, the Timer1 interrupt will be emitted after executing this function.
 *
 * @param void
 * @return void
 */
inline void Sys_Force_TimerInterrupt_HDI(void){
    IFS0bits.T2IF = 1; //enable Timer1 interrupt -> T1IE = 1
}
