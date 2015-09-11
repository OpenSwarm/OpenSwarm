/**
 * @file system_Process_Management.c
 * @author  Stefan M. Trenkwalder <stefan.markus.trenkwalder@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This file includes all functions wich are needed to manage processes (e.g. task swichting)
 */

#include "system_Process_Management_HDI.h"

#include <stdlib.h>
#include <string.h>
#include "system_Scheduler.h"
#include "system_Timer_HDI.h"
#include "system_Interrupts.h"
#include "system_Memory.h"
#include "definitions.h"

#include <p30F6014A.h>
#include <stdbool.h>
#include <stdio.h>


typedef struct sys_occured_event_s{
    uint16 eventID;

    struct sys_occured_event_s *next;
}sys_occured_event;


typedef struct sys_process_event_handler_s{
    uint16 eventID;
    pEventHandlerFunction handler;/*!< Pointer to function which computes the evnt data */
    pConditionFunction condition;
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
    uint16 stackPointer;/*!< Stack Pointer */
    uint16 framePointer;
    uint16 stackPointerLimit;/*!< Stack Pointer */

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

/*
typedef struct sys_blocked_process_list_element_s{
    //uint16 eventID;//not needed
    //uint16 occurrences;//not needed
    
    sys_pcb_list_element *process;
    
    struct sys_blocked_process_list_element_s *next;
} sys_blocked_pcb_list_element, sys_bpcb_list_element;
*/

inline bool Sys_Set_Defaults_PCB(sys_process_control_block *element, uint16 stacksize);  // function to set the default values of the sys_process_control_block struct

void Sys_Change_Stack_HDI(uint16 framepointer, uint16 stackpointer, uint16 stackpointerlimit); //this function switches to the new stack
void Sys_Switch_Process_HDI(sys_pcb_list_element *);

inline sys_pcb_list_element *Sys_Find_Process(uint16 pid);
void Sys_Insert_Process_to_List(sys_pcb_list_element *process, sys_pcb_list_element **list);//insers a process into a list of processes (sorted)
sys_pcb_list_element *Sys_Remove_Process_from_List(uint16 pID, sys_pcb_list_element **list);//removes the process (PID) struct from list and returns the pointer to the struct

void Sys_Block_Process(uint16 pid, uint16 eventID, pConditionFunction condition);
bool Sys_Continue_Pocess(uint16 pid, uint16 eventID, sys_event_data *data);

void Sys_Set_Running_Process_to_Zombie(); //function to delete a container element
void Sys_Delete_Process(sys_pcb_list_element *element); //function to delete a container element


// EVENTS:
inline sys_process_event_handler *Sys_Next_EventHandler(sys_process_event_handler *list, uint16 eventID);
inline void Sys_Clear_EventRegister(sys_pcb_list_element *element); //function to delete all elements in the eventregister
inline void Sys_Clear_EventData(sys_event_data **data);

inline sys_process_event_handler *Sys_Find_EventHandler(sys_process_event_handler *process, uint16 eventID);
inline sys_process_event_handler *Sys_Remove_Event_from_EventRegister(uint16 eventID, pEventHandlerFunction func, sys_process_event_handler **list);

//###### START & END PROCESSES
sys_pcb_list_element *sys_ready_processes = 0;/*!< pointer to the ready processes (single linked list) */
sys_pcb_list_element *sys_running_process = 0;/*!< pointer to the running process */
sys_pcb_list_element *sys_blocked_processes = 0;/*!< pointer to the blocked process */
sys_pcb_list_element *sys_zombies = 0;/*!< pointer to the zombie process */

//###### List of all occured events
sys_occured_event *sys_occured_events = 0;

/**
 * This function initialises the process management
 *
 * This function initialises the process management and creates the first elements in the linked list
 *
 * @param void
 * @return void
 */
void Sys_Init_Process_Management_HDI(){

    sys_pcb_list_element *element;
    while(sys_ready_processes != 0){//for each elements in the list
        element = sys_ready_processes;
        sys_ready_processes = sys_ready_processes->next;

        Sys_Delete_Process(element);//delete that element
    }

    while(sys_blocked_processes != 0){//for each elements in the list
        element = sys_blocked_processes;
        sys_blocked_processes = sys_blocked_processes->next;

        Sys_Delete_Process(element);//delete that element
    }
    sys_running_process = 0;

    sys_ready_processes = (sys_pcb_list_element *) Sys_Malloc(sizeof(sys_pcb_list_element));//create the root element
    if(sys_ready_processes == 0){
        return;//should never happen
    }
    
    Sys_Set_Defaults_Info(&sys_ready_processes->pcb.sheduler_info);

    sys_ready_processes->pcb.process_stack = 0; //create stack for process

     __asm__(//read stackpointer and framepointer
                "MOV W14, %0\n\t"
                "MOV W15, %1\n\t"
                "MOV SPLIM, %2\n\t"
                :"=rm" (sys_ready_processes->pcb.framePointer), "=rm" (sys_ready_processes->pcb.stackPointer), "=rm" (sys_ready_processes->pcb.stackPointerLimit)
                :
                :
                );

    sys_ready_processes->pcb.sheduler_info.priority = SYS_PROCESS_PRIORITY_SYSTEM; //this element is the system process
    sys_ready_processes->previous = 0;//it is the first element
    sys_ready_processes->next = 0;//it is the only element
    sys_ready_processes->pcb.process_ID = 0;
    sys_ready_processes->pcb.event_register = 0;
    sys_running_process = sys_ready_processes;//it is the running process
}

/**
 * This function counts the number of process
 *
 *  This function counts the number of process
 *
 * @param[in] pid This argument is the process identifier
 * @return void
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
 * This function sets default values to the sys_process_control_block struct
 *
 * This function sets the default values in a sys_process_control_block struct
 *
 * @param[in, out] element This is a pointer to a sys_process_control_block struct
 * @param[in] stacksize This is a uint16 whch represents the size of the stack which should be allocated for this process. The default value (=0) is in DEFAULT_PROCESS_STACK_SIZE.
 * @return void
 */
inline bool Sys_Set_Defaults_PCB(sys_pcb *element, uint16 stacksize){
    static uint16 new_id = 1;
    element->process_ID = new_id++;

    if(stacksize == 0){//if there is no stack size -> set default value
        stacksize = DEFAULT_PROCESS_STACK_SIZE;
    }

    Sys_Set_Defaults_Info(&element->sheduler_info);// set default values for the scheduler

    element->process_stack = (uint16 *) Sys_Malloc(stacksize); //create stack for process

    if(element->process_stack == NULL){
        return false;
    }

    element->stackPointer = (uint16) element->process_stack;
    element->framePointer = element->stackPointer;
    element->stackPointerLimit = element->stackPointer + stacksize;
    element->event_register = 0;

    return true;
}

/**
 * This function creates a new sys_process_control_block and add all needed info
 *
 * This function creates a new sys_process_control_block (in a sys_process_control_block_list_element) which contains all information wich is used to execute this process.
 *
 * @param[in] function This argument points to a function in memory which should be executed as an new task
 * @return void
 */
bool Sys_Start_Process_HDI(pFunction function){
  sys_process_control_block_list_element *element;

  element = (sys_process_control_block_list_element *) Sys_Malloc(sizeof(sys_process_control_block_list_element));//create the root element

  if(element == NULL){
      return false;
  }

  if(!Sys_Set_Defaults_PCB(&element->pcb,0)){
      Sys_Free(element);//set default values
      return false;
  }
  element->pcb.sheduler_info.priority = SYS_PROCESS_PRIORITY_NORMAL; //this element is the system process
  element->pcb.sheduler_info.state = SYS_PROCESS_STATE_BABY;
  element->previous = 0;
  element->next = 0;//it is the only element

  //TODO: add "void Sys_Set_Running_Process_to_Zombie()" to the bottom of the stack -> last thing a process executes is its termination!!!!
  __asm__(//loads temporarly the new stack and loads the jump address into the stack
          "PUSH W8\n"//save used registers
          "PUSH W9\n"
          "PUSH W10\n"
          "MOV W14, W8\n"//save old framepointer
          "MOV W15, W9\n"//save old stackpointer
          "MOV SPLIM, W10\n"//save old SPLIM
          "MOV %2, W14\n"//load new pointers
          "MOV %3, W15\n"
          "MOV %4, SPLIM\n"
          "PUSH %P5\n\t"//push the jump address into the stack
          "MOV #0x0000, w0\n\t"
          "PUSH w0\n\t"
          "MOV W14, %0\n\t"//save all pointers
          "MOV W15, %1\n\t"
          "MOV W8, W14\n"//restore old values
          "MOV W9, W15\n"
          "MOV W10, SPLIM\n"
          "POP W10\n"//restore registers
          "POP W9\n"
          "POP W8\n"
          : "=rm" (element->pcb.framePointer), "=rm" (element->pcb.stackPointer)
          : "rm" (element->pcb.framePointer), "rm" (element->pcb.stackPointer), "rm" (element->pcb.stackPointerLimit), "r" (function)
          :
          );

  Sys_Insert_Process_to_List(element, &sys_ready_processes);
  return true;

}

/**
 * This function kills a process
 *
 * This function deletes the syss_process_control_block element and stops a process
 *
 * @param[in] pid This argument is the process identifier
 * @return void
 */
void Sys_Kill_Process(uint16 pid){
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

void Sys_Set_Running_Process_to_Zombie(){

    if(sys_running_process->pcb.process_ID == 0){//system is never a zombie
        return;
    }

    sys_running_process->pcb.sheduler_info.state = SYS_PROCESS_STATE_ZOMBIE;

    Sys_Unsubscribe_Process(sys_running_process->pcb.process_ID);

    sys_pcb_list_element *previous_process;
    if(sys_running_process->previous == 0){
        previous_process = sys_running_process;
        while(previous_process != 0){//take the last element
           previous_process =  previous_process->next;
        }
    }else{
        previous_process = sys_running_process->previous;
    }
    
    sys_pcb_list_element *process = Sys_Remove_Process_from_List(sys_running_process->pcb.process_ID, &sys_ready_processes);
    Sys_Insert_Process_to_List(process, &sys_zombies);

    sys_running_process = previous_process;
    Sys_Force_TimerInterrupt_HDI();//this schedules the next  after the prvious process
}

/**
 * This function kills all zombie process
 *
 * This function deletes all proccesses which are marked as zombies.
 *
 * @param  void
 * @return void
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
 * This function deletes container elements
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
 * This function stores all registers and information of the running process into the coresponding struct
 *
 * This function stores all registers and information of the running process into the coresponding struct
 *
 * @param void
 * @return void
 */
inline void Sys_Save_Running_Process_HDI(){

    if(sys_running_process == 0){//if there is no running process (ERROR)
            return;//don't know what to do
    }

    Sys_Start_AtomicSection();

    //PUSH all on the stack
    __asm__(
            "PUSH SR\n"
            "PUSH W0\n"
            "PUSH W1\n"
            "PUSH W2\n"
            "PUSH W3\n"
            "PUSH W4\n"
            "PUSH W5\n"
            "PUSH W6\n"
            "PUSH W7\n"
            "PUSH W8\n"
            "PUSH W9\n"
            "PUSH W10\n"
            "PUSH W11\n"
            "PUSH W12\n"
            "PUSH W13\n"
            "PUSH ACCA\n"
            "PUSH ACCB\n"
            "PUSH TBLPAG\n"
            "PUSH PSVPAG\n"
            "PUSH RCOUNT\n"
            "PUSH DCOUNT\n"
            "PUSH DOSTARTH\n"
            "PUSH DOSTARTL\n"
            "PUSH DOENDH\n"
            "PUSH DOENDL\n"
            "PUSH CORCON\n"
            "POP.S\n"
            "PUSH SR\n"
            "PUSH W0\n"
            "PUSH W1\n"
            "PUSH W2\n"
            "PUSH W3\n"
            );

            __asm__(
                "MOV W14, %0\n\t"
                "MOV W15, %1\n\t"
                :"=rm" (sys_running_process->pcb.framePointer), "=rm" (sys_running_process->pcb.stackPointer)
                :
                :
                );

    Sys_End_AtomicSection();
}

/**
 * This function changes stackpointers to the new stack
 *
 *  This function changes stackpointers to the new stack
 *
 * @param[in] fp FramePointer address
 * @param[in] sp StackPointer address
 * @param[in] lm StackPointer Limit
 * @return void
 */
void Sys_Change_Stack_HDI(unsigned short fp/*W0*/, unsigned short sp/*W1*/, unsigned short lm/*W2*/){

    Sys_Start_AtomicSection();
    
    __asm__("MOV W0, [W1++]\n\t"   );//push frame pointer to the stack
    __asm__("MOV W1, W0\n\t"   );//set the framepointer to the TOS
    __asm__("ULNK\n\t"   );//remove waste from Sys_Change_Stack

    __asm__("NOTEMPTY: MOV [W14++],[W1++]\n\t"//copy all values for the underlying function into the stack
                      "CP W14,W15\n\t"
                      "BRA LT, NOTEMPTY\n\t"   );

    __asm__("MOV W0, W14\n\t"   );//set new stackpointers
    __asm__("MOV W1, W15\n\t"   );
    __asm__("MOV W2, SPLIM\n"   );
    __asm__("PUSH W14\n\t"   );//save framepointer
    __asm__("MOV W15, W14\n\t"   );//set framepointer to local stack
    __asm__("PUSH W0\n\t"   );//push all local variables into the stack
    __asm__("PUSH W1\n\t"   );
    __asm__("PUSH W2\n\t"   );
    
    Sys_End_AtomicSection();
    //__asm__("RETURN\n"   );

}

void Sys_Switch_Process_HDI(sys_pcb_list_element *new_process){

    if(new_process == sys_running_process){
        return;
    }

    Sys_Save_Running_Process_HDI();//save all registers

    Sys_Change_Stack_HDI(new_process->pcb.framePointer, new_process->pcb.stackPointer, new_process->pcb.stackPointerLimit);//change stack to the new stack


    sys_running_process->pcb.sheduler_info.state = SYS_PROCESS_STATE_WAITING;

    if(new_process->pcb.sheduler_info.state == SYS_PROCESS_STATE_BABY){//if it is a new process
        Sys_Continue_SystemTimer_HDI();//start the interrupts

        new_process->pcb.sheduler_info.state = SYS_PROCESS_STATE_RUNNING;
        sys_running_process = new_process;

        __asm__("ULNK\n");
        __asm__("RETFIE\n");//go to the new process
    }

    new_process->pcb.sheduler_info.state = SYS_PROCESS_STATE_RUNNING;
    sys_running_process = new_process;

    Sys_Start_AtomicSection();

    __asm__("ULNK\n");//remove the last function waste & restore all registers
    __asm__("POP W3\n");
    __asm__("POP W2\n");
    __asm__("POP W1\n");
    __asm__("POP W0\n");
    __asm__("POP SR\n");
    __asm__("PUSH.S\n");
    __asm__("POP CORCON\n"
            "POP DOENDL\n"
            "POP DOENDH\n"
            "POP DOSTARTL\n"
            "POP DOSTARTH\n"
            "POP DCOUNT\n"
            "POP RCOUNT\n"
            "POP PSVPAG\n"
            "POP TBLPAG\n"
            "POP ACCB\n"
            "POP ACCA\n"
            "POP W13\n"
            "POP W12\n"
            "POP W11\n"
            "POP W10\n"
            "POP W9\n"
            "POP W8\n"
            "POP W7\n"
            "POP W6\n"
            "POP W5\n"
            "POP W4\n"
            "POP W3\n"
            "POP W2\n"
            "POP W1\n"
            "POP W0\n"
            "POP SR\n"
            );

    Sys_End_AtomicSection();
            //__asm__("MOV #0x0C40, W14\n");
            //__asm__("MOV #0x0C46, W15\n");
    __asm__("ULNK\n");//remove all waste from Sys_Save_Running_Process_HDI
    __asm__("ULNK\n");//remove all waste from Sys_Switch_Process_HDI
    __asm__("RETURN\n");//jump directly to process
}

/**
 * This function loads all values into the registers of a process with the PID
 *
 * This function loads all values into the registers of a process with the PID
 *
 * @param[in] pid process id
 * @return void
 */
void Sys_Switch_Process(uint16 pid){
    sys_pcb_list_element *element = sys_ready_processes;

    while(element != 0){//search for the right process
        if(element->pcb.process_ID == pid){//if this is the correct one > write values
            Sys_Switch_Process_HDI(element);
            return;
        }

        element = element->next;
    }
}

void Sys_Switch_to_next_Process(){
    if(sys_running_process->next == 0){
        Sys_Switch_Process_HDI(sys_ready_processes);
    }else{
        Sys_Switch_Process_HDI(sys_running_process->next);
    }
}

/**
 * This function deletes container elements
 *
 * This function deletes container elements. Warning, this function only deletes the process. All the elements which are linked with next are lost in memory, if you haven't take care of that on advance.
 *
 * @param[in] element pointer to the element which should be deleted
 * @return void
 */
/*
void Sys_Delete_Blocked_Process(sys_blocked_pcb_list_element *element){
    Sys_Delete_Process(element->process);
    element->process = 0;
    
    free(element);
}
*/

/**
 * This function removed a pcb element from the list
 *
 * This function seaches all elements of a process list and removes the processs pID from it. Note: The element is not deleted. The pointer to it is returned.
 *
 * @param[in] pID the process identifier
 * @param[in,out] **list the process list which has to be seached
 * @return sys_pcb_list_element* the pointer to the removed element
 */
sys_pcb_list_element *Sys_Remove_Process_from_List(uint16 pID, sys_pcb_list_element **list){

    if( (*list) == 0){//empty list
        return 0;
    }

    sys_pcb_list_element *out;
    if((*list)->pcb.process_ID == pID){
        Sys_Start_AtomicSection();

        out = *list;

        (*list) = out->next;
        (*list)->previous = 0;

        out->previous = 0;
        out->next = 0;

        Sys_End_AtomicSection();
        return out;
    }


    sys_pcb_list_element *element = (*list)->next;
    while(element != 0){
        if(element->pcb.process_ID == pID){
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
 * This function inserts a process into a process list.
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

    if( *list == 0){
        *list = process;
        return;
    }

    if( (*list)->pcb.process_ID > process->pcb.process_ID){
        Sys_Start_AtomicSection();

        process->next = *list;
        process->previous = 0;
        (*list)->previous = process;

        *list = process;

        Sys_End_AtomicSection();
        return;
    }


    sys_pcb_list_element *element = *list;
    while(element->next != 0){
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

    Sys_Start_AtomicSection();

    process->next = 0;
    element->next = process;
    process->previous = element;

    Sys_End_AtomicSection();
    return;
}

inline sys_pcb_list_element *Sys_Find_Process(uint16 pid){
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

/**
 * Starts a critical section
 *
 * This Function starts a critical section to prevent the task-scheduling during its exectution
 *
 * @param void
 * @return void
 */
inline void Sys_Start_CriticalSection(void){
    Sys_Disable_TimerInterrupt_HDI();
}

/**
 * Ends a critical section
 *
 * This Function ends a critical section. The task-scheduling can now occure. Note: if a critical section was started once or more, it only takes a single call of this function to end all critical sections.
 *
 * @param void
 * @return void
 */
inline void Sys_End_CriticalSection(void){
    Sys_Enable_TimerInterrupt_HDI();
}


//###### UN-/BLOCK PROCESSES
/**
 * Puts a process on the blocked list and stops its execution (if it's executed)
 *
 * Puts a process on the blocked list and stops its execution (if it's executed)
 *
 * @param[in] pid       Process ID
 * @param[in] eventID   The Id of the event which can put the process (PID) back on the ready list
 * @param[in] n         The number of occurences of the event (EventID) which have to occure that the process (PID) gets put on the ready list
 * @return void
 */
void Sys_Block_Process(uint16 pid, uint16 eventID, pConditionFunction condition){

    if(sys_ready_processes == 0 || sys_ready_processes->next == 0) return; //no proccess to block

    Sys_Start_AtomicSection();
    sys_pcb_list_element *element = Sys_Remove_Process_from_List(pid, &sys_ready_processes);
    Sys_Insert_Process_to_List(element, &sys_blocked_processes);

    //Event must be registered and process must be subscribed to it
    Sys_Register_Event(eventID);//if already registered -> just returns;
    Sys_Subscribe_to_Event(eventID, pid, &Sys_Continue_Pocess, condition); //if already registered -> returns


    if(sys_running_process->pcb.process_ID == pid){//if this is the running process
        // CALL Scheduler
        Sys_Force_TimerInterrupt_HDI();
    }
    
    Sys_End_AtomicSection();
}


/**
 * Puts a process on the ready list
 *
 * Puts a process with the process ID (PID) back on the ready list. Consequently the process can be executed again.
 *
 * @param[in] pid       Process ID
 * @return void
 */
bool Sys_Continue_Pocess(uint16 pid, uint16 eventID, sys_event_data *data){
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
 *  All functions wich are needed to add/remove/delete events for an process
 *
 *
 *****************************************************************************************************************/
bool Sys_Add_Event_Subscription(uint16 pid, uint16 eventID, pEventHandlerFunction func, pConditionFunction cond){

    if(func == 0){
        return true;
    }

    sys_pcb_list_element *element = Sys_Find_Process(pid);

    if(element == 0){
        return false;
    }

    sys_process_event_handler *new_event = (sys_process_event_handler *) Sys_Malloc(sizeof(sys_process_event_handler));
    if(new_event == 0){
        return false;
    }

    new_event->eventID = eventID;
    new_event->handler = func;
    new_event->condition = cond;
    new_event->next = 0;
    new_event->buffered_data = 0;

    if(element->pcb.event_register != 0 ){
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
        element->pcb.event_register = new_event;
    }

    return true;
}


inline sys_process_event_handler *Sys_Next_EventHandler(sys_process_event_handler *list, uint16 eventID){

    sys_process_event_handler *element = list;

    while(element != 0){
        if(element->eventID == eventID)
            return element;

        element = element->next;
    }

    return 0;
}

void Sys_Add_Event_to_Process(uint16 pid, uint16 eventID, void *data, uint16 length){

    sys_pcb_list_element *element = Sys_Find_Process(pid);
    if(element == 0){//no process with pid
        return;
    }

    sys_occured_event **o_event = &sys_occured_events;
    while(*o_event != 0){
        o_event = &((*o_event)->next);
    }
    
    Sys_Start_AtomicSection();
        (*o_event) = Sys_Malloc(sizeof(sys_occured_event));
       if((*o_event) == 0){
           Sys_End_AtomicSection();
           return; //no memory left
       }

       (*o_event)->eventID = eventID;
       (*o_event)->next = 0;
    Sys_End_AtomicSection();
    //add eventID to the list of occured events

    sys_process_event_handler *event = element->pcb.event_register;
    while( (event = Sys_Next_EventHandler(event, eventID)) != 0 ){
        bool is_condition_met = false;
        if(event->condition != 0){
            is_condition_met = event->condition(data);
        }else{//non condition is always met
            is_condition_met = true;
        }

        if( is_condition_met ){
            sys_event_data *e_data = (sys_event_data*) Sys_Malloc(sizeof(sys_event_data));
            if(e_data == 0){//if malloc fails .. exit
                return;
            }

            if(length != 0){
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

//handler has to clean up the data!!!
//TODO: if handler == 0 remove it from list
inline void Sys_Execute_Events_in_ProcessList(uint16 eventID, sys_pcb_list_element *elements){
    sys_pcb_list_element *list = elements;
    while(list != 0){//assuming there are less processes then events
        sys_process_event_handler *event = list->pcb.event_register;
        while(event != 0){
            if(event->eventID == eventID){
                if(event->handler != 0){
                    event->handler(list->pcb.process_ID,eventID,event->buffered_data);
                }
                Sys_Clear_EventData(&(event->buffered_data));
                event->buffered_data = 0;
            }

            event = event->next;
        }

        list = list->next;
    }
}

inline void Sys_Execute_All_EventHandler(){
    sys_occured_event *o_event = sys_occured_events;
    sys_occured_events = 0;

    while(o_event != 0){//assuming there are less processes then events
        Sys_Execute_Events_in_ProcessList(o_event->eventID, sys_ready_processes);
        Sys_Execute_Events_in_ProcessList(o_event->eventID, sys_blocked_processes);

        
        sys_occured_event *occured_event = o_event;
        o_event = o_event->next;

        Sys_Free(occured_event);
    }
}


void Sys_Interprocess_EventHandling(){
    Sys_Execute_All_EventHandler();
    Sys_Reset_SystemTimer_HDI();//to ensure that the following process has the full execution time.
}



void Sys_Remove_All_Event_Subscriptions(uint16 eventID){
    sys_pcb_list_element *process = sys_ready_processes;

    while(process != 0){
        Sys_Remove_Event_from_EventRegister(eventID, ALL_FUNCTIONS, &(process->pcb.event_register) );
        process = process->next;
    }
}

void Sys_Remove_Event_Subscription(uint16 pid, uint16 eventID, pEventHandlerFunction func){

    sys_pcb_list_element *element = Sys_Find_Process(pid);

    if(element != 0){
        Sys_Remove_Event_from_EventRegister(eventID, func, &(element->pcb.event_register) );
    }
}

//Assumption eventID+func is unique in process eventregister. Note: -1 means all functions
inline sys_process_event_handler *Sys_Remove_Event_from_EventRegister(uint16 eventID, pEventHandlerFunction func, sys_process_event_handler **list){

    if(list == 0 || *list == 0){
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

















//////////'######################## TODO: multiple event handler for 1 process & event









/*
bool Sys_Has_Process_Subscription(uint16 pid, uint16 eventID){
    sys_pcb_list_element *process = Sys_Find_Process(pid);
    if(process == 0){
        return true;
    }

    sys_process_event_handler *event_h = Sys_Find_EventHandler(process->pcb.event_register, eventID);
    if(event_h == 0){
        return false;
    }

    return true;
}
*/

sys_event_data *Sys_Wait_For_Condition(uint16 eventID, pConditionFunction function){

    Sys_Block_Process(sys_running_process->pcb.process_ID, eventID, function);

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

inline sys_event_data *Sys_Wait_For_Event(uint16 eventID){
    return Sys_Wait_For_Condition(eventID, 0);
}
