/*!
 * \file
 * \ingroup io
 * \ingroup epuck
 * \ingroup hdi
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 10 August 2015
 * 
 * \brief  Hardware dependent implementations to start and stop the I/O timer.  This timer executes IO functions periodically.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */


#ifndef SYSTEM_IO_HDI_H
#define	SYSTEM_IO_HDI_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../../../os/definitions.h"

#define STEPS_PER_SECOND 10000
#define STEPS_PER_MILISECOND 10
    
/**
 * @brief Linked list element containing IO Handler function pointers 
 * 
 * It is a single linked list element containing a function pointer to an IO hander.
 */
typedef struct sys_periodical_IOHandler_s {
    pFunction function;
    
    struct sys_periodical_IOHandler_s *next;
} sys_periodical_IOHandler, sys_pIOHandler;

inline void Sys_Init_IOTimer_HDI();
inline void Sys_Start_IOTimer_HDI();
inline void Sys_IOTimer_code_HDI();

extern sys_periodical_IOHandler *sys_iohandlers;


inline void Sys_Stop_IOTimer_HDI(void);
inline void Sys_Continue_IOTimer_HDI(void);
inline void Sys_Reset_IOTimer_HDI(void);
inline void Sys_Disable_IOTimerInterrupt_HDI(void);
inline void Sys_Enable_IOTimerInterrupt_HDI(void);
inline void Sys_Force_IOTimerInterrupt_HDI(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_HDI_H */

