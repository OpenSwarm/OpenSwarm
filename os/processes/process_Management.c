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
bool Sys_Continue_Pocess(uint eventID, sys_event_data *data, void* pid);

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
 * @return uint returns the number of processes
 */
uint Sys_Get_Number_Processes(){

    return Sys_Get_Number_ReadyProcesses() + Sys_Get_Number_BlockedProcesses();
}

/**
 *
 *  This function counts the number of ready process
 *
 * @return uint returns the number of ready processes
 */
uint Sys_Get_Number_ReadyProcesses(){
    uint n = 0;
    sys_process_control_block_list_element *element;
    
    Sys_Start_AtomicSection();
    element = sys_ready_processes;

    while(element != 0){
        n++;
        element = element->next;
    }
    
    Sys_End_AtomicSection();
    return n;
}

/**
 *
 *  This function counts the number of blocked process
 *
 * @return uint returns the number of blocked processes
 */
uint Sys_Get_Number_BlockedProcesses(){
    uint n = 0;
    sys_process_control_block_list_element *element;
    
    Sys_Start_AtomicSection();
    element = sys_blocked_processes;

    while(element != 0){
        n++;
        element = element->next;
    }
    
    Sys_End_AtomicSection();
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

    Sys_Start_AtomicSection();
    if(pid == sys_running_process->pcb.process_ID){//Do not kill the running program
        Sys_End_AtomicSection();
        Sys_Set_Running_Process_to_Zombie();//kill asynchronously
        return;
    }

    element = Sys_Remove_Process_from_List(pid, &sys_ready_processes);
    if(element != 0){
            element->pcb.sheduler_info.state = SYS_PROCESS_STATE_ZOMBIE;
            Sys_Delete_Process(element);
            Sys_End_AtomicSection();
            return;
    }

    element = Sys_Remove_Process_from_List(pid, &sys_blocked_processes);
    if(element != 0){
            element->pcb.sheduler_info.state = SYS_PROCESS_STATE_ZOMBIE;
            Sys_Delete_Process(element);
            Sys_End_AtomicSection();
            return;
    }
    
    Sys_End_AtomicSection();
}

/**
 *
 * This function puts the running process in the zombie list and switches content to the next ready process
 *
 */
void Sys_Set_Running_Process_to_Zombie(){

    Sys_Start_AtomicSection();
    if(sys_running_process->pcb.process_ID == 0){//system is never a zombie
        Sys_End_AtomicSection();
        return;
    }

    sys_pcb_list_element *process = 0;
    sys_pcb_list_element *previous_process = 0;
    
    sys_running_process->pcb.sheduler_info.state = SYS_PROCESS_STATE_ZOMBIE;

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

    Sys_End_AtomicSection();
    Sys_Force_TimerInterrupt();//this schedules the next  after the prvious process
}

/**
 *
 * This function deletes all proccesses which are marked as zombies.
 *
 */
inline void Sys_Kill_Zombies(){
    sys_pcb_list_element *zombie;
    
    Sys_Start_AtomicSection();
        zombie = sys_zombies;
        sys_zombies = 0;
    Sys_End_AtomicSection();

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
    sys_pcb_list_element *element;
    
    Sys_End_AtomicSection();
    element = sys_ready_processes;

    while(element != 0){//search for the right process
        if(element->pcb.process_ID == pid){//if this is the correct one > write values
            Sys_Switch_Process_HDI(element);
            Sys_End_AtomicSection();
            return;
        }

        element = element->next;
    }
    Sys_End_AtomicSection();
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
 *  Process States (blocking continuing)
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

    if(sys_ready_processes == 0){
        return;
    }
    
    if(sys_ready_processes->next == 0){ 
        return; //no proccess to block
    }

    Sys_Start_AtomicSection();
    sys_pcb_list_element *element = Sys_Remove_Process_from_List(pid, &sys_ready_processes);
    Sys_Insert_Process_to_List(element, &sys_blocked_processes);

    //Event must be registered and process must be subscribed to it
    Sys_Register_Event(eventID);//if already registered -> just returns;
    Sys_Subscribe_to_Event(eventID, &Sys_Continue_Pocess, condition, &(element->pcb.process_ID)); //if already registered -> returns


    if(sys_running_process->pcb.process_ID == pid){//if this is the running process
        // CALL Scheduler
        Sys_End_AtomicSection();
        Sys_Force_TimerInterrupt();
        return;
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
bool Sys_Continue_Pocess(uint eventID, sys_event_data *data, void *ppid){
    if(sys_blocked_processes == 0) return true; //no proccess is blocked .. process must be running

    uint pid = *((uint *) ppid);
    
    Sys_Start_AtomicSection();
    sys_pcb_list_element *element = Sys_Remove_Process_from_List( pid, &sys_blocked_processes);
    Sys_Insert_Process_to_List(element, &sys_ready_processes);
    
    Sys_Clear_EventData( (element->pcb.event) );
    element->pcb.event = Sys_Copy_EventData(data);
    
    Sys_Unsubscribe_Handler(eventID, Sys_Continue_Pocess, ppid);
    
    Sys_End_AtomicSection();
    return true;
}

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

    return sys_running_process->pcb.event;
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
