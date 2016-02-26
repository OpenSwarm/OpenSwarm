/*!
 * \file
 * \ingroup selector
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * \version 1.0
 *
 * \date 19 February 2016
 * 
 * \brief  It defines functions to obtain the selected value of the selector.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 */

#include "../../definitions.h"
#include "../../events/events.h"
#include "../../io/io.h"

#define SYS_IO_SELECTOR_LOWPASS 25  /*!< The number of cycles that the selector must be steady (to encounter the bouncing) */

void Sys_Selector_PreProcessor(void);

/**
 *
 * This function initialises all the input ports of the microcontroller, initialises the selector handling, and registers the event SYS_EVENT_IO_SELECTOR_CHANGE.
 *
 */
void Sys_Init_Selector(void){
    
    SELECTOR0_DIR = INPUT_PIN;
    SELECTOR1_DIR = INPUT_PIN;
    SELECTOR2_DIR = INPUT_PIN;
    SELECTOR3_DIR = INPUT_PIN;

    if(!Sys_Register_IOHandler(Sys_Selector_PreProcessor)){
        return;
    }
    if(!Sys_Register_Event(SYS_EVENT_IO_SELECTOR_CHANGE)){
        Sys_Unregister_IOHandler(Sys_Selector_PreProcessor);
        return;
    }
}

/**
 *
 * This function is empty and could be used to start the selector handling (not required).
 *
 */
void Sys_Start_Selector(void){
    
}

/**
 *
 * This function returns the current selector value.
 *
 * @return uint8 retuns the current selector value.
 */    
uint8 Sys_Get_Selector(void){
    return SELECTOR0|(SELECTOR1<<1)|(SELECTOR2<<2)|(SELECTOR3<<3);
}

/**
 *
 * Defines a preprocessor callback functions to measure if the selector value has changed. If it did, it sends out an SYS_EVENT_IO_SELECTOR event.
 *
 */
void Sys_Selector_PreProcessor(void){
    static uint8 selector = 0;
    static uint8 filter = 0;
    static uint lowpass = 0;
    
    uint8 value = (SELECTOR3<<3) | (SELECTOR2<<2) | (SELECTOR1<<1) | SELECTOR0;
    
    if(selector == value){
        return;
    }
    
    if (filter == value){
        lowpass++;
    }else{
        lowpass = 0;
        filter = value;
        return;
    }
    
    if(lowpass >= SYS_IO_SELECTOR_LOWPASS){        
        selector = value;
        Sys_Send_Event(SYS_EVENT_IO_SELECTOR_CHANGE, &selector, sizeof(uint8));
    }
    
}