/*!
 * \file
 * \ingroup process
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date{08 July 2014}
 * 
 * \brief  It defines functions to manage processes (e.g. task creation, switching, termination)
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#include "process_Management.h"
#include "../platform/e-puck/process_Management_HDI.h"

#include "data.h"

#include "scheduler.h"
#include "system_Timer.h"

#include "../interrupts.h"
#include "../memory.h"
#include "../definitions.h"


/********************************************************
 *  Private Members
 ********************************************************/

/********************************************************
 *  Function Prototypes
 *      Basic Process Management
 ********************************************************/

void Sys_Block_Process(uint pid, uint eventID, pConditionFunction condition);
bool Sys_Continue_Pocess(uint pid, uint eventID, sys_event_data *data);

void Sys_Set_Running_Process_to_Zombie(); //function to delete a container element

/********************************************************
 ********************************************************
 *****   Code
 ********************************************************
 ********************************************************/

/********************************************************
 *  General Task Management
 ********************************************************/
/**
 *
 * This function initialises the process management and creates the first elements in the linked list
 *
 */
inline void Sys_Init_Process_Management(){
    return Sys_Init_Process_Management_HDI();
}

/**
 *
 *  This function counts the number of process
 *
 */
inline unsigned short Sys_Get_Number_Processes(){
    unsigned short n = 0;
    sys_process_control_block_list_element *element = sys_ready_processes;

    while(element != 0){
        n++;
        element = element->next;
    }

    element = sys_blocked_processes;

    while(element != 0){
        n++;
        element = element->next;
    }

    return n;

}

/**
 *
 * This function creates a new sys_process_control_block (in a sys_process_control_block_list_element) which contains all information wich is used to execute this process.
 *
 * @param[in] function This argument points to a function in memory which should be executed as an new task
 */
inline bool Sys_Start_Process(pFunction function){
    return Sys_Start_Process_HDI(function);
}

/**
 *
 * This function deletes the syss_process_control_block element and stops a process
 *
 * @param[in] pid This argument is the process identifier
 */
void Sys_Kill_Process(uint pid){
    // Check if the process is in the ready list
    sys_pcb_list_element *element;//first is the system

    if(pid == 0){
        return; //DO NOT KILL THE SYSTEM
    }

    if(pid == sys_running_process->pcb.process_ID){//Do not kill the running program
        Sys_Set_Running_Process_to_Zombie();//kill asynchronously
        return;
    }

    element = Sys_Remove_Process_from_List(pid, &sys_ready_processes);
    if(element != 0){
            element->pcb.sheduler_info.state = SYS_PROCESS_STATE_ZOMBIE;
            Sys_Delete_Process(element);
            return;
    }

    element = Sys_Remove_Process_from_List(pid, &sys_blocked_processes);
    if(element != 0){
            element->pcb.sheduler_info.state = SYS_PROCESS_STATE_ZOMBIE;
            Sys_Delete_Process(element);
            return;
    }
}

/**
 *
 * This function puts the running process in the zombie list and switches content to the next ready process
 *
 */
void Sys_Set_Running_Process_to_Zombie(){

    if(sys_running_process->pcb.process_ID == 0){//system is never a zombie
        return;
    }

    sys_pcb_list_element *process = 0;
    sys_pcb_list_element *previous_process = 0;
    
    sys_running_process->pcb.sheduler_info.state = SYS_PROCESS_STATE_ZOMBIE;

    Sys_Unsubscribe_Process(sys_running_process->pcb.process_ID);//unsubscribe this process from all events and clear the event-list

    //remove the element from the list
    if(sys_running_process->previous == 0){
        
        previous_process = sys_running_process->next;
        if(previous_process == 0){//no nnext and no previous element
            goto goZombieMode;
        }
        
        while(previous_process->next != 0){//take the last element
           previous_process =  previous_process->next;
        }
    }else{
        previous_process = sys_running_process->previous;
    }
 
    sys_running_process = previous_process;
    
goZombieMode:    
    process = Sys_Remove_Process_from_List(sys_running_process->pcb.process_ID, &sys_ready_processes);
    Sys_Insert_Process_to_List(process, &sys_zombies);

    Sys_Force_TimerInterrupt();//this schedules the next  after the prvious process
}

/**
 *
 * This function deletes all proccesses which are marked as zombies.
 *
 */
