/*
 * File:   data.h
 * Author: Stefan M. Trenkwalder
 *
 * Created on 08 July 2014, 19:20
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


#ifndef SYSTEM_PROCESS_DATA_H
#define	SYSTEM_PROCESS_DATA_H

#include <stdbool.h>
#include "../../os/definitions.h"
#include "../../os/events/events.h"

#include "scheduler.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
/********************************************************
 *  Struct Declarations
 ********************************************************/
    
//!  List of occured events
/*!
 This struct sores the event ID of an occurred event
*/
typedef struct sys_occured_event_s{
    uint16 eventID;

    struct sys_occured_event_s *next;
}sys_occured_event;

//!  List of process event-handlers
/*!
 This struct sores all information needed to decide if the event-handler is executed for the event (eventID). To store the event data and be executed, a condition has to be met.
*/
typedef struct sys_process_event_handler_s{
    uint16 eventID;
    pEventHandlerFunction handler;/*!< Pointer to function which computes the evnt data */
    pConditionFunction condition;/*!< Pointer to function which checks if the event-handler has to be executed (true) or nor (false) */
    sys_event_data *buffered_data;/*!< stores the data */

    struct sys_process_event_handler_s *previous;
    struct sys_process_event_handler_s *next;
}sys_process_event_handler, sys_peh;

//!  Process Control Block for the processes
/*!
 This struct sores all information of the current state of a process
*/
typedef struct sys_process_control_block_s{

    uint16 process_ID;                                       //    2
    uint16 stackPointer;/*!< Stack Pointer to TOP */
    uint16 framePointer;
    uint16 stackPointerLimit;/*!< Stack Pointer + MAX SIZE*/

    sys_scheduler_info sheduler_info;                               //  + 6
    sys_process_event_handler *event_register;

    uint16 *process_stack;                                  //  + DEFAULT STACK SIZE (200)

} sys_process_control_block, sys_pcb;


//!  Container struct for Process Control Block
/*!
 This struct is a container (linked list) for PCB
*/
typedef struct sys_process_control_block_list_element_s{

    sys_process_control_block pcb;

    struct sys_process_control_block_list_element_s* previous;
    struct sys_process_control_block_list_element_s* next;
    
} sys_process_control_block_list_element, sys_pcb_list_element;

extern sys_pcb_list_element *sys_ready_processes;/*!< pointer to the ready processes (linked list) */
extern sys_pcb_list_element *sys_running_process;/*!< pointer to the running process */
extern sys_pcb_list_element *sys_blocked_processes;/*!< pointer to the blocked process */
extern sys_pcb_list_element *sys_zombies;/*!< pointer to the zombie process */
extern sys_occured_event *sys_occurred_events;/*!< pointer to the occurred events */

/********************************************************
 *  Function Prototypes
 *      Basic Process Management
 ********************************************************/
inline sys_pcb_list_element *Sys_Find_Process(uint16 pid);
sys_pcb_list_element *Sys_Remove_Process_from_List(uint16 pID, sys_pcb_list_element **list);//removes the process (PID) struct from list and returns the pointer to the struct

void Sys_Delete_Process(sys_pcb_list_element *element); //function to delete a container element
inline bool Sys_Set_Defaults_PCB(sys_process_control_block *element, uint16 stacksize);  // function to set the default values of the sys_process_control_block struct

void Sys_Insert_Process_to_List(sys_pcb_list_element *process, sys_pcb_list_element **list);//insers a process into a list of processes (sorted)

/********************************************************
 *  Function Prototypes
 *      Basic Event Management for Processes
 ********************************************************/
inline sys_process_event_handler *Sys_Next_EventHandler(sys_process_event_handler *list, uint16 eventID);
inline void Sys_Clear_EventRegister(sys_pcb_list_element *element); //function to delete all elements in the eventregister
inline void Sys_Clear_EventData(sys_event_data **data);

inline sys_process_event_handler *Sys_Find_EventHandler(sys_process_event_handler *process, uint16 eventID);
inline sys_process_event_handler *Sys_Remove_Event_from_EventRegister(uint16 eventID, pEventHandlerFunction func, sys_process_event_handler **list);

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_PROCESS_MANAGEMENT_H */

