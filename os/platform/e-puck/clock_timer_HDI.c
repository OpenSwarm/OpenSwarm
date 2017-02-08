#include "clock_timer_HDI.h"

#include "../../system.h"

#define CONTROLTIME 10
#define PRESCALER 8

pFunction Sys_ClockTick = 0;

inline void Sys_todo_Clock();

inline void Sys_Init_Clock(void){
    
    TMR1 = CONTROLTIME*MILLISEC/16; //sets countervalue to 50%
    PR1 = CONTROLTIME*MILLISEC/8;

    // T1CON
    // [TON] [-] [TSIDL] [-] [-] [-] [-] [-] [-] [TGATE] [TCKPS1] [TCKPS0] [-] [TSYNC] [TCS] [-]
    // TON          = enables the Timer1
    // TSIDL        = Timer1 goes to sleep/idle (when processor goes sleeping)
    // TGATE        = Timer gets triggert from external source
    // TCKPS<0:1>   = sets timer prescaler [1, 8, 64, 256]
    // TSYNC        = enables the timer to be synchronised with external source (rising edge)
    // TCS          = sets clock source to external (1) or internal (0)
    T1CON = 0; //default
    T1CONbits.TCKPS = 0x01; //Prescaler 8
    
}

inline void Sys_Start_Clock(void){
    Sys_Continue_Clock();
}

/**
 *
 * This Function pauses the Timer1 Interrupt
 *
 */
inline void Sys_Pause_Clock(){
    IFS0bits.T1IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T1IE = 0; //disable Timer1 interrupt -> T1IE = 0

    T1CONbits.TON = 0; //stops counting
}

/**
 *
 * This Function pauses/stops the Timer1 Interrupt
 *
 */
inline void Sys_Stop_Clock(){
    Sys_Pause_Clock();
}

/**
 *
 * This Function continues the Timer1 Interrupt
 *
 */
inline void Sys_Continue_Clock(){
    IFS0bits.T1IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T1IE = 1; //enable Timer1 interrupt -> T1IE = 1

    T1CONbits.TON = 1; //starts counting
}
/**
 *
 * This Function starts the task-scheduling algorithm
 *
 */
void __attribute__((interrupt,no_auto_psv)) _T1Interrupt(void){
    
#ifndef SYS_CLOCK_USED
    IFS0bits.T1IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T1IE = 0; //disable Timer1 interrupt -> T1IE = 0
    
    T1CONbits.TON = 0; //stops counting
    return;
#endif
    
    if(Sys_ClockTick != 0){
        Sys_ClockTick();
    }
    IFS0bits.T1IF = 0; //unsets the Timer1 interrupt flag
}

/**
 *
 * This Function starts the task-scheduling algorithm
 *
 */
void __attribute__((interrupt,no_auto_psv)) _AltT1Interrupt(void){
#ifndef SYS_CLOCK_USED
    IFS0bits.T1IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T1IE = 0; //disable Timer1 interrupt -> T1IE = 0
    
    T1CONbits.TON = 0; //stops counting
    return;
#endif
    
    if(Sys_ClockTick != 0){
        Sys_ClockTick();
    }    
    
    IFS0bits.T1IF = 0; //unsets the Timer1 interrupt flag
}

/**
 *
 * Disables the Timer1 interrupt and sets the interrupt flag to 0
 *
 */
inline void Sys_Disable_ClockInterrupt(void){
    IFS0bits.T1IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T1IE = 0; //enable Timer1 interrupt -> T1IE = 1
}

/**
 *
 * Enables the Timer1 interrupt and leaves the interrupt flag to its value. If the flag was set, the Timer1 interrupt will be emitted after executing this function.
 *
 */
inline void Sys_Enable_ClockInterrupt(void){
    IEC0bits.T1IE = 1; //enable Timer1 interrupt -> T1IE = 1
}

/**
 *
 * forces the Timer1 interrupt.
 *
 */
inline void Sys_Force_ClockInterrupt(void){
    IFS0bits.T1IF = 1; //enable Timer1 interrupt -> T1IE = 1
}

/**
 *
 * Registers a function that is executed every timer tick.
 *
 */
void Sys_Register_TickFunction(pFunction func){
    Sys_ClockTick = func;
}