inline void Sys_Kill_Zombies(){
    sys_pcb_list_element *zombie = sys_zombies;

    while(zombie != 0){
        sys_pcb_list_element *deleteme = zombie;
        zombie = zombie->next;
        
        Sys_Delete_Process(deleteme);
    }
}


/**
 *
 * This function loads all values into the registers of a process with the PID
 *
 * @param[in] pid process id
 */
void Sys_Switch_Process(uint pid){
    sys_pcb_list_element *element = sys_ready_processes;

    while(element != 0){//search for the right process
        if(element->pcb.process_ID == pid){//if this is the correct one > write values
            Sys_Switch_Process_HDI(element);
            return;
        }

        element = element->next;
    }
}

/**
 *
 * This function loads all values into the registers of the process which is next in the list.
 *
 */
void Sys_Switch_to_next_Process(){
    if(sys_running_process->next == 0){
        Sys_Switch_Process_HDI(sys_ready_processes);
    }else{
        Sys_Switch_Process_HDI(sys_running_process->next);
    }
}

/**
 *
 * This Function starts a critical section to prevent the task-scheduling during its exectution
 *
 */
inline void Sys_Start_CriticalSection(void){
    Sys_Disable_TimerInterrupt();
}

/**
 *
 * This Function ends a critical section. The task-scheduling can now occure. Note: if a critical section was started once or more, it only takes a single call of this function to end all critical sections.
 *
 */
inline void Sys_End_CriticalSection(void){
    Sys_Enable_TimerInterrupt();
}


/********************************************************
 *  Process States (blocking continuing
 ********************************************************/

/**
 *
 * Puts a process on the blocked list and stops its execution (if it's executed)
 *
 * @param[in] pid       Process ID
 * @param[in] eventID   The Id of the event which can put the process (PID) back on the ready list
 * @param[in] condition the condition under which the process is released
 */
void Sys_Block_Process(uint pid, uint eventID, pConditionFunction condition){

    if(sys_ready_processes == 0 || sys_ready_processes->next == 0) return; //no proccess to block

    Sys_Start_AtomicSection();
    sys_pcb_list_element *element = Sys_Remove_Process_from_List(pid, &sys_ready_processes);
    Sys_Insert_Process_to_List(element, &sys_blocked_processes);

    //Event must be registered and process must be subscribed to it
    Sys_Register_Event(eventID);//if already registered -> just returns;
    Sys_Subscribe_to_Event(eventID, pid, &Sys_Continue_Pocess, condition); //if already registered -> returns


    if(sys_running_process->pcb.process_ID == pid){//if this is the running process
        // CALL Scheduler
        Sys_Force_TimerInterrupt();
    }
    
    Sys_End_AtomicSection();
}

/**
 *
 * Puts a process with the process ID (PID) back on the ready list. Consequently the process can be executed again.
 *
 * @param[in] pid       Process ID
 * @param[in] eventID   Event ID
 * @param[in] data      pointer to the data of the event
 */
bool Sys_Continue_Pocess(uint pid, uint eventID, sys_event_data *data){
    if(sys_blocked_processes == 0) return true; //no proccess is blocked .. process must be running

    Sys_Start_AtomicSection();
    sys_pcb_list_element *element = Sys_Remove_Process_from_List(pid, &sys_blocked_processes);
    Sys_Insert_Process_to_List(element, &sys_ready_processes);
    Sys_End_AtomicSection();

    Sys_Remove_Event_from_EventRegister(eventID, Sys_Continue_Pocess, &(element->pcb.event_register));

    bool exists_another = false;//exists another subscribed event handler of the eventID
    sys_process_event_handler * event = element->pcb.event_register;
    while(event != 0){
        if(event->eventID == eventID){
            exists_another = true;
        }

        event = event->next;
    }

    if(exists_another == false){
        Sys_Unsubscribe_from_Event(eventID, pid);
    }
    
    return true;
}


/****** EVENTS: **************************************************************************************************
 *****************************************************************************************************************
 *  All functions which are needed to add/remove/delete events for an process
 *
 *****************************************************************************************************************
 *****************************************************************************************************************/

/**
 *
 * Puts a process on the blocked list and stops its execution (if it's executed)
 *
 * @param[in] pid       Process ID
 * @param[in] eventID   The Id of the event which can put the process (PID) back on the ready list
 * @param[in] func      The function that handles the event
 * @param[in] cond      The condition under which the handler is executed
 * @return bool         Was the event-handler successfully added?
 */
