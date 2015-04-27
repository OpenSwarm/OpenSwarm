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
#include "definitions.h"

#include <p30F6014A.h>
#include <stdbool.h>


typedef struct sys_event_data_s{
    void *value;
    uint16 size;

    struct sys_event_data_s *next;
}sys_event_data;

typedef struct sys_process_event_handler_s{
    uint16 eventID;
    pEventHandler handler;
    sys_event_data *buffered_data;

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

    struct sys_process_control_block_list_element_s* next;

} sys_process_control_block_list_element, sys_pcb_list_element;


typedef struct sys_blocked_process_list_element_s{
    uint16 eventID;
    uint16 occurrences;
    
    sys_pcb_list_element *process;
    
    struct sys_blocked_process_list_element_s *next;
} sys_blocked_pcb_list_element, sys_bpcb_list_element;



void Sys_Delete_Process(sys_pcb_list_element *element); //function to delete a container element

inline bool Sys_Set_Defaults_PCB(sys_process_control_block *element, uint16 stacksize);  // function to set the default values of the sys_process_control_block struct

void Sys_Change_Stack_HDI(uint16 framepointer, uint16 stackpointer, uint16 stackpointerlimit); //this function switches to the new stack
void Sys_Switch_Process_HDI(sys_pcb_list_element *);

inline sys_pcb_list_element *Sys_Find_Process(uint16 pid);
inline sys_process_event_handler *Sys_Find_EventHandler(sys_process_event_handler *process, uint16 eventID);

sys_pcb_list_element *sys_ready_processes = 0;/*!< pointer to the ready processes (single linked list) */
sys_pcb_list_element *sys_running_process = 0;/*!< pointer to the running process */
sys_blocked_pcb_list_element *sys_blocked_processes = 0;/*!< pointer to the blocked process */

/**
 * This function initialises the process management
 *
 * This function initialises the process management and creates the first elements in the linked list
 *
 * @param void
 * @return void
 */
