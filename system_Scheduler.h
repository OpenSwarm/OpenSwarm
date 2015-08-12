/* 
 * File:   system_Scheduler.h
 * Author: Stefan
 *
 * Created on 07 July 2014, 17:44
 */

#ifndef SYSTEM_SCHEDULER_H
#define	SYSTEM_SCHEDULER_H

#ifdef	__cplusplus
extern "C" {
#endif

/*! process state values */
#define SYS_PROCESS_STATE_BABY 0xBABE
#define SYS_PROCESS_STATE_RUNNING 0xFFFF
#define SYS_PROCESS_STATE_BLOCKED 0xBCED
#define SYS_PROCESS_STATE_WAITING 0x5555
#define SYS_PROCESS_STATE_ZOMBIE 0xDEAD

/*! process priority values */
#define SYS_PROCESS_PRIORITY_SYSTEM 0xFFFF
#define SYS_PROCESS_PRIORITY_HIGH 0x0FFF
#define SYS_PROCESS_PRIORITY_NORMAL 0x00FF
#define SYS_PROCESS_PRIORITY_LOW 0x000F

//!  The scheduling information for each process
/*!
 This struct defines all values wich are needed for the scheduling algorithm
*/
typedef struct sys_scheduler_info_s{
    unsigned short state;       /*!< Process state information */
    unsigned short priority;    /*!< process priority level */
}sys_scheduler_info;


void Sys_Scheduler_RoundRobin(void); //the scheduling algorithm
inline void Sys_Set_Defaults_Info(sys_scheduler_info *sct); //this function sets the default values for the struct


#ifdef	__cplusplus
}
#endif


#endif	/* SYSTEM_SCHEDULER_H */