bool Sys_Add_Event_Subscription(uint pid, uint eventID, pEventHandlerFunction func, pConditionFunction cond){

    if(func == 0){//if nothing handles events, nothing will always handle events -> true
        return true;
    }

    sys_pcb_list_element *element = Sys_Find_Process(pid);

    if(element == 0){//cant find process (pid))
        return false;
    }

    sys_process_event_handler *new_event = (sys_process_event_handler *) Sys_Malloc(sizeof(sys_process_event_handler));
    if(new_event == 0){//no memory
        return false;
    }

    new_event->eventID = eventID;
    new_event->handler = func;
    new_event->condition = cond;
    new_event->next = 0;
    new_event->buffered_data = 0;

    if(element->pcb.event_register != 0 ){
        //add at the end
        sys_process_event_handler *event_h = element->pcb.event_register;

        while(event_h != 0){
            if(event_h->next == 0){
                break;
            }else{
                event_h = event_h->next;
            }
        }

        event_h->next = new_event;
    }else{
        //add to top
        element->pcb.event_register = new_event;
    }

    return true;
}

/**  
 *
 * This function adds the event-data to the local list of the process (pid).
 *
 * @param[in] pid      process identifier
 * @param[in] eventID  event identifier
 * @param[in] data     memory that contains the value of the occurred event
 * @param[in] length   length of the data (bytes)
 */
void Sys_Add_Event_to_Process(uint pid, uint eventID, void *data, uint length){

    sys_pcb_list_element *element = Sys_Find_Process(pid);
    if(element == 0){//no process with pid
        return;
    }

    bool add_event = true;
    sys_occurred_event **o_event = &sys_occurred_events;
    while(*o_event != 0){//check if the event (eventID) already occurred
        if((*o_event)->eventID == eventID){//it already occurred
            add_event = false;
            break;
        }
        o_event = &((*o_event)->next);
    }
    
    if(add_event){//if it hasn't occurred 
    Sys_Start_AtomicSection();
        //add eventID to the list of occurred events
        (*o_event) = Sys_Malloc(sizeof(sys_occurred_event));
       if((*o_event) == 0){
           Sys_End_AtomicSection();
           return; //no memory left
       }

       (*o_event)->eventID = eventID;
       (*o_event)->next = 0;
    Sys_End_AtomicSection();
    }

    //NOW add the data
    sys_process_event_handler *event = element->pcb.event_register;
    while( (event = Sys_Next_EventHandler(event, eventID)) != 0 ){
        
        //check if the condition was met to add the event data
        bool is_condition_met = false;
        if(event->condition != 0){
            is_condition_met = event->condition(data);
        }else{//no condition is always met
            is_condition_met = true;
        }

        if( is_condition_met ){
            sys_event_data *e_data = (sys_event_data*) Sys_Malloc(sizeof(sys_event_data));
            if(e_data == 0){//if malloc fails .. exit
                return;
            }

            //create the struct that holds the data
            if(length != 0){//if there is data
                e_data->value = Sys_Malloc(length);
                if(e_data->value == 0){//if malloc fails .. exit
                    Sys_Free(e_data);
                    return;
                }

                Sys_Memcpy(data, e_data->value, length);
            }else{
                e_data->value = 0;
            }
            e_data->size = length;
            e_data->next = 0;

            //add the struct to the end of the buffered_data
            if(event->buffered_data == 0){
                event->buffered_data = e_data;
            }else{
                sys_event_data *set_data = event->buffered_data;
                while(set_data->next != 0){
                    set_data = set_data->next;
                }
                set_data->next = e_data;
            }

            event = event->next;
        }
    }
}

/**
 * 
 * This function executes all event handlers and processes stored event data. First it checks the list of occurred events and then it executes all event handlers of these events
 *
 * @param[in] eventID  event identifier
 * @param[in] elements list of processes
 */
//handler has to clean up the data!!!
//TODO: if handler == 0 remove it from list
inline void Sys_Execute_Events_in_ProcessList(uint eventID, sys_pcb_list_element *elements){
    sys_pcb_list_element *list = elements;
    while(list != 0){//assuming there are less processes then events
        sys_process_event_handler *event = list->pcb.event_register;
        while(event != 0){
            if(event->eventID == eventID){
                if(event->handler != 0){
                    event->handler(list->pcb.process_ID,eventID,event->buffered_data);
                }
                Sys_Clear_EventData(&(event->buffered_data));
                //TODO: buffered data is a linked list -> if more elements stored -> mem leak
                event->buffered_data = 0;
            }

            event = event->next;
        }

        list = list->next;
    }
}

