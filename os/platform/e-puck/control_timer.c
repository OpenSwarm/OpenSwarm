#include <p30F6014A.h>

#include "control_timer.h"

#include "../../definitions.h"
#include "adc.h"
#include "camera.h"

#define CONTROLTIME 25
#define PRESCALER 1

int run_behaviour = 0;
uint16 phase = 0;

inline void Sys_todo_ControlTimer();

inline void Sys_Init_ControlTimer(void){
    
    TMR1 = 0; //sets countervalue to 0
    PR1 = CONTROLTIME*MILLISEC/8;

    // T1CON
    // [TON] [-] [TSIDL] [-] [-] [-] [-] [-] [-] [TGATE] [TCKPS1] [TCKPS0] [-] [TSYNC] [TCS] [-]
    // TON          = enables the Timer2
    // TSIDL        = Timer2 goes to sleep/idle (when processor goes sleeping)
    // TGATE        = Timer gets triggert from external source
    // TCKPS<0:1>   = sets timer prescaler [1, 8, 64, 256]
    // TSYNC        = enables the timer to be synchronised with external source (rising edge)
    // TCS          = sets clock source to external (1) or internal (0)
    T1CON = 0; //default
    T1CONbits.TCKPS = 1; //Prescaler 8
}

inline void Sys_Start_ControlTimer(void){
    Sys_Continue_ControlTimer();
}

/**
 *
 * This Function activated the Timer2 Interrupt
 *
 */
inline void Sys_Pause_ControlTimer(){
    IFS0bits.T1IF = 0; //unsets the Timer2 interrupt flag
    IEC0bits.T1IE = 0; //disable Timer2 interrupt -> T1IE = 0

    T1CONbits.TON = 0; //stops counting
}

/**
 *
 * This Function deactivated the Timer2 Interrupt
 *
 */
inline void Sys_Continue_ControlTimer(){
    IFS0bits.T1IF = 0; //unsets the Timer2 interrupt flag
    IEC0bits.T1IE = 1; //enable Timer2 interrupt -> T1IE = 1

    T1CONbits.TON = 1; //starts counting
}
/**
 *
 * This Function starts the task-scheduling algorithm
 *
 */
void __attribute__((interrupt,no_auto_psv)) _T1Interrupt(void){
    Sys_todo_ControlTimer();
    
    IFS0bits.T1IF = 0; //unsets the Timer1 interrupt flag
}

/**
 *
 * This Function starts the task-scheduling algorithm
 *
 */
void __attribute__((interrupt,no_auto_psv)) _AltT1Interrupt(void){
    Sys_todo_ControlTimer();
    
    IFS0bits.T1IF = 0; //unsets the Timer1 interrupt flag
}

/**
 *
 * Disables the Timer2 interrupt and sets the interrupt flag to 0
 *
 */
inline void Sys_Disable_ControlTimerInterrupt(void){
    IFS0bits.T1IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T1IE = 0; //enable Timer1 interrupt -> T1IE = 1
}

/**
 *
 * Enables the Timer2 interrupt and leaves the interrupt flag to its value. If the flag was set, the Timer1 interrupt will be emitted after executing this function.
 *
 */
inline void Sys_Enable_ControlTimerInterrupt(void){
    IEC0bits.T1IE = 1; //enable Timer1 interrupt -> T1IE = 1
}

/**
 *
 * forces the Timer2 interrupt.
 *
 */
#define EPS 1
#define TAU 2000

#define LED_THRES 250
#define REF_THRES 500

#define CONTROLLER_TIME 25 //ms

inline void Sys_Force_ControlTimerInterrupt(void){
    IFS0bits.T1IF = 1; //enable Timer1 interrupt -> T1IE = 1
}

inline void Sys_todo_ControlTimer(){    
    if(run_behaviour){            
        phase += CONTROLLER_TIME;
        
        if (phase > LED_THRES) {
            ledsOff();
        }
        
        if (phase > REF_THRES) {
            if (seeFlash()) {
                phase += (phase*EPS)/10;
                BODY_LED = 1;
//              BODY_LED = ~BODY_LED;
            }
            else {
                BODY_LED = 0;
            }
        }

        if (phase >= TAU) {
            ledsOn();
            phase = 0;
            BODY_LED = 0;                
        }     
    }
}

inline void init_behaviour(){
    phase = 0;Sys_Rand16() % TAU;;
}

inline void start_behaviour(){
    run_behaviour = true;
}

inline void stop_behaviour(){
    run_behaviour = false;
}


void ledsOff(){
    LED0 = 0;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
}

void ledsOn(){
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1;
}
