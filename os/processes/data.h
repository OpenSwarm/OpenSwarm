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

//!  Process Control Block contains all data for a single process
/*!
 * It contains all information related to a single process. (including stack pointer, frame pointer, stack, etc. )
*/
typedef struct sys_process_control_block_s{

    uint process_ID;                            /*!< Process identifier */
    uint stackPointer;                        /*!< Stack Pointer Register */
    uint framePointer;                        /*!< Frame Pointer Register*/
    uint stackPointerLimit;                   /*!< Stack Pointer Limit Register*/
    
    sint interruptPriority;                   /*!< process-specific Interrupt Priority*/
    sint interruptPriorityNesting;            /*!< process-specific interrupt nesting level (= How often was #Sys_Start_AtomicSection - #Sys_Stop_AtomicSection called)*/

    sys_scheduler_info sheduler_info;           /*!< scheduler-specific datastructure */
    sys_event_data *event;
    
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

sint Sys_GetCurrentIRQPNesting(void);
sint Sys_GetCurrentIRQPriority(void);
void Sys_SetCurrentIRQPNesting(sint n);
void Sys_SetCurrentIRQPriority(sint irqp);

void Sys_IncCurrentIRQPNesting(void);
void Sys_DecCurrentIRQPNesting(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_PROCESS_MANAGEMENT_H */

