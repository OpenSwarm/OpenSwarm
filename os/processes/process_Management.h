/*!
 * \file
 * \ingroup process
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 08 July 2014
 * 
 * \brief It declares functions to manage processes (e.g. task creation, switching, termination)
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

/*! \defgroup process Process Management
 * \brief Functions to create, switch, block, yield, and terminate processes and start critical sections.
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * A process is a basic form to execute functions in OpenSwarm. Due to target device architecture, OpenSwarm does not provide functions to separate memory in pages or segments. 
 * Therefore, all processes are executed in the same memory area. Consequently, Each process can be seen as a single thread where all threads share the same memory space. 
 * A thread is represented by a function with no arguments and no return value. One function can be executed multiple times as individual threads.
 * 
 * OpenSwarm organises processes in four lists (pid sorted):
 *  -#  running list: includes only one element---the process that is executed at that time.
 *  -#  ready list: includes all processes are ready to be executed and will be scheduled according to the scheduling algorithm.
 *  -#  blocked list: includes all processes that are waiting for events to occur.
 *  -#  Zombie list: includes all processes that are about to be terminated but not deleted yet.
 *  
 * \section process_usage Usage
 * The process management is initialised with Sys_Init_Process_Management(void), which generated the System Thread (pid: 0) and initialises all data structures. After initialising, the following functions are available.
 * \subsection process_usercode User code:
 *  -#  Processes are started and terminated with Sys_Start_Process(pFunction function) and Sys_Kill_Process(uint) respectively.
 *  -#  A Process can be yield with Sys_Yield(void) and remains in the ready list. The process can be rescheduled by the scheduler.
 *  -#  A thread/process can be suspended while waiting for arriving events with Sys_Wait_For_Event(uint) and Sys_Wait_For_Condition(uint, pConditionFunction). Processes that are suspended are on the block list and are not rescheduled whilst in it.
 * \subsection process_internal Internal function (shouldn't be used by the user)
 *   \subsubsection process_scheduling Scheduling (functions to decide which process is executed at which time)
 *     Scheduling-related functions can be found in scheduler.h and process_Management.h.
 *       - The executing process can be switched by using Sys_Switch_Process(uint) and  Sys_Switch_to_next_Process(void).
 *       - To implement a new scheduling algorithm, sys_scheduler_info_s, a function to implement the algorithm (void function(void)), and a function to set the default values of the struct (void Sys_Set_Defaults_Info(sys_scheduler_info *)) needs to be implemented (scheduler.h).
 *   \subsubsection process_timer System Timer (timer to start the scheduling, found in system_Timer.h):
 *       -# The System Timer needs to be initialised and started by Sys_Init_SystemTimer(pFunction) and Sys_Start_SystemTimer(void) respectively (these functions are used when the process Management is initialised and started).
 *       -# It can be stopped, continued, and reset by Sys_Stop_SystemTimer(), Sys_Continue_SystemTimer(), and Sys_Reset_SystemTimer() respectively.
 *       -# The timer interrupt can be disabled and enabled by Sys_Disable_TimerInterrupt(void) and Sys_Enable_TimerInterrupt(void).
 *       -# To force a system timer and therefore the scheduling process, Sys_Force_TimerInterrupt() will be executed.
 *   \subsubsection process_event Process Event handling (functions to store/process events with it's subscribed process and add/remove subscriptions) \ref events
 *       - Event subscription to a process can be added and removed by Sys_Add_Event_Subscription(uint, uint, pEventHandlerFunction, pConditionFunction) and Sys_Remove_Event_Subscription(uint, uint, pEventHandlerFunction).
 *       - Removing all subscription to any process of a singe event can be done by Sys_Remove_All_Event_Subscriptions(uint eventID).
 *       - To copy the data of an occurred event to a specific process, Sys_Add_Event_to_Process(uint, uint, void *, uint) can be used.
 *       - All stored data is processed by its registered event handler by  Sys_Execute_All_EventHandler().
 *       - The event data can be cleared with Sys_Clear_EventData(sys_event_data **).
 *
 * \section process_example Example 
 * \code
 * #include "os/system.h"
 * #include "os/events/events.h"
 * #include "os/processes/process_Management.h"
 * 
 * #define WAIT_FOR_ME 0x0F
 * 
 * 
 * void thread(void){//thread definition
 *       while(true){
 *           //do something as a thread
 *           sys_event_data * data = Sys_Wait_For_Event(WAIT_FOR_ME);//blocking
 *           Sys_Clear_EventData(data);
 *       }
 * }
 * 
 * int main(void){
 *  //initialise some global or local variables
 *
 *  int variable;
 * 
 * 	Sys_Init_Kernel();
 *
 *  Sys_Register_Event(WAIT_FOR_ME);
 *      
 *  Sys_Start_Kernel();//OpenSwarm is running now      
 * 	while(1){
 *      if( ... ){//under a certain condition
 *          Sys_Send_Event(WAIT_FOR_ME, &variable, sizeof(int));
 *          //alternatively, you could use Sys_Send_IntEvent(WAIT_FOR_ME, variable);
 *      }
 *      //do something
 * 	}
 * }
 * \endcode
 * 
 * \section process_license License
 * LICENSE: adapted FreeBSD License (see http://openswarm.org/license)\n
 * Copyright (c) 2015, Stefan M. Trenkwalder\n
 * All rights reserved. 
 */


#ifndef SYSTEM_PROCESS_MANAGEMENT_H
#define	SYSTEM_PROCESS_MANAGEMENT_H

#include <stdbool.h>
#include "../definitions.h"
#include "../events/events.h"

#include "scheduler.h"

#ifdef	__cplusplus
extern "C" {
#endif


/*! process default stack size */
#define DEFAULT_PROCESS_STACK_SIZE 200

//############# PROCESS MANAGEMENT ###############################
void Sys_Switch_Process(uint pid); //loads all values into the registers to start the other programm
void Sys_Switch_to_next_Process(void);

inline bool Sys_Start_Process(pFunction function);//create a new process with function as template
inline void Sys_Kill_Process(uint pid);//kills a process
inline void Sys_Kill_Zombies(void);//kills any process makred as zomies
void Sys_Yield(void);//create a new process with function as template

void Sys_Init_Process_Management(void);// initialises the process management and creates a system process

inline unsigned short Sys_Get_Number_Processes(void);//returns the number of processes
inline void Sys_Start_CriticalSection(void); //no scheduling possible within a critical section
inline void Sys_End_CriticalSection(void);

//############# PROCESS EVENT MANAGEMENT ###############################
bool Sys_Add_Event_Subscription(uint pid, uint eventID, pEventHandlerFunction func, pConditionFunction cond);//enables a process to recieve events via event handler
void Sys_Remove_Event_Subscription(uint pid, uint eventID, pEventHandlerFunction func);//process won't be able to recieve events (unique: pid + eventID + func)
void Sys_Remove_All_Event_Subscriptions(uint eventID);//no process can recieve the events eventID
void Sys_Add_Event_to_Process(uint pid, uint eventID, void *data, uint length);//copies the event-data to the PCB
void Sys_Execute_All_EventHandler();//a function to process all event-data that were sent and stored. 
inline void Sys_Clear_EventData(sys_event_data **data);//frees the memory of the data-struct

//############# PROCESS Synchronisation ###############################
inline sys_event_data *Sys_Wait_For_Event(uint eventID); // blocks until eventID occurs (returns pointer to event data)
sys_event_data *Sys_Wait_For_Condition(uint eventID, pConditionFunction function); //blocks until eventID meets the condition (function)


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_PROCESS_MANAGEMENT_H */

