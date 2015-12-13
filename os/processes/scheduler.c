/**
 * @file scheduler.c
 * @author  Stefan M. Trenkwalder <stefan.markus.trenkwalder@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Created on 07 July 2014
 *
 * LICENSE: adapted FreeBSD License
 * Copyright (c) 2015, Stefan M. Trenkwalder
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. If this or parts of this source code (as code or binary) is, in any form, used for an commercial product or service (in any form), this product or service must provide a clear notice/message to any user/customer that OpenSwarm was used in this product.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * @section DESCRIPTION
 *
 * This file includes all functions wich are needed to specify a scheduling algorithm
 */

#include "scheduler.h"
#include "process_Management.h"

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
