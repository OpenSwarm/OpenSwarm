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
sys_occurred_event *sys_occurred_events = 0;/*!< pointer to the occurred events */

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

    if( (*list) == 0){//empty list
        return 0;
    }

    sys_pcb_list_element *out;
    if((*list)->pcb.process_ID == pID){//is this the PCB with pid
        // remove from list
        Sys_Start_AtomicSection();

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
            Sys_Start_AtomicSection();

            element->previous->next = element->next;
            element->next->previous = element->previous;

            element->previous = 0;
            element->next = 0;

            Sys_End_AtomicSection();
            return element;
        }

        element = element->next;
    }

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
    sys_pcb_list_element *element = sys_ready_processes;

    while(element != 0){
            if(element->pcb.process_ID == pid){
                return element;
            }
            element = element->next;
    }

    element = sys_blocked_processes;
    while(element != 0){
            if(element->pcb.process_ID == pid){
                return element;
            }
            element = element->next;
    }

    return 0;
}

/****** EVENTS: **************************************************************************************************
 *****************************************************************************************************************
 *  All functions which are needed to add/remove/delete events for an process
 *
 *****************************************************************************************************************
 *****************************************************************************************************************/

/**
 *
 * This function searches (sequentially) all event handler for an event (eventID). The list contains a list of eventhandler and this function return the first occurrence of eventID. To search the list entirely, use the function on a list and after resulting an element use the same function on the next element (sublist).
 *
 * @param[in] list      list of event handler
 * @param[in] eventID   The Id of the event which can put the process (PID) back on the ready list
 * @return sys_process_event_handler *          pointer to the next event handler for the event (eventID) in list (0 if not found)
 */
inline sys_process_event_handler *Sys_Next_EventHandler(sys_process_event_handler *list, uint eventID){

    sys_process_event_handler *element = list;

    while(element != 0){
        if(element->eventID == eventID)
            return element;

        element = element->next;
    }

    return 0;
}

/*******************************
 *  Cleaning up
 *******************************/

/**
 * 
 * This function removes subscribed handler function from event-handler list
 *
 * @param[in] eventID   Identifier of the event that has to be removed
 * @param[in] func      pointer to the subscribed handler function
 * @param[in] list      list of event handlers
 * @return sys_process_event_handler *      (New) top of the list (if changed)
 */
//Assumption eventID+func is unique in process eventregister. Note: -1 means all functions
inline sys_process_event_handler *Sys_Remove_Event_from_EventRegister(uint eventID, pEventHandlerFunction func, sys_process_event_handler **list){

    if(list == 0 || *list == 0){//list is empty
        return 0;
    }

    sys_process_event_handler *top = *list;
    if(top->eventID == eventID && ( func == ALL_FUNCTIONS || top->handler == func)){
       sys_process_event_handler *new_top = top->next;
       *list = top->next;

       Sys_Clear_EventData(&top->buffered_data);
       Sys_Free(top);

       return new_top;
    }

    sys_process_event_handler *previous_element = top;
    sys_process_event_handler *element = top->next;

    do{
        if(element->eventID == eventID && ( func == ALL_FUNCTIONS || top->handler == func)){
            previous_element->next = element->next;

            Sys_Clear_EventData(&element->buffered_data);
            Sys_Free(element);

            element = previous_element->next;
            continue;
        }
        previous_element = element;
        element = element->next;
    }while(element == 0);

    return top;
}

/**
 * 
 * This function removes and frees a list of sys_event_data
 *
 * @param[in,out] data   pointer to the event_data (list)
 * @return void
 */
inline void Sys_Clear_EventData(sys_event_data **data){
    sys_event_data *element = *data;
    *data = 0;

    while(element != 0){
           sys_event_data *temp = element;

           Sys_Free(temp->value);
           Sys_Free(temp);

           element = element->next;
       }
}

/**
 * 
 * This function clears and frees all elements of a process. The process is also unsubscribed from any event, because and empty event register cannot handle any events. 
 *
 * @param[in,out] element   pointer to the pcb of the process
 * @return void
 */
inline void Sys_Clear_EventRegister(sys_pcb_list_element *element){
    sys_process_event_handler *event_h = element->pcb.event_register;

    Sys_Unsubscribe_Process(element->pcb.process_ID);

    while(event_h != 0){
        sys_process_event_handler *temp = event_h;
        event_h = event_h->next;

        Sys_Clear_EventData(&temp->buffered_data);
        Sys_Free(temp);
    }
    element->pcb.event_register = 0;
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

    Sys_Clear_EventRegister(element);

    Sys_Free(element->pcb.process_stack);
    element->pcb.process_stack = 0;
    Sys_Free(element);

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
    element->event_register = 0;

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

    process->previous = 0;
    process->next = 0;

    if( *list == 0){//is it the first element
        *list = process;
        return;
    }

    if((*list)->pcb.process_ID == process->pcb.process_ID){
        return;//already exists
    }
    
    if( (*list)->pcb.process_ID >= process->pcb.process_ID){// if first pid is bigger
        //put it on top
        Sys_Start_AtomicSection();

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
            return;//already exists
        }
        
        if(element->next->pcb.process_ID > process->pcb.process_ID){
            Sys_Start_AtomicSection();

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
    Sys_Start_AtomicSection();

    process->next = 0;
    element->next = process;
    process->previous = element;

    Sys_End_AtomicSection();
    return;
}
