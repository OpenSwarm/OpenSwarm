/*!
 * \file
 * \ingroup process
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date{07 July 2014}
 * 
 * \brief It declares functions to specify a scheduling algorithm
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#ifndef SYSTEM_SCHEDULER_H
#define	SYSTEM_SCHEDULER_H

#include "../definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif

/*! process state values */
#define SYS_PROCESS_STATE_BABY 0xBABE       /*!< State to indicate that a process is created but not yet ready to be executed */
#define SYS_PROCESS_STATE_RUNNING 0xFFFF    /*!< State to indicate that a process is executed */
#define SYS_PROCESS_STATE_BLOCKED 0xBCED    /*!< State to indicate that a process is blocked and waits till an event occurs */
#define SYS_PROCESS_STATE_WAITING 0x5555    /*!< State to indicate that a process is waiting to be executed*/
#define SYS_PROCESS_STATE_ZOMBIE 0xDEAD     /*!< State to indicate that a process is about to be deleted*/

/*! process priority values */
#define SYS_PROCESS_PRIORITY_SYSTEM 0xFFFF    /*!< Process priority: System = highest */
#define SYS_PROCESS_PRIORITY_HIGH 0x0FFF
#define SYS_PROCESS_PRIORITY_NORMAL 0x00FF
#define SYS_PROCESS_PRIORITY_LOW 0x000F

//!  The scheduling information for each process
/*!
 * This struct contains all values that are needed for the scheduling algorithm. For instance, if priority based round robin sheduling is used, the process priorities should be stored within this struct.
 * This struced is designed to be reimplemented by the user, if required. 
*/
typedef struct sys_scheduler_info_s{
    uint state;       /*!< Process state information */
    uint priority;    /*!< process priority level */
}sys_scheduler_info;


void Sys_Scheduler_RoundRobin(void); //the scheduling algorithm
inline void Sys_Set_Defaults_Info(sys_scheduler_info *sct); //this function sets the default values for the struct


#ifdef	__cplusplus
}
#endif


#endif	/* SYSTEM_SCHEDULER_H */

