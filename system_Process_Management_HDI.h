/* 
 * File:   system_Process_Management.h
 * Author: Stefan
 *
 * Created on 08 July 2014, 18:58
 */

#ifndef SYSTEM_PROCESS_MANAGEMENT_H
#define	SYSTEM_PROCESS_MANAGEMENT_H

#include <stdbool.h>
#include "definitions.h"
#include "system_Events.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*! process default stack size */
#define DEFAULT_PROCESS_STACK_SIZE 200

void Sys_Switch_Process(uint16 pid); //loads all values into the registers to start the other programm
void Sys_Switch_to_next_Process();

bool Sys_Start_Process_HDI(pFunction function);//create a new process with function as template
inline void Sys_Kill_Process(uint16 pid);//kills a process


void Sys_Init_Process_Management_HDI();// initialises the process management and creates a system process

inline unsigned short Sys_Get_Number_Processes();//returns the number of processes

bool Sys_Add_Event_Subscription(uint16 pid, uint16 eventID, pEventHandler func);
void Sys_Remove_Event_Subscription(uint16 pid, uint16 eventID);
void Sys_Remove_All_Event_Subscriptions(uint16 eventID);
void Sys_Add_Event(uint16 pid, uint16 eventID, void *data, uint16 length);


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_PROCESS_MANAGEMENT_H */

