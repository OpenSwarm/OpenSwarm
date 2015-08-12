/**
 * @file system_Scheduler.c
 * @author  Stefan M. Trenkwalder <stefan.markus.trenkwalder@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This file includes all functions wich are needed to specify a scheduling algorithm
 */

#include "system_Scheduler.h"
#include "system_Process_Management_HDI.h"

#include "HDI_epuck_ports.h"



static unsigned short called_Scheduler = 0;

/**
 * This function represents the Schedling algorithm
 *
 * This function shows the implementation of the RoundRobin Scheduling algorithm
 *
 * @param void
 * @return void
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
 * This function sets default values to the scheduling struct
 *
 * This function sets the default values in a sys_scheduler_info struct
 *
 * @param[in, out] sct This is a pointer to a sys_scheduler_info struct
 * @return void
 */
inline void Sys_Set_Defaults_Info(sys_scheduler_info *sct){
    sct->priority = 1;
    sct->state = SYS_PROCESS_STATE_WAITING;
}
