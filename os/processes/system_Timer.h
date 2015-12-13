/* 
 * File:   system_Timer_HDI.h
 * Author: Stefan Trenkwalder
 *
 * Created on 07 July 2014, 14:15
 *
 * LICENCE: adapted FreeBSD Licence
 * Copyright (c) 2015, Stefan Trenkwalder
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. If this or parts of this source code or binary code in any form is used for a commercial product or service in any form, this product or service must provide a clear notice/message readable for any user or customer that this product or service uses OpenSwarm.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef SYSTEMTIMER_H
#define	SYSTEMTIMER_H

#include "../../os/definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif


void Sys_Init_SystemTimer(pFunction);   //initialises the system timer and sets a scheduling algorithm
void Sys_Start_SystemTimer(void);       //starts the system timer (to do periodic scheduling

inline void Sys_Stop_SystemTimer();
inline void Sys_Continue_SystemTimer();

inline void Sys_Disable_TimerInterrupt(void); //no scheduling possible within a critical section
inline void Sys_Enable_TimerInterrupt(void);   //

inline void Sys_Force_TimerInterrupt(void); //forces an execution of the Timer1 Interrupt routine

inline void Sys_Reset_SystemTimer();

inline void Sys_todo_SystemTimer();

#ifdef	__cplusplus
}
#endif
#endif	/* SYSTEMTIMER_H */

