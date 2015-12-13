/*
 * File:   system_IO_remoteControl.h
 * Author: Stefan M. Trenkwalder
 *
 * Created on 08 September 2015, 12:32
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
 */

#ifndef SZSTEM_IO_REMOTECONTROL_HDI_H
#define SZSTEM_IO_REMOTECONTROL_HDI_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "../../../os/definitions.h"
    

#define RC_WAIT_FOR_QUARTERBIT 4
#define RC_WAIT_FOR_HALFBIT    8
#define RC_WAIT_FOR_BIT        18
#define RC_WAIT_INITIALLY      RC_WAIT_FOR_BIT+RC_WAIT_FOR_QUARTERBIT
#define RC_NOT_STARTED -1
    
extern bool message_arriving;
extern sint8 waiting_cycles;

extern uint16 rx_buffer;

extern bool isNewDataAvailable;

extern sint8 receiving_bit;
    
inline void Sys_Init_RemoteControl_HDI(void);
inline void Sys_Start_RemoteControl_HDI(void);

#ifdef  __cplusplus
}
#endif

#endif  /* SZSTEM_IO_REMOTECONTROL_H */

