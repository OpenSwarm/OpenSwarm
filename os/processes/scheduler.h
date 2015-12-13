/* 
 * File:   scheduler.h
 * Author: Stefan M. Trenkwalder
 *
 * Created on 07 July 2014, 17:44
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


#ifndef SYSTEM_SCHEDULER_H
#define	SYSTEM_SCHEDULER_H

#ifdef	__cplusplus
extern "C" {
#endif

/*! process state values */
#define SYS_PROCESS_STATE_BABY 0xBABE
#define SYS_PROCESS_STATE_RUNNING 0xFFFF
#define SYS_PROCESS_STATE_BLOCKED 0xBCED
#define SYS_PROCESS_STATE_WAITING 0x5555
#define SYS_PROCESS_STATE_ZOMBIE 0xDEAD

/*! process priority values */
#define SYS_PROCESS_PRIORITY_SYSTEM 0xFFFF
#define SYS_PROCESS_PRIORITY_HIGH 0x0FFF
#define SYS_PROCESS_PRIORITY_NORMAL 0x00FF
#define SYS_PROCESS_PRIORITY_LOW 0x000F

//!  The scheduling information for each process
/*!
 This struct defines all values wich are needed for the scheduling algorithm
*/
typedef struct sys_scheduler_info_s{
    unsigned short state;       /*!< Process state information */
    unsigned short priority;    /*!< process priority level */
}sys_scheduler_info;


void Sys_Scheduler_RoundRobin(void); //the scheduling algorithm
inline void Sys_Set_Defaults_Info(sys_scheduler_info *sct); //this function sets the default values for the struct


#ifdef	__cplusplus
}
#endif


#endif	/* SYSTEM_SCHEDULER_H */

