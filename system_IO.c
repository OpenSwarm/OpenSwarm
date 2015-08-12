
#include <p30F6014A.h>
#include "HDI_DSPIC30F6014A.h"

#include "definitions.h"

typedef struct sys_periodical_IOHandler_s {
    pFunction function;
    
    struct sys_periodical_IOHandler_s *next;
} sys_periodical_IOHandler, sys_pIOHandler;

inline void Sys_Init_IOTimer_HDI();
inline void Sys_Start_IOTimer_HDI();
inline void Sys_IOTimer_code_HDI();

sys_periodical_IOHandler *sys_iohandlers = 0;

void Sys_Init_IOManagement(void){
    Sys_Init_IOTimer_HDI();
}


void Sys_Start_IOManagement(void){
    Sys_Start_IOTimer_HDI();
}

inline void Sys_Init_IOTimer_HDI(){
    TMR3 = 0; //sets countervalue to 0
    PR3 = FCY/(1000); // 16MIPS for 1ms

    // T1CON
    // [TON] [-] [TSIDL] [-] [-] [-] [-] [-] [-] [TGATE] [TCKPS1] [TCKPS0] [-] [TSYNC] [TCS] [-]
    // TON          = enables the Timer1
    // TSIDL        = Timer3 goes to sleep/idle (when processor goes sleeping)
    // TGATE        = Timer gets triggert from external source
    // TCKPS<0:1>   = sets timer prescaler [1, 8, 64, 256]
    // TSYNC        = enables the timer to be synchronised with external source (rising edge)
    // TCS          = sets clock source to external (1) or internal (0)
    T3CON = 0; //timer is turned off but set
    T3CONbits.TCKPS = 1; //Prescaler 8
}

/**
 * Function to starts the initialised system timer
 *
 * This Function starts the Timer0 of the DSPIC 30F6014A for timer interfvals of 10 ms. The MUST be initialised first with Init_SystemTimer_HDI()
 *
 * @param void
 * @return void
 */
inline void Sys_Start_IOTimer_HDI(){
    //this interrupt interrups the system timer
    IPC1 = IPC1 | 0x6000; //set Timer1 interrupt priority level to 5 \in [0,7] where 7 is the highest priority and 0 is disabled

    T3CONbits.TON = 1;//enable timer -> TON = 1
}

/**
 * Activates the Timer1 Interrupt
 *
 * This Function activated the Timer1 Interrupt
 *
 * @param void
 * @return void
 */
inline void Sys_Stop_IOTimer_HDI(){
    IFS0bits.T3IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T3IE = 0; //disable Timer1 interrupt -> T1IE = 0

    T3CONbits.TON = 0; //stops counting
}
inline void Sys_Stop_IOTimer(){
   Sys_Stop_IOTimer_HDI();
}

/**
 * Deactivates the Timer1 Interrupt
 *
 * This Function deactivated the Timer1 Interrupt
 *
 * @param void
 * @return void
 */
inline void Sys_Continue_IOTimer_HDI(){
    IFS0bits.T3IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T3IE = 1; //enable Timer1 interrupt -> T1IE = 1

    T3CONbits.TON = 1; //starts counting
}
inline void Sys_Continue_IOTimer(){
    Sys_Continue_IOTimer_HDI();
}

/**
 * Resets the Timer1 value to the initial value
 *
 * This Function resets the Timer1 value
 *
 * @param void
 * @return void
 */
inline void Sys_Reset_IOTimer_HDI(){
    IFS0bits.T3IF = 0; //unsets the Timer1 interrupt flag
    TMR3 = 0; //sets countervalue to 0
}
inline void Sys_Reset_IOTimer(){
    Sys_Reset_IOTimer_HDI();
}

/**
 * Interrupt Service Rutine for the Timer1 HDI
 *
 * This Function starts the task-scheduling algorithm
 *
 * @param void
 * @return void
 */
void __attribute__((interrupt,no_auto_psv)) _T3Interrupt(){
    Sys_IOTimer_code_HDI();
}

/**
 * Interrupt Service Rutine for the Timer1 HDI (alternate)
 *
 * This Function starts the task-scheduling algorithm
 *
 * @param void
 * @return void
 */
void __attribute__((interrupt,no_auto_psv)) _AltT3Interrupt(){
    Sys_IOTimer_code_HDI();
}

/**
 * Disables the Timer1 interrupt
 *
 * Disables the Timer1 interrupt and sets the interrupt flag to 0
 *
 * @param void
 * @return void
 */
inline void Sys_Disable_IOTimerInterrupt_HDI(){
    IFS0bits.T3IF = 0; //unsets the Timer1 interrupt flag
    IEC0bits.T3IE = 0; //enable Timer1 interrupt -> T1IE = 1
}
inline void Sys_Disable_IOTimerInterrupt(){
    Sys_Disable_IOTimerInterrupt_HDI();
}

/**
 * Enables the Timer1 interrupt
 *
 * Enables the Timer1 interrupt and leaves the interrupt flag to its value. If the flag was set, the Timer1 interrupt will be emitted after executing this function.
 *
 * @param void
 * @return void
 */
inline void Sys_Enable_IOTimerInterrupt_HDI(){
    IEC0bits.T3IE = 1; //enable Timer1 interrupt -> T1IE = 1
}
inline void Sys_Enable_IOTimerInterrupt(){
    Sys_Enable_IOTimerInterrupt_HDI();
}
/**
 * Enables the Timer1 interrupt
 *
 * Enables the Timer1 interrupt and leaves the interrupt flag to its value. If the flag was set, the Timer1 interrupt will be emitted after executing this function.
 *
 * @param void
 * @return void
 */
inline void Sys_Force_IOTimerInterrupt_HDI(){
    IFS0bits.T3IF = 1; //enable Timer1 interrupt -> T1IE = 1
}
inline void Sys_Force_IOTimerInterrupt(){
    Sys_Force_IOTimerInterrupt_HDI();
}

inline void Sys_IOTimer_code_HDI(){
    sys_periodical_IOHandler *handlers = sys_iohandlers;

    while(handlers != 0){//execute all handler functions
        handlers->function();

        handlers = handlers->next;
    }

    Sys_Reset_IOTimer_HDI();
}

bool Sys_Register_IOHandler(pFunction func){

    sys_pIOHandler *new_handler = malloc(sizeof(sys_periodical_IOHandler));
    if(new_handler ==){//malloc failed
        return false;
    }

    new_handler->function = func;
    new_handler->next = 0;

    if(sys_iohandlers == 0){
        sys_iohandlers = new_handler;
        return true;
    }

    sys_pIOHandler *handler = sys_iohandlers;
    while(handler != 0){
        if(handler->next == 0){
            handler->next = new_handler;
        }

        handler = handler->next;
    }
}


void Sys_Unregister_IOHandler(pFunction func){
    sys_pIOHandler *handler = sys_iohandlers;
    sys_pIOHandler *pre_handler = sys_iohandlers;
    while(handler != 0){
        if(handler->function == func){
            pre_handler->next = handler->next;
            handler->next = 0;
            free(handler);
            return;
        }

        pre_handler = handler;
        handler = handler->next;
    }
}
