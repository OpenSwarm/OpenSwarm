/*!
 * \file
 * \ingroup io
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 28 July 2015
 * 
 * \brief  It defines the system clock that provides a continuous time value (granulation of 10 ms).
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#include "io.h"
#include "io_clock.h"

#include "../events/events.h"

#ifdef EPUCK_USED
    #include "e-puck/clock_timer_HDI.h"
#endif

static uint32 sys_clock = 0; /*!< counter which is system clock */
    
void Sys_SystemClock_Counter(void);

/**
 *
 * This function initialises the system clock which is a counter that increments every 10ms.
 *
 */
inline void Sys_Init_SystemClock(){
    Sys_Register_Event(SYS_EVENT_10ms_CLOCK); //event that occurs every milli second
    Sys_Register_TickFunction(Sys_SystemClock_Counter); 
    
    Sys_Init_Clock();
}

/**
 *
 * Renaming of the function Sys_Init_Clock().
 * 
 */
inline void Sys_Init_SystemTime(){
    Sys_Init_Clock();
}

/**
 *
 * This function starts the system clock which is a counter that increments every 10ms.
 *
 */
inline void Sys_Start_SystemClock(){
    Sys_Start_Clock();
}

/**
 *
 * Renaming of the function Sys_Start_SystemClock().
 * 
 */
inline void Sys_Start_SystemTime(){
    Sys_Start_SystemClock();
}

/**
 *
 * This function pauses the system clock.
 *
 */
inline void Sys_Pause_SystemClock(void){
    Sys_Pause_Clock();
}

/**
 *
 * Renaming of the function Sys_Pause_SystemClock().
 * 
 */
inline void Sys_Pause_SystemTime(void){
    Sys_Pause_SystemClock();
}

/**
 *
 * This function continues the counting of the system clock.
 *
 */
inline void Sys_Continue_SystemClock(void){
    Sys_Continue_Clock();
}

/**
 *
 * Renaming of the function Sys_Continue_SystemClock().
 * 
 */
inline void Sys_Continue_SystemTim(void){
    Sys_Continue_SystemClock();
}

/**
 *
 * This function calculates the system clock tick and increases the counter if a millisecond passed.
 *
 */
void Sys_SystemClock_Counter(){
    
    sys_clock++;
    
    Sys_Send_Event(SYS_EVENT_10ms_CLOCK, &sys_clock, sizeof(sys_clock));
}

/**
 *
 * Renaming of the function Sys_Get_SystemClock().
 * 
 * @return time that has passed since OpenSwarm was started (uint32)
 */
inline uint32 Sys_Get_SystemTime(){
    return Sys_Get_SystemClock();
}

/**
 *
 * returns the system clock/time in milliseconds
 * 
 * @return uint32  time that has passed since OpenSwarm was started
 */
inline uint32 Sys_Get_SystemClock(){
    return sys_clock;
}

/**
 *
 * returns the system clock/time in milliseconds
 * 
 * @return uint32  time that has passed since OpenSwarm was started
 */
inline void Sys_Set_SystemClock(uint32 time){
    sys_clock = time;
}
