#include "io.h"
#include "../platform/e-puck/system_IO_HDI.h"
#include <stdlib.h>

#include "../platform/e-puck/DSPIC30F6014A_HDI.h"

#include "../definitions.h"
#include "../interrupts.h"
#include "../memory.h"

#include "motors.h"

void Sys_Init_IOManagement(void){
    Sys_Init_IOTimer_HDI();
}

void Sys_Start_IOManagement(void){
    Sys_Start_IOTimer_HDI();
}

void Sys_Stop_IOManagement(void){
    Sys_Stop_IOTimer_HDI();
}

/**
 * Activates the Timer1 Interrupt
 *
 * This Function activated the Timer1 Interrupt
 *
 * @param void
 * @return void
 */
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
inline void Sys_Reset_IOTimer(){
    Sys_Reset_IOTimer_HDI();
}

/**
 * Disables the Timer1 interrupt
 *
 * Disables the Timer1 interrupt and sets the interrupt flag to 0
 *
 * @param void
 * @return void
 */
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
inline void Sys_Force_IOTimerInterrupt(){
    Sys_Force_IOTimerInterrupt_HDI();
}

bool Sys_Register_IOHandler(pFunction func){

    sys_pIOHandler *new_handler = Sys_Malloc(sizeof(sys_periodical_IOHandler));
    if(new_handler == 0){//malloc failed
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
            return true;
        }

        handler = handler->next;
    }

    return false;
}

void Sys_Unregister_IOHandler(pFunction func){
    sys_pIOHandler *handler = sys_iohandlers;
    sys_pIOHandler *pre_handler = sys_iohandlers;
    while(handler != 0){
        if(handler->function == func){
            pre_handler->next = handler->next;
            handler->next = 0;
            Sys_Free(handler);
            return;
        }

        pre_handler = handler;
        handler = handler->next;
    }
}