/**
 * 
 * This function executes all event handlers and processes stored event data. First it checks the list of occurred events and then it executes all event handlers of these events
 *
 */
inline void Sys_Execute_All_EventHandler(){
    sys_occurred_event *o_event = sys_occurred_events;
    sys_occurred_events = 0;

    while(o_event != 0){//assuming there are less processes then events
        
        Sys_Execute_Events_in_ProcessList(o_event->eventID, sys_ready_processes);
        Sys_Execute_Events_in_ProcessList(o_event->eventID, sys_blocked_processes);

        
        sys_occurred_event *occured_event = o_event;
        o_event = o_event->next;
        
        occured_event->next = 0;
        Sys_Free(occured_event);
    }
}

/**
 * 
 * This function starts the execution of the event handler and resets the execution time of the process
 *
 */
void Sys_Interprocess_EventHandling(){
    Sys_Execute_All_EventHandler();
    Sys_Reset_SystemTimer();//to ensure that the following process has the full execution time.
}

/*******************************
 *  Cleaning up
 *******************************/

/**
 * 
 * This function removes all subscriptions of any process  to event (eventID)
 *  *
 * @param[in] eventID   Identifier of the event that has to be removed
 */
void Sys_Remove_All_Event_Subscriptions(uint eventID){
    sys_pcb_list_element *process = sys_ready_processes;

    while(process != 0){//go through all processes
        Sys_Remove_Event_from_EventRegister(eventID, ALL_FUNCTIONS, &(process->pcb.event_register) );
        process = process->next;
    }
}

/**
 * 
 * This function removes subscribed handler function for process (pid) to event (eventID)
 *
 * @param[in] pid       Identifier of the process
 * @param[in] eventID   Identifier of the event that has to be removed
 * @param[in] func      pointer to the subscribed handler function
 */
void Sys_Remove_Event_Subscription(uint pid, uint eventID, pEventHandlerFunction func){

    sys_pcb_list_element *element = Sys_Find_Process(pid);

    if(element != 0){
        Sys_Remove_Event_from_EventRegister(eventID, func, &(element->pcb.event_register) );
    }
}

//////////'######################## TODO: multiple event handler for 1 process & event

/*******************************
 *  Blocking processes while waiting of events
 *******************************/

/**
 * 
 * This function blocks the current process while waiting for an event that sends data which meet the condition. 
 *
 * @param[in] eventID   Identifier of the event that need to occur
 * @param[in] function  Pointer to the function that represents the condition function (return true if condition is met and continues the process). If function = 0 .. condition is always met.
 * @return sys_event_data *  Pointer to the event data struct that contains the values carried by the event
 */
sys_event_data *Sys_Wait_For_Condition(uint eventID, pConditionFunction function){

    Sys_Block_Process(sys_running_process->pcb.process_ID, eventID, function);
    
    //This is only executed if process continued
    //now get return value (tansmited event data)
    sys_process_event_handler *event = sys_running_process->pcb.event_register;
    sys_process_event_handler *prev_event = event;
    while(event != 0){
        if(event->eventID == eventID && event->condition == function){
            void *data = event->buffered_data;

            if(prev_event == event){
                sys_running_process->pcb.event_register = event->next;
            }else{
                prev_event->next = event->next;
            }
            event->next = 0;
            Sys_Free(event);//deletes event handler but leaves the data (return value)

            return data;
        }
        prev_event = event;
        event = event->next;
    }

    return 0;
}

/**
 * 
 * This function blocks the current process and waits for the occurrence of event (eventID).
 *
 * @param eventID 	ID of the event
 * @return sys_event_data* Pointer to the event data struct that contains the values carried by the event
 */
inline sys_event_data *Sys_Wait_For_Event(uint eventID){
    return Sys_Wait_For_Condition(eventID, 0);
}

/**
 * 
 * This function blocks the current process and let the next process be executed.
 *
 */
void Sys_Yield(){
    if( sys_running_process == sys_ready_processes && sys_ready_processes->next == 0){
        return;
    }
    
    Sys_Force_TimerInterrupt();
}
