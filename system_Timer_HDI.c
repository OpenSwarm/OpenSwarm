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

#include "HDI_DSPIC30F6014A.h"

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
    
    TMR1 = 0; //sets countervalue to 0
    PR1 = FCY/(100*8); // 16MIPS for 10ms -> (160 000 I)/(10 ms) -- PRESCALER 8 ->  20000

    // T1CON
    // [TON] [-] [TSIDL] [-] [-] [-] [-] [-] [-] [TGATE] [TCKPS1] [TCKPS0] [-] [TSYNC] [TCS] [-]
    // TON          = enables the Timer1
    // TSIDL        = Timer1 goes to sleep/idle (when processor goes sleeping)
    // TGATE        = Timer gets triggert from external source
    // TCKPS<0:1>   = sets timer prescaler [1, 8, 64, 256]
    // TSYNC        = enables the timer to be synchronised with external source (rising edge)
    // TCS          = sets clock source to external (1) or internal (0)
    T1CON = 0; //timer is turned off but set
    T1CONbits.TCKPS = 1; //Prescaler 8


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

    IPC0 = IPC0 | 0x5000; //set Timer1 interrupt priority level to 5 \in [0,7] where 7 is the highest priority and 0 is disabled 

    Sys_Activate_SystemTimer_Interrupt_HDI();
    T1CONbits.TON = 1;//enable timer -> TON = 1
}

/**
 * Activates the Timer1 Interrupt
 *
 * This Function activated the Timer1 Interrupt
 *
 * @param void
 * @return void
 */
inline void Sys_Deactivate_SystemTimer_Interrupt_HDI(){
    IFS0bits.T1IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T1IE = 0; //disable Timer1 interrupt -> T1IE = 0
}

/**
 * Deactivates the Timer1 Interrupt
 *
 * This Function deactivated the Timer1 Interrupt
 *
 * @param void
 * @return void
 */
inline void Sys_Activate_SystemTimer_Interrupt_HDI(){
    IFS0bits.T1IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T1IE = 1; //enable Timer1 interrupt -> T1IE = 1
}

/**
 * Interrupt Service Rutine for the Timer1 HDI
 *
 * This Function starts the task-scheduling algorithm
 *
 * @param void
 * @return void
 */
void __attribute__((interrupt,no_auto_psv)) _T1Interrupt(void){

    Sys_Deactivate_SystemTimer_Interrupt_HDI();

    if(sys_process_scheduler != 0){
        sys_process_scheduler();
    }

    Sys_Activate_SystemTimer_Interrupt_HDI();
}

/**
 * Interrupt Service Rutine for the Timer1 HDI (alternate)
 *
 * This Function starts the task-scheduling algorithm
 *
 * @param void
 * @return void
 */
void __attribute__((interrupt,no_auto_psv)) _AltT1Interrupt(void){

    Sys_Deactivate_SystemTimer_Interrupt_HDI();

    if(sys_process_scheduler != 0){
        sys_process_scheduler();
    }

    Sys_Activate_SystemTimer_Interrupt_HDI();
}
