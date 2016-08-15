/*!
 * \file
 * \ingroup process
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 08 July 2014
 *
 * \brief  It defines functions to manage process lists and related structs
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#include "process_Management.h" 
#include "../platform/e-puck/process_Management_HDI.h"

#include <stdlib.h>

#include "../interrupts.h"
#include "../memory.h"
#include "../definitions.h"

#include <stdbool.h>
#include <stdio.h>

/********************************************************
 *  Private Members
 ********************************************************/

/********************************************************
 *  Global Variables
 ********************************************************/
sys_pcb_list_element *sys_ready_processes = 0;/*!< pointer to the ready processes (linked list) */
sys_pcb_list_element *sys_running_process = 0;/*!< pointer to the running process */
sys_pcb_list_element *sys_blocked_processes = 0;/*!< pointer to the blocked process */
sys_pcb_list_element *sys_zombies = 0;/*!< pointer to the zombie process */

/********************************************************
 ********************************************************
 *****   Code
 ********************************************************
 ********************************************************/

/**
 *
 * This function seaches all elements of a process list and removes the processs pID from it. Note: The element is not deleted. The pointer to it is returned.
 *
 * @param[in] pID the process identifier
 * @param[in,out] **list the process list which has to be seached
 * @return sys_pcb_list_element* the pointer to the removed element
 */
sys_pcb_list_element *Sys_Remove_Process_from_List(uint pID, sys_pcb_list_element **list){

    sys_pcb_list_element *out;
    
    if( (*list) == 0){//empty list
        return 0;
    }
    
    Sys_Start_AtomicSection();
    if((*list)->pcb.process_ID == pID){//is this the PCB with pid
        // remove from list

        out = *list;

        (*list) = out->next;
        (*list)->previous = 0;

        out->previous = 0;
        out->next = 0;

        Sys_End_AtomicSection();
        return out;
    }

    //go through all elements in the list
    //todo: I could do all in one do{}
    sys_pcb_list_element *element = (*list)->next;
    while(element != 0){//as long as there is an element
        if(element->pcb.process_ID == pID){
            //if found -> remove

            element->previous->next = element->next;
            element->next->previous = element->previous;

            element->previous = 0;
            element->next = 0;

            Sys_End_AtomicSection();
            return element;
        }

        element = element->next;
    }

    Sys_End_AtomicSection();
    return 0;
}

/**
 *
 * This function return the pointer to the PCB of process with pid
 *
 * @param[in] pid process ID
 * @return void
 */
inline sys_pcb_list_element *Sys_Find_Process(uint pid){
    sys_pcb_list_element *element;
    
    Sys_Start_AtomicSection();
        element = sys_ready_processes;

        while(element != 0){
            if(element->pcb.process_ID == pid){
                Sys_End_AtomicSection();
                return element;
            }
            element = element->next;
        }

        element = sys_blocked_processes;
        while(element != 0){
            if(element->pcb.process_ID == pid){
                Sys_End_AtomicSection();
                return element;
            }
            element = element->next;
        }
        
    Sys_End_AtomicSection();
    return 0;
}

/**
 *
 * This function deletes container elements. Warning: this function only deletes the process. All the elements which are linked with next are lost in memory, if you haven't take care of that on advance.
 *
 * @param[in] element pointer to the element which should be deleted
 * @return void
 */
void Sys_Delete_Process(sys_pcb_list_element *element){
    Sys_Start_AtomicSection();

    if(element != 0){
        Sys_Free(element->pcb.process_stack);
        element->pcb.process_stack = 0;
        
        Sys_Clear_EventData(element->pcb.event);
        Sys_Free(element);
    }
    Sys_End_AtomicSection();
}

/**
 *
 * This function sets the default values in a sys_process_control_block struct
 *
 * @param[in, out] element This is a pointer to a sys_process_control_block struct
 * @param[in] stacksize This is a uint whch represents the size of the stack which should be allocated for this process. The default value (=0) is in DEFAULT_PROCESS_STACK_SIZE.
 * @return void
 */
inline bool Sys_Set_Defaults_PCB(sys_pcb *element, uint stacksize){
    static uint new_id = 1;
    element->process_ID = new_id++;
    element->interruptPriority = DEFAULT_IRQPRIORITY;
    element->interruptPriorityNesting = 0;

    if(stacksize == 0){//if there is no stack size -> set default value
        stacksize = DEFAULT_PROCESS_STACK_SIZE;
    }

    Sys_Set_Defaults_Info(&element->sheduler_info);// set default values for the scheduler

    element->process_stack = (uint *) Sys_Malloc(stacksize); //create stack for process

    if(element->process_stack == NULL){
        return false;
    }

    element->stackPointer = (uint) element->process_stack;
    element->framePointer = element->stackPointer;
    element->stackPointerLimit = element->stackPointer + stacksize;
    element->event = 0;

    return true;
}

/**
 *
 * This function inserts a process into a process list. Note: The elements are sorted (process ID)
 *
 * @param[in] process the process struct
 * @param[in,out] **list the process list which has to be seached
 * @return void
 */
void Sys_Insert_Process_to_List(sys_pcb_list_element *process, sys_pcb_list_element **list){

    if(process == 0){//no process
        return;
    }

    Sys_Start_AtomicSection();
    process->previous = 0;
    process->next = 0;

    if( *list == 0){//is it the first element
        *list = process;
        Sys_End_AtomicSection();
        return;
    }

    if((*list)->pcb.process_ID == process->pcb.process_ID){
        Sys_End_AtomicSection();
        return;//already exists
    }
    
    if( (*list)->pcb.process_ID >= process->pcb.process_ID){// if first pid is bigger
        //put it on top

        process->next = *list;
        process->previous = 0;
        (*list)->previous = process;

        *list = process;

        Sys_End_AtomicSection();
        return;
    }
    
    //todo: put all into  one do{}
    sys_pcb_list_element *element = *list;
    while(element->next != 0){//as long as there are elements 
        
        if(element->next->pcb.process_ID == process->pcb.process_ID){
            Sys_End_AtomicSection();
            return;//already exists
        }
        
        if(element->next->pcb.process_ID > process->pcb.process_ID){

            process->next = element->next;
            if(process->next != 0){
                process->next->previous = process;
            }
            process->previous = element;
            element->next = process;


            Sys_End_AtomicSection();
            return;
        }

        element = element->next;
    }

    //put it at the end

    process->next = 0;
    element->next = process;
    process->previous = element;

    Sys_End_AtomicSection();
    return;
}

inline uint Sys_GetCurrentIRQPNesting(void){
    if( sys_running_process == 0){
        return 0;
    } 
    
    return sys_running_process->pcb.interruptPriorityNesting;
}

inline uint Sys_GetCurrentIRQPriority(void){
    if( sys_running_process == 0){
        return 0;
    } 
    
    return sys_running_process->pcb.interruptPriority;
}

inline void Sys_SetCurrentIRQPNesting(uint n){
    if( sys_running_process == 0){
        return;
    } 
    
    sys_running_process->pcb.interruptPriorityNesting = n;
}

inline void Sys_SetCurrentIRQPriority(uint irqp){
    if( sys_running_process == 0){
        return;
    } 
    
    sys_running_process->pcb.interruptPriority = irqp; 
}


inline void Sys_IncCurrentIRQPNesting(void){
    if( sys_running_process == 0){
        return;
    } 
    
    sys_running_process->pcb.interruptPriorityNesting++;
}

inline void Sys_DecCurrentIRQPNesting(void){
    if( sys_running_process == 0){
        return;
    } 
    
    sys_running_process->pcb.interruptPriorityNesting--;
}
