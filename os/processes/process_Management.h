/*
 * File:   process_Management.h
 * Author: Stefan M. Trenkwalder
 *
 * Created on 08 July 2014, 18:58
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

#ifndef SYSTEM_PROCESS_MANAGEMENT_H
#define	SYSTEM_PROCESS_MANAGEMENT_H

#include <stdbool.h>
#include "../definitions.h"
#include "../events/events.h"

#include "scheduler.h"

#ifdef	__cplusplus
extern "C" {
#endif


/*! process default stack size */
#define DEFAULT_PROCESS_STACK_SIZE 200

//############# PROCESS MANAGEMENT ###############################
void Sys_Switch_Process(uint16 pid); //loads all values into the registers to start the other programm
void Sys_Switch_to_next_Process(void);

inline bool Sys_Start_Process(pFunction function);//create a new process with function as template
inline void Sys_Kill_Process(uint16 pid);//kills a process
inline void Sys_Kill_Zombies(void);//kills any process makred as zomies
void Sys_Yield(void);//create a new process with function as template

void Sys_Init_Process_Management(void);// initialises the process management and creates a system process

inline unsigned short Sys_Get_Number_Processes(void);//returns the number of processes
inline void Sys_Start_CriticalSection(void); //no scheduling possible within a critical section
inline void Sys_End_CriticalSection(void);

//############# PROCESS EVENT MANAGEMENT ###############################
bool Sys_Add_Event_Subscription(uint16 pid, uint16 eventID, pEventHandlerFunction func, pConditionFunction cond);//enables a process to recieve events via event handler
void Sys_Remove_Event_Subscription(uint16 pid, uint16 eventID, pEventHandlerFunction func);//process won't be able to recieve events (unique: pid + eventID + func)
void Sys_Remove_All_Event_Subscriptions(uint16 eventID);//no process can recieve the events eventID
void Sys_Add_Event_to_Process(uint16 pid, uint16 eventID, void *data, uint16 length);//copies the event-data to the PCB
void Sys_Execute_All_EventHandler();//a function to process all event-data that were sent and stored. 
inline void Sys_Clear_EventData(sys_event_data **data);//frees the memory of the data-struct

//############# PROCESS Synchronisation ###############################
inline sys_event_data *Sys_Wait_For_Event(uint16 eventID); // blocks until eventID occurs (returns pointer to event data)
sys_event_data *Sys_Wait_For_Condition(uint16 eventID, pConditionFunction function); //blocks until eventID meets the condition (function)


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_PROCESS_MANAGEMENT_H */

