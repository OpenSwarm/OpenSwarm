/*!
 * \file
 * \ingroup process
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 08 July 2014
 * 
 * \brief  It declares functions to manage process lists and related structs
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
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
    
//! Linked list element containing an occurred events
/*!
 It is a single linked list element that stores the id on an occurred event.
*/
typedef struct sys_occurred_event_s{
    uint eventID;/*!< ID of the occured event*/

    struct sys_occurred_event_s *next;/*!< pointer to the next element in the linked list.*/
}sys_occurred_event;

//!  Double linked list element of process event-handlers
/*!
 It is a double linked list containing all information needed to decide if the event-handler should be executed for an occurred event or not. It sores the pointer to the handler the condition function and data.
*/
typedef struct sys_process_event_handler_s{
    uint eventID;/*!< ID of the occured event*/
    pEventHandlerFunction handler;/*!< Pointer to a function which processes occurred events*/
    pConditionFunction condition;/*!< Pointer to  a function which checks if the event-handler should be executed (true) or not (false) */
    sys_event_data *buffered_data;/*!< stores a list of recieved event data that need to be processed */

    struct sys_process_event_handler_s *previous;/*!< pointer to the previous element in the linked list.*/
    struct sys_process_event_handler_s *next;/*!< pointer to the next element in the linked list.*/
}sys_process_event_handler, sys_peh;

//!  Process Control Block contains all data for a single process
/*!
 * It contains all information related to a single process. (including stack pointer, frame pointer, stack, etc. )
*/
typedef struct sys_process_control_block_s{

    uint process_ID;                            /*!< Process identifier */
    uint stackPointer;                        /*!< Stack Pointer Register */
    uint framePointer;                        /*!< Frame Pointer Register*/
    uint stackPointerLimit;                   /*!< Stack Pointer Limit Register*/

    sys_scheduler_info sheduler_info;           /*!< scheduler-specific datastructure */
    sys_process_event_handler *event_register;  /*!< Lists of all events the process is subscribed to */

    uint *process_stack;                      /*!< Pointer to the beginning of the stack */

} sys_process_control_block, sys_pcb;


//!  Double linked list element containing sys_process_control_block
/*!
 It is a double linked list element containing the PCB of a process
*/
typedef struct sys_process_control_block_list_element_s{

    sys_process_control_block pcb;/*!< Process Control Block of a process */

    struct sys_process_control_block_list_element_s* previous;/*!< pointer to the previous PCB */
    struct sys_process_control_block_list_element_s* next;/*!< pointer to the next PCB */
    
} sys_process_control_block_list_element, sys_pcb_list_element;

extern sys_pcb_list_element *sys_ready_processes;/*!< pointer to the ready processes (linked list) */
extern sys_pcb_list_element *sys_running_process;/*!< pointer to the running process */
extern sys_pcb_list_element *sys_blocked_processes;/*!< pointer to the blocked process */
extern sys_pcb_list_element *sys_zombies;/*!< pointer to the zombie process */
extern sys_occurred_event *sys_occurred_events;/*!< pointer to the occurred events */

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

