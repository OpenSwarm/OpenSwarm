/* 
 * File:   system_Timer_HDI.h
 * Author: Stefan
 *
 * Created on 07 July 2014, 14:15
 *
 * This file includes all hardware dependent functions, which are nesessary to initialise, configure and run the system Timer
 */

#ifndef SYSTEMTIMER_H
#define	SYSTEMTIMER_H

#include "definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif


void Sys_Init_SystemTimer_HDI(pFunction);   //initialises the system timer and sets a scheduling algorithm
void Sys_Start_SystemTimer_HDI(void);       //starts the system timer (to do periodic scheduling

inline void Sys_Deactivate_SystemTimer_Interrupt_HDI(void); //deactivate the periodic interrupts
inline void Sys_Activate_SystemTimer_Interrupt_HDI(void);   //activates them again

#ifdef	__cplusplus
}
#endif
#endif	/* SYSTEMTIMER_H */

