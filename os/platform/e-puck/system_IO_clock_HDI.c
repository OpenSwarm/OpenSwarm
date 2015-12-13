/**
 * @file system_IO_clock_HDI.c
 * @author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Created on 11 September 2015
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
 * This file includes hardware dependent code for the System Clock.
 */

#include "system_IO_clock.h"
#include "../platform/e-puck/system_IO_HDI.h"

#include "system_IO.h"
#include "../events/system_Events.h"

static uint32 sys_clock = 0;
    
void Sys_SystemClock_Counter(void);

inline void Sys_Init_Clock(){
    Sys_Register_Event(SYS_EVENT_1ms_CLOCK);
    Sys_Register_IOHandler(Sys_SystemClock_Counter);
}

inline void Sys_Init_SystemTime(){
    Sys_Init_Clock();
}

void Sys_SystemClock_Counter(){
    
    static uint16 counter = 0;
    
    if( counter < STEPS_PER_MILISECOND){
        counter++;
        return;
    }
    Sys_Send_Event(SYS_EVENT_1ms_CLOCK, &sys_clock, sizeof(sys_clock));
    counter = 0;
    sys_clock++;
}

inline uint32 Sys_Get_SystemTime(){
    return Sys_Get_SystemClock();
}

inline uint32 Sys_Get_SystemClock(){
    return sys_clock;
}
