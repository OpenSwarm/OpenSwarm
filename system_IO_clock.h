/* 
 * File:   system_IO_clock.h
 * Author: strenkwalder
 *
 * Created on 11 September 2015, 13:13
 */

#ifndef SYSTEM_IO_CLOCK_H
#define	SYSTEM_IO_CLOCK_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "definitions.h"
    
inline void Sys_Init_Clock(void);
inline void Sys_Init_SystemTime(void);

inline uint32 Sys_Get_SystemTime(void);
inline uint32 Sys_Get_SystemClock(void);
#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_CLOCK_H */

