/**
 * @file system_Timer.c
 * @author  Stefan M. Trenkwalder <stefan.markus.trenkwalder@gmail.com>
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
 * This file includes all hardware dependent functions, which are nesessary to initialise, configure and run the system Time
 */

#include "../platform/e-puck/system_Timer_HDI.h"
#include "process_Management.h"

#include "../platform/e-puck/DSPIC30F6014A_HDI.h"
#include "../../os/interrupts.h"

inline void Sys_todo_SystemTimer(); //this is only an inline functions to process the same in T1 interrupt and ALT-T1 interrupt

/**
 * Function to initialise the system timer
 *
 * This Function sets the Timer0 of the DSPIC 30F6014A for timer interfvals of 10 ms. The timer will be startet with Start_SystemTimer_HDI()
 *
 * @param[in,out] scheduler This is a pointer t an callback function, which schuld becalled whenever a timer interrupt is emmitted.
 * @return void
 */
inline void Sys_Init_SystemTimer(pFunction scheduler){
    Sys_Init_SystemTimer_HDI(scheduler);
}


/**
 * Function to starts the initialised system timer
 *
 * This Function starts the Timer0 of the DSPIC 30F6014A for timer interfvals of 10 ms. The MUST be initialised first with Init_SystemTimer_HDI()
 *
 * @param void
 * @return void
 */
inline void Sys_Start_SystemTimer(){
    Sys_Start_SystemTimer_HDI();
}

/**
 * Activates the Timer1 Interrupt
 *
 * This Function activated the Timer1 Interrupt
 *
 * @param void
 * @return void
 */
inline void Sys_Stop_SystemTimer(){
    Sys_Stop_SystemTimer_HDI();
}

/**
 * Deactivates the Timer1 Interrupt
 *
 * This Function deactivated the Timer1 Interrupt
 *
 * @param void
 * @return void
 */
inline void Sys_Continue_SystemTimer(){
    Sys_Continue_SystemTimer_HDI();
}

/**
 * Resets the Timer1 value to the initial value
 *
 * This Function resets the Timer1 value
 *
 * @param void
 * @return void
 */
inline void Sys_Reset_SystemTimer(){
    Sys_Reset_SystemTimer_HDI();
}

/**
 * This function is executed periodically by the system timer interrupt
 *
 * This function is executed periodically by the system timer interrupt. It kills all zombies, executes event handlers and executes the scheduling algorithm.
 *
 * @param void
 * @return void
 */
inline void Sys_todo_SystemTimer(){

    Sys_Stop_SystemTimer_HDI();

    Sys_Kill_Zombies();

    if(sys_process_scheduler != 0){
        sys_process_scheduler();
    }

    Sys_Execute_All_EventHandler();

    Sys_Reset_SystemTimer_HDI(); //to guarantee the same execution time
    Sys_Continue_SystemTimer_HDI();
}

/**
 * Disables the Timer1 interrupt
 *
 * Disables the Timer1 interrupt and sets the interrupt flag to 0
 *
 * @param void
 * @return void
 */
inline void Sys_Disable_TimerInterrupt(void){
    Sys_Disable_TimerInterrupt_HDI();
}

/**
 * Enables the Timer1 interrupt
 *
 * Enables the Timer1 interrupt and leaves the interrupt flag to its value. If the flag was set, the Timer1 interrupt will be emitted after executing this function.
 *
 * @param void
 * @return void
 */
inline void Sys_Enable_TimerInterrupt(void){
    Sys_Enable_TimerInterrupt_HDI();
}

/**
 * Enables the Timer1 interrupt
 *
 * Enables the Timer1 interrupt and leaves the interrupt flag to its value. If the flag was set, the Timer1 interrupt will be emitted after executing this function.
 *
 * @param void
 * @return void
 */
inline void Sys_Force_TimerInterrupt(void){
    Sys_Force_TimerInterrupt_HDI();
}
