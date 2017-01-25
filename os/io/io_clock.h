/*!
 * \file
 * \ingroup io
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 28 July 2015
 * 
 * \brief  It declares the system clock that provides a continuous time value (granulation of 1 ms).
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#ifndef SYSTEM_IO_CLOCK_H
#define	SYSTEM_IO_CLOCK_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../definitions.h"
    
inline void Sys_Init_SystemClock(void);
inline void Sys_Deactivate_SystemClock(void);
inline void Sys_Start_SystemClock(void);
inline void Sys_Stop_SystemClock(void);
inline void Sys_Pause_SystemClock(void);
inline void Sys_Continue_SystemClock(void);

#define Sys_Init_SystemTime Sys_Init_SystemClock 
#define Sys_Start_SystemTime Sys_Start_SystemClock
#define Sys_Deactivate_SystemTime Sys_Deactivate_SystemClock
#define Sys_Stop_SystemTime Sys_Stop_SystemClock
#define Sys_Pause_SystemTime Sys_Pause_SystemClock
#define Sys_Continue_SystemTime Sys_Continue_SystemClock

inline uint32 Sys_Get_SystemClock(void);

#define Sys_Get_SystemTime Sys_Get_SystemClock

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_CLOCK_H */

