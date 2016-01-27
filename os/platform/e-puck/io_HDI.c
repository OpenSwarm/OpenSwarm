/*!
 * \file
 * \ingroup io
 * \ingroup hdi
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 10 August 2015
 * 
 * \brief  Hardware dependent implementations to control the IO timer and to (un)register IO Handler.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#include "io_HDI.h"
#include <stdlib.h>

#include "DSPIC30F6014A_HDI.h"

#include "../../definitions.h"
#include "../../interrupts.h"
#include "../../memory.h"

#include "motors.h"

sys_periodical_IOHandler *sys_iohandlers;	/*!< List of I/O handlers*/


/**
 *
 * This function initialises the I/O Timer.
 *
 */
inline void Sys_Init_IOTimer_HDI(){

    Sys_Stop_IOTimer();

    TMR3 = 0; //sets countervalue to 0
    PR3 = MILLISEC/10; // 16MIPS for 1ms

    // T1CON
    // [TON] [-] [TSIDL] [-] [-] [-] [-] [-] [-] [TGATE] [TCKPS1] [TCKPS0] [-] [TSYNC] [TCS] [-]
    // TON          = enables the Timer1
    // TSIDL        = Timer3 goes to sleep/idle (when processor goes sleeping)
    // TGATE        = Timer gets triggert from external source
    // TCKPS<0:1>   = sets timer prescaler [1, 8, 64, 256]
    // TSYNC        = enables the timer to be synchronised with external source (rising edge)
    // TCS          = sets clock source to external (1) or internal (0)
    T3CON = 0; //timer is turned off but set
    T3CONbits.TCKPS = 0; //Prescaler 8
}

/**
 *
 * This function starts the I/O Timer.
 *
 */
inline void Sys_Start_IOTimer_HDI(){
    //this interrupt interrups the system timer
    IPC1bits.T3IP = SYS_IRQP_IO_TIMER; //set Timer1 interrupt priority level to 5 \in [0,7] where 7 is the highest priority and 0 is disabled

    Sys_Continue_IOTimer();
}

/**
 *
 * This function stops the I/O Timer.
 *
 */
inline void Sys_Stop_IOTimer_HDI(){
    IFS0bits.T3IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T3IE = 0; //disable Timer1 interrupt -> T1IE = 0

    T3CONbits.TON = 0; //stops counting
}

/**
 *
 * This function continues the I/O Timer.
 *
 */
inline void Sys_Continue_IOTimer_HDI(){
    IFS0bits.T3IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T3IE = 1; //enable Timer1 interrupt -> T1IE = 1

    T3CONbits.TON = 1; //starts counting
}

/**
 *
 * This function resets the I/O Timer.
 *
 */
inline void Sys_Reset_IOTimer_HDI(){
    IFS0bits.T3IF = 0; //unsets the Timer1 interrupt flag
    //TMR3 = 0; //sets countervalue to 0
}

/**
 *
 * This Function starts the task-scheduling algorithm
 *
 */
void __attribute__((interrupt,no_auto_psv)) _T3Interrupt(){
    IFS0bits.T3IF = 0;
    Sys_IOTimer_code_HDI();
}

/**
 * Interrupt Service Routine for the Timer1 (alternate) starts the task-scheduling algorithm
 *
 */
void __attribute__((interrupt,no_auto_psv)) _AltT3Interrupt(){
    IFS0bits.T3IF = 0;
    Sys_IOTimer_code_HDI();
}

/**
 *
 * Disables the Timer1 interrupt and sets the interrupt flag to 0
 *
 */
inline void Sys_Disable_IOTimerInterrupt_HDI(){
    IFS0bits.T3IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T3IE = 0; //enable Timer1 interrupt -> T1IE = 1
}

/**
 *
 * Enables the Timer1 interrupt and leaves the interrupt flag to its value. If the flag was set, the Timer1 interrupt will be emitted after executing this function.
 *
 */
inline void Sys_Enable_IOTimerInterrupt_HDI(){
    IEC0bits.T3IE = 1; //enable Timer1 interrupt -> T1IE = 1
}
/**
 *
 * forces the Timer1 interrupt to occur.
 *
 */
inline void Sys_Force_IOTimerInterrupt_HDI(){
    IFS0bits.T3IF = 1; //enable Timer1 interrupt -> T1IE = 1
}

/**
 *
 * This function is executed every time the I/O timer is active and executes all I/O handlers
 *
 */
inline void Sys_IOTimer_code_HDI(){
    sys_periodical_IOHandler *handlers = sys_iohandlers;

    while(handlers != 0){//execute all handler functions
        handlers->function();

        handlers = handlers->next;
    }

    Sys_Reset_IOTimer_HDI();
}
