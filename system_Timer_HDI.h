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

inline void Sys_Stop_SystemTimer_HDI();
inline void Sys_Continue_SystemTimer_HDI();

inline void Sys_Disable_TimerInterrupt_HDI(void); //no scheduling possible within a critical section
inline void Sys_Enable_TimerInterrupt_HDI(void);   //

inline void Sys_Force_TimerInterrupt_HDI(void); //forces an execution of the Timer1 Interrupt routine

inline void Sys_Reset_SystemTimer_HDI();

#ifdef	__cplusplus
}
#endif
#endif	/* SYSTEMTIMER_H */

