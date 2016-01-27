/*!
 * \file
 * \ingroup process
 * \ingroup hdi
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * \version 1.0
 *
 * \date 07 July 2014
 * 
 * \brief  Hardware dependent implementations to initialise, configure and run the system Timer
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 */

#include "system_Timer_HDI.h"

#include "../../processes/system_Timer.h"
#include "../../processes/process_Management.h"

#include "DSPIC30F6014A_HDI.h"
#include "../../interrupts.h"
#include "../../definitions.h"

pFunction sys_process_scheduler = 0; //points to the task scheduling algorithm

/**
 *
 * This Function sets the Timer0 of the DSPIC 30F6014A for timer interfvals of 10 ms. The timer will be startet with Start_SystemTimer_HDI()
 *
 * @param[in,out] scheduler This is a pointer t an callback function, which schuld becalled whenever a timer interrupt is emmitted.
 */
void Sys_Init_SystemTimer_HDI(pFunction scheduler){

    sys_process_scheduler = scheduler;
    
    TMR2 = 0; //sets countervalue to 0
    PR2 = 10*MILLISEC/256; // 16MIPS for 50ms

    // T1CON
    // [TON] [-] [TSIDL] [-] [-] [-] [-] [-] [-] [TGATE] [TCKPS1] [TCKPS0] [-] [TSYNC] [TCS] [-]
    // TON          = enables the Timer2
    // TSIDL        = Timer2 goes to sleep/idle (when processor goes sleeping)
    // TGATE        = Timer gets triggert from external source
    // TCKPS<0:1>   = sets timer prescaler [1, 8, 64, 256]
    // TSYNC        = enables the timer to be synchronised with external source (rising edge)
    // TCS          = sets clock source to external (1) or internal (0)
    T2CON = 0; //timer is turned off but set
    T2CONbits.TCKPS = 3; //Prescaler 256
}


/**
 *
 * This Function starts the Timer2 of the DSPIC 30F6014A for timer interfvals of 10 ms. The MUST be initialised first with Init_SystemTimer_HDI()
 *
 */
void Sys_Start_SystemTimer_HDI(){

    IPC1bits.T2IP = SYS_IRQP_SYSTEM_TIMER; //set Timer2 interrupt priority level to 5 \in [0,7] where 7 is the highest priority and 0 is disabled

    Sys_Continue_SystemTimer_HDI();
    T2CONbits.TON = 1;//enable timer -> TON = 1
}

/**
 *
 * This Function activated the Timer2 Interrupt
 *
 */
inline void Sys_Stop_SystemTimer_HDI(){
    IFS0bits.T2IF = 0; //unsets the Timer2 interrupt flag
    IEC0bits.T2IE = 0; //disable Timer2 interrupt -> T1IE = 0

    T2CONbits.TON = 0; //stops counting
}

/**
 *
 * This Function deactivated the Timer2 Interrupt
 *
 */
inline void Sys_Continue_SystemTimer_HDI(){
    IFS0bits.T2IF = 0; //unsets the Timer2 interrupt flag
    IEC0bits.T2IE = 1; //enable Timer2 interrupt -> T1IE = 1

    T2CONbits.TON = 1; //starts counting
}

/**
 *
 * This Function resets the Timer2 value
 *
 */
inline void Sys_Reset_SystemTimer_HDI(){
    TMR2 = 0; //sets counter value to 0
}

/**
 *
 * This Function starts the task-scheduling algorithm
 *
 */
void __attribute__((interrupt,no_auto_psv)) _T2Interrupt(void){
    Sys_todo_SystemTimer();
}

/**
 *
 * This Function starts the task-scheduling algorithm
 *
 */
void __attribute__((interrupt,no_auto_psv)) _AltT2Interrupt(void){
    Sys_todo_SystemTimer();
}

/**
 *
 * Disables the Timer2 interrupt and sets the interrupt flag to 0
 *
 */
inline void Sys_Disable_TimerInterrupt_HDI(void){
    IFS0bits.T2IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T2IE = 0; //enable Timer1 interrupt -> T1IE = 1
}

/**
 *
 * Enables the Timer2 interrupt and leaves the interrupt flag to its value. If the flag was set, the Timer1 interrupt will be emitted after executing this function.
 *
 */
inline void Sys_Enable_TimerInterrupt_HDI(void){
    IEC0bits.T2IE = 1; //enable Timer1 interrupt -> T1IE = 1
}

/**
 *
 * forces the Timer2 interrupt.
 *
 */
inline void Sys_Force_TimerInterrupt_HDI(void){
    IFS0bits.T2IF = 1; //enable Timer1 interrupt -> T1IE = 1
}
