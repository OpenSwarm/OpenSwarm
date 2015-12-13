/* 
 * File:   system_Process_Management.h
 * Author: Stefan Trenkwalder
 *
 * Created on 08 July 2014, 18:58
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


#ifndef SYSTEM_PROCESS_MANAGEMENT_HDI_H
#define	SYSTEM_PROCESS_MANAGEMENT_HDI_H


#include "../../../os/processes/data.h"

#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif


//############# PROCESS MANAGEMENT ###############################
    
void Sys_Change_Stack_HDI(uint16 framepointer, uint16 stackpointer, uint16 stackpointerlimit); //this function switches to the new stack

void Sys_Init_Process_Management_HDI(void);// initialises the process management and creates a system process
bool Sys_Start_Process_HDI(pFunction function);

inline void Sys_Save_Running_Process_HDI(void);
void Sys_Change_Stack_HDI(unsigned short fp/*W0*/, unsigned short sp/*W1*/, unsigned short lm/*W2*/);

void Sys_Switch_Process_HDI(sys_pcb_list_element *new_process);

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_PROCESS_MANAGEMENT_H */
