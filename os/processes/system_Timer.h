/*!
 * \file
 * \ingroup process
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date{07 July 2014}
 * 
 * \brief It declares functions to initialise, configure and run the system Timer
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#ifndef SYSTEMTIMER_H
#define	SYSTEMTIMER_H

#include "../../os/definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif


void Sys_Init_SystemTimer(pFunction);   //initialises the system timer and sets a scheduling algorithm
void Sys_Start_SystemTimer(void);       //starts the system timer (to do periodic scheduling

inline void Sys_Stop_SystemTimer();
inline void Sys_Continue_SystemTimer();

inline void Sys_Disable_TimerInterrupt(void); //no scheduling possible within a critical section
inline void Sys_Enable_TimerInterrupt(void);   //

inline void Sys_Force_TimerInterrupt(void); //forces an execution of the Timer1 Interrupt routine

inline void Sys_Reset_SystemTimer();

inline void Sys_todo_SystemTimer();

#ifdef	__cplusplus
}
#endif
#endif	/* SYSTEMTIMER_H */

