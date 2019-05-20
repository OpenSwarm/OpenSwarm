/*!
 * \file
 * \ingroup process
 * \ingroup hdi
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date{08 July 2014}
 * 
 * \brief Hardware dependent implementations to manage processes (e.g. task swichting)
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#include "process_Management_HDI.h"
#include "../../processes/process_Management.h"

#include <stdlib.h>
#include "system_Timer_HDI.h"

#include "../../interrupts.h"
#include "../../memory.h"
#include "../../definitions.h"


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
void Sys_Init_Process_Management_HDI(){

    sys_pcb_list_ptr  element;
    
    Sys_Start_AtomicSection();
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
        Sys_End_AtomicSection();
        return;//should never happen
    }
    
    Sys_Set_Defaults_Info(&(sys_ready_processes->pcb.sheduler_info));

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
    sys_ready_processes->pcb.event = 0;
    sys_running_process = sys_ready_processes;//it is the running process
    
    Sys_End_AtomicSection();
}

/**
 *
 * This function creates a new sys_process_control_block (in a sys_process_control_block_list_element) which contains all information wich is used to execute this process.
 *
 * @param[in] function This argument points to a function in memory which should be executed as an new task
 */
bool Sys_Start_Process_HDI(pFunction function){
  sys_pcb_list_ptr element;

  Sys_Start_AtomicSection();
  element = (sys_pcb_list_bpointer) Sys_Malloc(sizeof(sys_process_control_block_list_element));//create the root element

  if(element == NULL){
      Sys_End_AtomicSection();
      return false;
  }

  if(!Sys_Set_Defaults_PCB(&(element->pcb),0)){
      Sys_Free(element);//set default values
      Sys_End_AtomicSection();
      return false;
  }
  element->pcb.sheduler_info.priority = SYS_PROCESS_PRIORITY_NORMAL; //this element is the system process
  element->pcb.sheduler_info.state = SYS_PROCESS_STATE_BABY;
  element->previous = 0;
  element->next = 0;//it is the only element
  

  //TODO: add "void Sys_Set_Running_Process_to_Zombie()" to the bottom of the stack -> last thing a process executes is its termination!!!!
  asm volatile (//loads temporarly the new stack and loads the jump address into the stack
          "PUSH W8\n\t"//save used registers
          "PUSH W9\n\t"
          "PUSH W10\n\t"
          "MOV W14, W8\n\t"//save old framepointer
          "MOV W15, W9\n\t"//save old stackpointer
          "MOV SPLIM, W10\n\t"//save old SPLIM
          "MOV %2, W14\n\t"//load new pointers
          "MOV %3, W15\n\t"
          "MOV %4, SPLIM\n\t"
          "PUSH %P5\n\t"//push the jump address into the stack
          "MOV #0x0000, w0\n\t"
          "PUSH w0\n\t"
          "MOV W14, %0\n\t"//save all pointers
          "MOV W15, %1\n\t"
          "MOV W8, W14\n\t"//restore old values
          "MOV W9, W15\n\t"
          "MOV W10, SPLIM\n\t"
          "POP W10\n\t"//restore registers
          "POP W9\n\t"
          "POP W8\n\t"
          : "=rm" (element->pcb.framePointer), "=rm" (element->pcb.stackPointer)
          : "rm" (element->pcb.framePointer), "rm" (element->pcb.stackPointer), "rm" (element->pcb.stackPointerLimit), "r" (function)
          :
          );

  Sys_Insert_Process_to_List(element, &sys_ready_processes);
  
  Sys_End_AtomicSection();
  return true;

}

/**
 *
 * This function switches from sys_running_process to new_process
 *
 * @param[in] new_process pointer to the process which should be executed
 */
