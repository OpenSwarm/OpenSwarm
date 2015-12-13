/**
 * @file remoteControl.c
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
 * This file includes functions needed to recieve and decode messages from a remote control
 */

#include "remoteControl.h"
#include "../platform/e-puck/system_IO_remoteControl_HDI.h"

#include <stdbool.h>
#include <stdio.h>

#include "io.h"
#include "../events/events.h"
#include "../definitions.h"
#include "../interrupts.h"
    
inline void Sys_Init_RemoteControl(void){
    Sys_Init_RemoteControl_HDI();
}
inline void Sys_Start_RemoteControl(void){
    Sys_Start_RemoteControl_HDI();
}
//https://en.wikipedia.org/wiki/RC-5
void Sys_Receive_RemoteControl_Msg(){
    if(!message_arriving){
        return;
    }
    
    if(waiting_cycles > 1){
        waiting_cycles--;
        return;
    }

    uint8 value = REMOTE;
    static uint16 data_value=0;
        
    if (receiving_bit == RC_NOT_STARTED){
        if(value == 1){//this is only noise
            Sys_Start_AtomicSection();
                Sys_Start_RemoteControl();
                message_arriving = false;
                waiting_cycles = RC_WAIT_INITIALLY;
            Sys_End_AtomicSection();
            return;
        } else {
            waiting_cycles = RC_WAIT_FOR_HALFBIT; //because active low -> wait at the first half of the bit !!
            receiving_bit = 0;
            data_value = 0;
            return;
        }
    }

    if (receiving_bit < 12){
        Sys_Start_AtomicSection();
            data_value <<= 1;
            data_value += value;
            waiting_cycles  = RC_WAIT_FOR_BIT;
            receiving_bit++;
        Sys_End_AtomicSection();
        return;
    }
    
    Sys_Start_AtomicSection();
        Sys_Start_RemoteControl();

        message_arriving = false;
        receiving_bit = RC_NOT_STARTED;
        waiting_cycles = 20;

    Sys_End_AtomicSection();

    if(rx_buffer == data_value){
       return;
    }
    rx_buffer = data_value;
    isNewDataAvailable = true;
            
        //Sys_Memcpy( &data_value, &rx_buffer, 2 );

   uint8 val = rx_buffer & 0x003f;
   Sys_Send_Event(SYS_EVENT_IO_REMOECONTROL, &val, 1);
}

bool Sys_HasRemoteC_Sent_New_Data() {
    return isNewDataAvailable;
}
uint8 Sys_RemoteC_Get_CheckBit() {
    return (rx_buffer & 0x0800) >> 11;
}

uint8 Sys_RemoteC_Get_Address() {
    return (rx_buffer & 0x07c0) >> 6;
}

uint8 Sys_RemoteC_Get_Data() {
    isNewDataAvailable = false;
    return rx_buffer & 0x003f;
}
