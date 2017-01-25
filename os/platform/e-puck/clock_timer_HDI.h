/* 
 * File:   control_timer.h
 * Author: stefa
 *
 * Created on 23 June 2016, 14:18
 */

#ifndef CLOCK_TIMER_H
#define	CLOCK_TIMER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../../definitions.h"
    
inline void Sys_Init_Clock(void);
inline void Sys_Start_Clock(void);
inline void Sys_Stop_Clock(void);
inline void Sys_Pause_Clock(void);
inline void Sys_Continue_Clock(void);
inline void Sys_Disable_ClockInterrupt(void);
inline void Sys_Enable_ClockInterrupt(void);
inline void Sys_Force_ClockInterrupt(void);

void Sys_Register_TickFunction(pFunction func);

#ifdef	__cplusplus
}
#endif

#endif	/* CONTROL_TIMER_H */

