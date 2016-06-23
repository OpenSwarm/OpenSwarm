/* 
 * File:   control_timer.h
 * Author: stefa
 *
 * Created on 23 June 2016, 14:18
 */

#ifndef CONTROL_TIMER_H
#define	CONTROL_TIMER_H

#ifdef	__cplusplus
extern "C" {
#endif

    
inline void Sys_Init_ControlTimer(void);
inline void Sys_Start_ControlTimer(void);
inline void Sys_Pause_ControlTimer(void);
inline void Sys_Continue_ControlTimer(void);
inline void Sys_Disable_ControlTimerInterrupt(void);
inline void Sys_Enable_ControlTimerInterrupt(void);
inline void Sys_Force_ControlTimerInterrupt(void);

inline void init_behaviour();
inline void start_behaviour();
inline void stop_behaviour();

void ledsOn();
void ledsOff();
#ifdef	__cplusplus
}
#endif

#endif	/* CONTROL_TIMER_H */

