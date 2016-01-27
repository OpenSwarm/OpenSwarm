/*!
 * \file
 * \ingroup process
 * \ingroup hdi
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date{08 July 2014}
 * 
 * \brief Hardware dependent implementations to manage processes (e.g. task swichting)
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */


#ifndef SYSTEM_PROCESS_MANAGEMENT_HDI_H
#define	SYSTEM_PROCESS_MANAGEMENT_HDI_H


#include "../../../os/processes/data.h"

#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif


//############# PROCESS MANAGEMENT ###############################
    
void Sys_Init_Process_Management_HDI(void);// initialises the process management and creates a system process
bool Sys_Start_Process_HDI(pFunction function);

inline void Sys_Save_Running_Process_HDI(void);
void Sys_Change_Stack_HDI(unsigned short fp/*W0*/, unsigned short sp/*W1*/, unsigned short lm/*W2*/);

void Sys_Switch_Process_HDI(sys_pcb_list_element *new_process);

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_PROCESS_MANAGEMENT_H */

