/*!
 * \file
 * \ingroup process
 * \ingroup hdi
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * \version 1.0
 *
 * \date 07 July 2014
 * 
 * \brief  Hardware dependent implementations to initialise, configure and run the system Timer
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 */


#ifndef SYSTEMTIMER_H
#define	SYSTEMTIMER_H

#include "../../../os/definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif

extern pFunction sys_process_scheduler; //points to the task scheduling algorithm

void Sys_Init_SystemTimer_HDI(pFunction);   //initialises the system timer and sets a scheduling algorithm
void Sys_Start_SystemTimer_HDI(void);       //starts the system timer (to do periodic scheduling

inline void Sys_Stop_SystemTimer_HDI();
inline void Sys_Continue_SystemTimer_HDI();

inline void Sys_Disable_TimerInterrupt_HDI(void); //no scheduling possible within a critical section
inline void Sys_Enable_TimerInterrupt_HDI(void);   //

inline void Sys_Force_TimerInterrupt_HDI(void); //forces an execution of the Timer1 Interrupt routine

inline void Sys_Reset_SystemTimer_HDI();

inline void Sys_todo_SystemTimer(); //this is only an inline functions to process the same in T1 interrupt and ALT-T1 interrupt

#ifdef	__cplusplus
}
#endif
#endif	/* SYSTEMTIMER_H */

