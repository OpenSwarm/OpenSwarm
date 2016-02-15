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


#include <p30F6014A.h>
#include "library/motor_led/e_epuck_ports.h"



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
 *
 * This function creates a new sys_process_control_block (in a sys_process_control_block_list_element) which contains all information wich is used to execute this process.
 *
 * @param[in] function This argument points to a function in memory which should be executed as an new task
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
 *
 * This function stores all registers and information of the running process into the corresponding struct
 *
 */
inline void Sys_Save_Running_Process_HDI(){

    if(sys_running_process == 0){//if there is no running process (ERROR)
            return;//don't know what to do
    }

    Sys_Start_AtomicSection();

    //PUSH everything on the stack
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
 *
 *  This function changes stackpointers to the new stack
 *
 * @param[in] fp FramePointer address
 * @param[in] sp StackPointer address
 * @param[in] lm StackPointer Limit
 */
void Sys_Change_Stack_HDI(unsigned short fp/*W0*/, unsigned short sp/*W1*/, unsigned short lm/*W2*/){

    Sys_Start_AtomicSection();
    

    __asm__ __volatile__ (
            "MOV %0, [%1++]\n\t" //push frame pointer to the stack
            "MOV %1, %0\n\t" //set the framepointer to the TOS
            "ULNK\n\t" //remove waste from Sys_Change_Stack
            "NOTEMPTY: MOV [W14++],[%1++]\n\t"//copy all values for the underlying function into the stack
            "CP W14,W15\n\t"
            "BRA LT, NOTEMPTY\n\t"
            "MOV %0, W14\n\t" //set new stackpointers
            "MOV %1, W15\n\t"
            "MOV %2, SPLIM\n"
            "PUSH W14\n\t" //save framepointer
            "MOV W15, W14\n\t" //set framepointer to local stack
            "PUSH %0\n\t" //push all local variables into the stack
            "PUSH %1\n\t"
            "PUSH %2\n\t" 
        : 
        : "r" (fp), "r" (sp), "r" (lm) 
        : 
    );
    
    Sys_End_AtomicSection();
    //__asm__("RETURN\n"   );

}

/**
 *
 * This function switches from sys_running_process to new_process
 *
 * @param[in] new_process pointer to the process which should be executed
 */
void Sys_Switch_Process_HDI(sys_pcb_list_element *new_process){

    if(new_process == sys_running_process){//Do I want to switch to the same process??
        return;//How stupid
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
    
    __asm__("ULNK\n");//remove all waste from Sys_Save_Running_Process_HDI
    __asm__("ULNK\n");//remove all waste from Sys_Switch_Process_HDI
    __asm__("RETURN\n");//jump directly to process
}