void Sys_Init_Process_Management_HDI(){

    sys_process_control_block_list_element *element;
    while(sys_ready_processes != 0){//for each elements in the list
        element = sys_ready_processes;
        sys_ready_processes = sys_ready_processes->next;

        Sys_Delete_Process(element);//delete that element
    }

    sys_ready_processes = (sys_process_control_block_list_element *) malloc(sizeof(sys_process_control_block_list_element));//create the root element
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
    sys_ready_processes->next = 0;//it is the only element
    sys_ready_processes->pcb.process_ID = 0;
    sys_running_process = sys_ready_processes;//it is the running process
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
            "PUSH W14\n"
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
    sys_process_control_block_list_element *element = sys_ready_processes;

    while(element != 0){//search for the right process

        if(element->pcb.process_ID == pid){//if this is the correct one > write values

            Sys_Switch_Process_HDI(element);
            /*
            Sys_Change_Stack_HDI(element->pcb.framePointer, element->pcb.stackPointer, element->pcb.stackPointerLimit);//change stack to the new stack

            sys_running_process->pcb.sheduler_info.state = SYS_PROCESS_STATE_WAITING;

             if(element->pcb.sheduler_info.state == SYS_PROCESS_STATE_BABY){//if it is a new process
                Sys_Activate_SystemTimer_Interrupt_HDI();//start the interrupts

                element->pcb.sheduler_info.state = SYS_PROCESS_STATE_RUNNING;
                sys_running_process = element;
                __asm__("ULNK\n");
                __asm__("RETFIE\n");//go to the new process
            }

            element->pcb.sheduler_info.state = SYS_PROCESS_STATE_RUNNING;
            sys_running_process = element;
            
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
                    "POP W14\n"
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
          
            //__asm__("MOV #0x0C40, W14\n");
            //__asm__("MOV #0x0C46, W15\n");
            __asm__("ULNK\n");//remove all waste from Sys_Save_Running_Process_HDI
            __asm__("ULNK\n");//remove all waste from Sys_Switch_Process_HDI
            __asm__("RETURN\n");
            */
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
 * This function deletes container elements
 *
 * @param[in] element pointer to the element which should be deleted
 * @return void
 */
void Sys_Delete_Process(sys_process_control_block_list_element *element){
    free(element->pcb.process_stack);
    free(element);
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
inline bool Sys_Set_Defaults_PCB(sys_process_control_block *element, uint16 stacksize){
    sys_process_control_block_list_element *ele = sys_ready_processes;

    static uint16 new_id = 1;
    element->process_ID = new_id++;

    if(stacksize == 0){//if there is no stack size -> set default value
        stacksize = DEFAULT_PROCESS_STACK_SIZE;
    }

    Sys_Set_Defaults_Info(&element->sheduler_info);// set default values for the scheduler

    element->process_stack = (uint16 *) malloc(stacksize); //create stack for process

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
  sys_process_control_block_list_element *element, *ele;

  element = (sys_process_control_block_list_element *) malloc(sizeof(sys_process_control_block_list_element));//create the root element

  if(element == NULL){
      return false;
  }

  if(!Sys_Set_Defaults_PCB(&element->pcb,0)){
      free(element);//set default values
      return false;
  }
  element->pcb.sheduler_info.priority = SYS_PROCESS_PRIORITY_NORMAL; //this element is the system process
  element->pcb.sheduler_info.state = SYS_PROCESS_STATE_BABY;
  element->next = 0;//it is the only element

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

  if(sys_ready_processes == 0){//is it the first programme?
      sys_ready_processes = element;
      return true;
  }

  ele = sys_ready_processes;
  while(ele != 0){//give the process the first free number
      if( ele->next == 0){
         ele->next = element;
         return true;
      }else{
         ele = ele->next;
      }
  }


  
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
    sys_process_control_block_list_element *element = sys_ready_processes->next;
    sys_process_control_block_list_element *ele = sys_ready_processes;

    if(pid == 0){
        return; //DO NOT KILL THE SYSTEM
    }

    while(element != 0){//search for the right process

        if(element->pcb.process_ID == pid){//if this is the correct one > write values

            element->pcb.sheduler_info.state = SYS_PROCESS_STATE_ZOMBIE;

            ele->next = element->next;
            element->next = 0;

            free(element->pcb.process_stack);
            free(element);

            return;

        }

        ele = element;
        element = element->next;
    }

    // Check if the process is in the blocked list
    if(sys_blocked_processes == NULL){
        return;
    }
    
    sys_blocked_pcb_list_element *b_element = sys_blocked_processes->next;
    sys_blocked_pcb_list_element *b_ele = sys_blocked_processes;
    
    if(sys_blocked_processes == 0){
        return;
    }else{
        b_element = sys_blocked_processes->next;
        b_ele = sys_blocked_processes; 
    }

    while(b_element != 0){//search for the right process

        if(b_element->process->pcb.process_ID == pid){//if this is the correct one > write values

            b_element->process->pcb.sheduler_info.state = SYS_PROCESS_STATE_ZOMBIE;

            b_ele->next = b_element->next;
            b_element->next = 0;

            free(b_element->process->pcb.process_stack);
            free(b_element->process);
            free(b_element);

            return;

        }

        b_ele = b_element;
        b_element = b_element->next;
    }
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

    sys_blocked_pcb_list_element *b_element = sys_blocked_processes;

    while(b_element != 0){
        n++;
        b_element = b_element->next;
    }

    return n;

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
    //__asm__("RETURN\n"   );

}


bool Sys_Add_Event_Subscription(uint16 pid, uint16 eventID, pEventHandler func){
    sys_process_control_block_list_element *element = sys_ready_processes;

    while(element != 0){
            if(element->pcb.process_ID == pid){
                sys_process_event_handler *new_event = (sys_process_event_handler *) malloc(sizeof(sys_process_event_handler));
                if(new_event == 0){
                    return false;
                }
                new_event->eventID = eventID;
                new_event->handler = func;
                new_event->next = 0;
                new_event->buffered_data = 0;
            
                if(element->pcb.event_register == 0){
                    element->pcb.event_register = new_event;
                    return true;
                }

                sys_process_event_handler *event_h = element->pcb.event_register;
                while(event_h->next != 0){//add at the end ot the list
                    if(event_h->eventID == eventID){
                        free(new_event);
                        return false;
                    }
                    event_h = event_h->next;
                }

                event_h->next = new_event;
                return true;
            }
            element = element->next;
    }
    
    sys_blocked_pcb_list_element *b_element = sys_blocked_processes;
    while(b_element != 0){
            if(b_element->process->pcb.process_ID == pid){
                sys_process_event_handler *new_event = (sys_process_event_handler *) malloc(sizeof(sys_process_event_handler));
                if(new_event == 0){
                    return false;
                }
                new_event->eventID = eventID;
                new_event->handler = func;
                new_event->next = 0;
                new_event->buffered_data = 0;

                if(b_element->process->pcb.event_register == 0){
                    b_element->process->pcb.event_register = new_event;
                    return true;
                }

                sys_process_event_handler *event_h = b_element->process->pcb.event_register;
                while(event_h->next != 0){//add at the end ot the list
                    if(event_h->eventID == eventID){
                        free(new_event);
                        return false;
                    }
                    event_h = event_h->next;
                }

                event_h->next = new_event;
                return true;
            }
            b_element = b_element->next;
    }
    return false;
}

//////////'######################## TODO
void Sys_Add_Event(uint16 pid, uint16 eventID, void *data, uint16 length){
   
    sys_pcb_list_element *element = Sys_Find_Process(pid);
    if(element == 0){
        return;
    }
    
    sys_process_event_handler *event = Sys_Find_EventHandler(element->pcb.event_register, eventID);
    if(event == 0){
        return;
    }
     
    sys_event_data *e_data = (sys_event_data*) malloc(sizeof(sys_event_data));
    if(e_data == 0){
        return;
    }
    e_data->value = malloc(length);
    if(e_data->value == 0){
        free(e_data);
        return;
    }                        
    memcpy ( data, e_data->value, length );
    e_data->size = length;
    e_data->next = 0;

    if(event->buffered_data == 0){
        event->buffered_data = e_data;
        return;
    }
    
    sys_event_data *set_data = event->buffered_data;
    while(set_data->next != 0){
        set_data = set_data->next;
    }
    set_data->next = e_data;
    return;
}

inline sys_process_event_handler *Sys_Remove_Event_from_EventRegister(uint16 eventID, sys_process_event_handler *list){
    
    if(list == 0){
        return 0;
    }
    
    sys_process_event_handler *top = list;
    if(top->eventID == eventID){
       sys_process_event_handler *new_top = top->next; 
       
       free(top->buffered_data);
       free(top);
       
       return new_top;
    }
    
    sys_process_event_handler *previous_element = list;
    sys_process_event_handler *element = list->next;
    
    do{
        if(element->eventID == eventID){
            previous_element->next = element->next;
            
            free(element->buffered_data);
            free(element);
            
            return top;
        }        
        previous_element = element;
        element = element->next;
    }while(element == 0);

    return top;
}

void Sys_Remove_Event_Subscription(uint16 pid, uint16 eventID){
   
    sys_pcb_list_element *element = Sys_Find_Process(pid);

    if(element != 0){
        element->pcb.event_register = Sys_Remove_Event_from_EventRegister(eventID, element->pcb.event_register);
    }
}


inline sys_pcb_list_element *Sys_Find_Process(uint16 pid){
    sys_pcb_list_element *element = sys_ready_processes;

    while(element != 0){
            if(element->pcb.process_ID == pid){
                return element;
            }
            element = element->next;
    }

    sys_blocked_pcb_list_element *b_element = sys_blocked_processes;
    while(b_element != 0){
            if(b_element->process->pcb.process_ID == pid){
                return b_element->process;
            }
            b_element = b_element->next;
    }
    
    return 0;
}


inline sys_process_event_handler *Sys_Find_EventHandler(sys_process_event_handler *list, uint16 eventID){
    
    sys_process_event_handler *element = list;
    
    while(list != 0){
        if(element->eventID == eventID)
            return element;
        
        element = element->next;
    }
    
    return 0; 
}


void Sys_Remove_All_Event_Subscriptions(uint16 eventID){
    sys_pcb_list_element *process = sys_ready_processes;

    while(process != 0){
        Sys_Remove_Event_from_EventRegister(eventID, process->pcb.event_register);
        process = process->next;
    }
}

void Sys_Switch_Process_HDI(sys_pcb_list_element *new_process){

    Sys_Save_Running_Process_HDI();//save all registers
    Sys_Change_Stack_HDI(new_process->pcb.framePointer, new_process->pcb.stackPointer, new_process->pcb.stackPointerLimit);//change stack to the new stack

    sys_running_process->pcb.sheduler_info.state = SYS_PROCESS_STATE_WAITING;

    if(new_process->pcb.sheduler_info.state == SYS_PROCESS_STATE_BABY){//if it is a new process
        Sys_Activate_SystemTimer_Interrupt_HDI();//start the interrupts

        new_process->pcb.sheduler_info.state = SYS_PROCESS_STATE_RUNNING;
        sys_running_process = new_process;
    
        __asm__("ULNK\n");
        __asm__("RETFIE\n");//go to the new process
    }

    new_process->pcb.sheduler_info.state = SYS_PROCESS_STATE_RUNNING;
    sys_running_process = new_process;
            
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
            "POP W14\n"
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
          
            //__asm__("MOV #0x0C40, W14\n");
            //__asm__("MOV #0x0C46, W15\n");
    __asm__("ULNK\n");//remove all waste from Sys_Save_Running_Process_HDI
    __asm__("ULNK\n");//remove all waste from Sys_Switch_Process_HDI
    __asm__("RETURN\n");//jump directly to process
}