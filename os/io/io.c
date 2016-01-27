/*!
 * \file
 * \ingroup io
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 10 August 2015
 * 
 * \brief  It defines functions to control the IO timer and to (un)register IO Handler.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */


#include "io.h"

#include "../definitions.h"

#ifdef EPUCK_USED 
#include "e-puck/io_HDI.h"
#endif

#include "../interrupts.h"
#include "../memory.h"

/**
 *
 * This function initialises the I/O Timer and therefore the I/O Management.
 *
 */
void Sys_Init_IOManagement(void){
    Sys_Init_IOTimer_HDI();
}

/**
 *
 * This function starts the I/O Timer and therefore the I/O Management.
 *
 */
void Sys_Start_IOManagement(void){
    Sys_Start_IOTimer_HDI();
}

/**
 *
 * This function stops the I/O Timer and therefore the I/O Management.
 *
 */
void Sys_Stop_IOManagement(void){
    Sys_Stop_IOTimer_HDI();
}

/**
 *
 * This function stops the I/O Timer.
 *
 */
inline void Sys_Stop_IOTimer(){
   Sys_Stop_IOTimer_HDI();
}

/**
 *
 * This function continues the I/O Timer. Note that the timer continues to count where it stops.
 *
 */
inline void Sys_Continue_IOTimer(){
    Sys_Continue_IOTimer_HDI();
}

/**
 *
 * This function sets the I/O Timer counter to 0 and the I/O timer needs the full time duration to throw the interrupt.
 *
 */
inline void Sys_Reset_IOTimer(){
    Sys_Reset_IOTimer_HDI();
}

/**
 *
 * This function disables the I/O Timer interrupt. Note that the timer still continues to count.
 *
 */
inline void Sys_Disable_IOTimerInterrupt(){
    Sys_Disable_IOTimerInterrupt_HDI();
}

/**
 *
 * This function enables the I/O Timer interrupt.
 *
 */
inline void Sys_Enable_IOTimerInterrupt(){
    Sys_Enable_IOTimerInterrupt_HDI();
}

/**
 *
 * This function forces a new I/O Timer interrupt even if the timer hasn't reached its threshold.
 *
 */
inline void Sys_Force_IOTimerInterrupt(){
    Sys_Force_IOTimerInterrupt_HDI();
}

/**
 *
 * This function registers a new I/O handler which is executed every time the I/O timer interrupt occurs.
 *
 * @param[in] func  pointer to the function that should be executed by the I/O timer periodically
 * @return true if it was successful
 */
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

/**
 *
 * This function unregisters a I/O handler identified by its function address. 
 *
 * @param[in] func  pointer to the function that should be executed by the I/O timer periodically
 */
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
