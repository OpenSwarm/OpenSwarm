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
    
inline void Sys_Init_Clock(void);
inline void Sys_Init_SystemTime(void);

inline uint32 Sys_Get_SystemTime(void);
inline uint32 Sys_Get_SystemClock(void);
#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_CLOCK_H */

