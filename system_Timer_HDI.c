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

#include "system_Timer_HDI.h"
#include "system_Process_Management_HDI.h"

#include "HDI_DSPIC30F6014A.h"
#include "system_IRQ_Priorities.h"

inline void Sys_Stop_SystemTimer_HDI(void); //deactivate the periodic interrupts
inline void Sys_Continue_SystemTimer_HDI(void);   //activates them again

inline void Sys_todo_SystemTimer(); //this is only an inline functions to process the same in T1 interrupt and ALT-T1 interrupt

static pFunction sys_process_scheduler = 0; //points to the task scheduling algorithm

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
    PR2 = 50*MILLISEC/256; // 16MIPS for 50ms

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