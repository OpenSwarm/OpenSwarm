/**
 * @file system_Timer_HDI.c
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
 * This file includes all hardware dependent functions, which are nesessary to initialise, configure and run the system Timer
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