/*
 * File:   io.h
 * Author: Stefan M. Trenkwalder
 *
 * Created on 28 July 2015, 11:21
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

#ifndef SYSTEM_IO_H
#define	SYSTEM_IO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../definitions.h"
    
inline void Sys_Init_IOManagement(void);   //initialises the system timer and sets a scheduling algorithm
inline void Sys_Start_IOManagement(void);
inline void Sys_Stop_IOManagement(void);

inline void Sys_Stop_IOTimer(void);
inline void Sys_Continue_IOTimer(void);
inline void Sys_Reset_IOTimer(void);
inline void Sys_Disable_IOTimerInterrupt(void);
inline void Sys_Enable_IOTimerInterrupt(void);
inline void Sys_Force_IOTimerInterrupt(void);

bool Sys_Register_IOHandler(pFunction func);
void Sys_Unregister_IOHandler(pFunction func);

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_H */

