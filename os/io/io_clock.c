/*!
 * \file
 * \ingroup io
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 28 July 2015
 * 
 * \brief  It defines the system clock that provides a continuous time value (granulation of 1 ms).
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#include "io.h"
#include "io_clock.h"

#include "e-puck/io_HDI.h"

#include "../events/events.h"

static uint32 sys_clock = 0; /*!< counter which is system clock */
    
void Sys_SystemClock_Counter(void);

/**
 *
 * This function initialises the system clock which is in principle a counter that inicates passed milli seconds.
 *
 */
inline void Sys_Init_Clock(){
    Sys_Register_Event(SYS_EVENT_1ms_CLOCK); //event that occurs every milli second
    Sys_Register_IOHandler(Sys_SystemClock_Counter); 
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
 * This function calculates the system clock tick and increases the counter if a millisecond passed.
 *
 */
void Sys_SystemClock_Counter(){
    
    static uint counter = 0;
    
    if( counter < STEPS_PER_MILISECOND){
        counter++;
        return;
    }
    Sys_Send_Event(SYS_EVENT_1ms_CLOCK, &sys_clock, sizeof(sys_clock));
    counter = 0;
    sys_clock++;
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
