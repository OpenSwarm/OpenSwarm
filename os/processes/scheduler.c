/*!
 * \file
 * \ingroup process
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date{07 July 2014}
 * 
 * \brief It defines functions to specify a scheduling algorithm
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */


#include "scheduler.h"
#include "process_Management.h"

static unsigned short called_Scheduler = 0;

/**
 *
 * This function shows the implementation of the RoundRobin Scheduling algorithm
 *
 */
void Sys_Scheduler_RoundRobin(void){
    //TODO write round robin algorithm

    //TEST: every second switch the led
    //if(i >= 100){
        called_Scheduler++;
        //s = (s+1)%(n) + 1;
        Sys_Switch_to_next_Process();
        /*
    }else{
        i++;
    }*/
}


/**
 *
 * This function sets the default values in a sys_scheduler_info struct
 *
 * @param[in, out] sct This is a pointer to a sys_scheduler_info struct
 */
inline void Sys_Set_Defaults_Info(sys_scheduler_info *sct){
    sct->priority = 1;
    sct->state = SYS_PROCESS_STATE_WAITING;
}