void Sys_Switch_Process_HDI(sys_pcb_list_ptr new_process){

    if(new_process == sys_running_process || sys_running_process == 0){//Do I want to switch to the same process??
        return;//How stupid
    }

    Sys_Start_AtomicSection();
    
    //PUSH everything on the stack
    __asm__("PUSH W0\n\t"
            "PUSH W1\n\t"
            "PUSH W2\n\t"
            "PUSH W3\n\t"
            "PUSH W4\n\t"
            "PUSH W5\n\t"
            "PUSH W6\n\t"
            "PUSH W7\n\t"
            "PUSH W8\n\t"
            "PUSH W9\n\t"
            "PUSH W10\n\t"
            "PUSH W11\n\t"
            "PUSH W12\n\t"
            "PUSH W13\n\t"
            "PUSH ACCA\n\t"
            "PUSH ACCB\n\t"
            "PUSH TBLPAG\n\t"
            "PUSH PSVPAG\n\t"
            "PUSH RCOUNT\n\t"
            "PUSH DCOUNT\n\t"
            "PUSH DOSTARTH\n\t"
            "PUSH DOSTARTL\n\t"
            "PUSH DOENDH\n\t"
            "PUSH DOENDL\n\t"
            "PUSH CORCON\n\t"
            "POP.S\n\t"
//            "PUSH SR\n\t"
            "PUSH W0\n\t"
            "PUSH W1\n\t"
            "PUSH W2\n\t"
            "PUSH W3\n\t"
            );

    __asm__(//save the current stack & frame pointer
            "MOV W14, %0\n\t"
            "MOV W15, %1\n\t"
        :"=rm" (sys_running_process->pcb.framePointer), "=rm" (sys_running_process->pcb.stackPointer)
        :
        :
        );
    
    /*
    __asm__ __volatile__ (
            "MOV %0, [%1++]\n\t" //push frame pointer to the stack
            "MOV %1, %0\n\t" //set the framepointer to the TOS
            //"ULNK\n\t" //remove waste from Sys_Change_Stack
            "NOTEMPTY: MOV [W14++],[%1++]\n\t"//copy all values for the current frame into the stack
            "CP W14,W15\n\t"
            "BRA LT, NOTEMPTY\n\t"
            "MOV %0, W14\n\t" //set new stackpointers
            "MOV %1, W15\n\t"
            "MOV %2, SPLIM\n\t"
        : 
        : "r" (new_process->pcb.framePointer), "r" (new_process->pcb.stackPointer), "r" (new_process->pcb.stackPointerLimit) 
        : 
    );*/
    
    __asm__ __volatile__ (
            "MOV %0, [%1++]\n\t" //push frame pointer to the stack
            "MOV %1, %0\n\t" //set the framepointer to the TOS
  "NOTEMPTY: MOV [W14++],[%1++]\n\t"//copy all values for the current frame into the stack
            "CP W14,W15\n\t"
            "BRA LT, NOTEMPTY\n\t"
            "NOP\n\t"
            "PUSH W10\n\t"
            "MOV SPLIM, W10\n\t"
            "CP W10,%2\n\t"
            "BRA GEU, SPLAST\n\t"
            "MOV %2, SPLIM\n\t"
    "SPLAST: POP W10\n\t"
            "MOV %0, W14\n\t" //set new stackpointers
            "MOV %1, W15\n\t"
            "MOV %2, SPLIM\n\t"
        : 
        : "r" (new_process->pcb.framePointer), "r" (new_process->pcb.stackPointer), "r" (new_process->pcb.stackPointerLimit) 
        : 
    );

    sys_running_process->pcb.sheduler_info.state = SYS_PROCESS_STATE_WAITING;

    if(new_process->pcb.sheduler_info.state == SYS_PROCESS_STATE_BABY){//if it is a new process
        Sys_Continue_SystemTimer_HDI();//start the interrupts

        new_process->pcb.sheduler_info.state = SYS_PROCESS_STATE_RUNNING;
        sys_running_process = new_process;

        
        Sys_End_AtomicSection();
        __asm__("ULNK\n\t");
        __asm__("RETFIE\n\t");//go to the new process
    }

    new_process->pcb.sheduler_info.state = SYS_PROCESS_STATE_RUNNING;
    sys_running_process = new_process;

    

    __asm__("ULNK\n\t");//remove the last function waste & restore all registers
    __asm__("POP W3\n\t");
    __asm__("POP W2\n\t");
    __asm__("POP W1\n\t");
    __asm__("POP W0\n\t");
//    __asm__("POP SR\n\t");
    __asm__("PUSH.S\n\t");
    __asm__("POP CORCON\n\t"
            "POP DOENDL\n\t"
            "POP DOENDH\n\t"
            "POP DOSTARTL\n\t"
            "POP DOSTARTH\n\t"
            "POP DCOUNT\n\t"
            "POP RCOUNT\n\t"
            "POP PSVPAG\n\t"
            "POP TBLPAG\n\t"
            "POP ACCB\n\t"
            "POP ACCA\n\t"
            "POP W13\n\t"
            "POP W12\n\t"
            "POP W11\n\t"
            "POP W10\n\t"
            "POP W9\n\t"
            "POP W8\n\t"
            "POP W7\n\t"
            "POP W6\n\t"
            "POP W5\n\t"
            "POP W4\n\t"
            "POP W3\n\t"
            "POP W2\n\t"
            "POP W1\n\t"
            "POP W0\n\t"
            );

    Sys_End_AtomicSection();
    
    //__asm__("ULNK\n\t");//remove all waste from Sys_Save_Running_Process_HDI
    //__asm__("ULNK\n\t");//remove all waste from Sys_Switch_Process_HDI
    //__asm__("RETURN\n\t");//jump directly to process
}
