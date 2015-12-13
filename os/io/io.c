/**
 * @file i2c.c
 * @author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Created on 10 August 2015
 *
 * LICENSE: adapted FreeBSD License
 * Copyright (c) 2015, Stefan M. Trenkwalder
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. If this or parts of this source code (as code or binary) is, in any form, used for an commercial product or service (in any form), this product or service must provide a clear notice/message to any user/customer that OpenSwarm was used in this product.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * @section DESCRIPTION
 *
 * This file includes the IO timer and functions to execute IO functions periodically.
 */

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
