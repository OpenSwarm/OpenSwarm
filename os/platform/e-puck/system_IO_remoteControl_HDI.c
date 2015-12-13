/**
 * @file system_IO_remoteControl.c
 * @author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Created on 08 September 2015
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
 * This file includes hardware dependent code to decode remote control signals.
 */

#include "system_IO_remoteControl_HDI.h"

#include "../../../os/definitions.h"
#include "../../../os/interrupts.h"
#include "../../../os/io/io.h"
#include "../../../os/io/remoteControl.h"
#include "../../../os/events/events.h"

bool message_arriving = false;
sint8 waiting_cycles = 20;

uint16 rx_buffer = 0;

bool isNewDataAvailable = false;

sint8 receiving_bit = RC_NOT_STARTED;

inline void Sys_Init_RemoteControl_HDI(void){
    REMOTE_DIR = INPUT_PIN; 
    
    IPC0bits.INT0IP = SYS_IRQP_REMOTECONTROL;
    INTCON2bits.INT0EP = 1;     //use falling edge
    IFS0bits.INT0IF = 0;        //clear to IRQ flag
    
    if(!Sys_Register_IOHandler(Sys_Receive_RemoteControl_Msg)){// no error handling used
        return;
    }
    
    Sys_Register_Event(SYS_EVENT_IO_REMOECONTROL);
}

inline void Sys_Start_RemoteControl_HDI(void){
    IFS0bits.INT0IF = 0;        ///clear to IRQ flag
    IEC0bits.INT0IE = 1;        //enable external INT0  
}

void __attribute__((__interrupt__, auto_psv))  _INT0Interrupt(void){
    //When a message arrives deactivate interrupt -> is activated at the end of the message
    IEC0bits.INT0IE = 0;
        IFS0bits.INT0IF = 0;
    message_arriving = true;
        waiting_cycles = RC_WAIT_INITIALLY;
    return;
}
