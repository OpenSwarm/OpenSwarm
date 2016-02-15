/*! \file
 * \ingroup base
 * \brief defines functions to initialise and start OpenSwarm.
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 *  \version   	1.0
 *  \date      	2015
 *  \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 * \n In short, Openswarm can be executed as shown in the following example
 * \code
 * #include "os/system.h"
 * 
 * int main(void){
 *      //initialise some global or local variables
 *
 * 	Sys_Init_Kernel();
 *
 *      //do some preperation before executing OpenSwarm and user applications
 *      
 *      Sys_Start_Kernel();      
 * 	while(1){
 *         //do nothing
 * 	}
 * }
 * \endcode
 */

#include "definitions.h"

#include "system.h"          /* variables/params used by system.c             */
#include "processes/system_Timer.h"    /* functions to start the periodic timer interval */
#include "processes/scheduler.h"    /* the implementation of the scheduler */
#include "processes/process_Management.h"

#include "platform/e-puck/library/motor_led/e_init_port.h"


#include "io/io.h"
#include "io/io_clock.h"
    
#ifdef EPUCK_USED
#ifdef SYS_MOTOR_USED
#include "io/e-puck/motors.h"
#endif
#ifdef SYS_UART1_USED
#include "io/e-puck/uart.h"
#endif
#ifdef SYS_REMOTECONTROL_USED
#include "io/e-puck/remoteControl.h"
#endif
#ifdef SYS_CAMERA_USED
#include "io/e-puck/camera.h"
#endif
#endif

/**
 *
 * This Function sets the system Timer (Timer0) and sets an scheduling algorithm. It also intitalises I/O devices (e.g. if e-puck is used: motor, UART, remote control, and camera)
 *
 * @post 	To start OpenSwarm, Sys_Start_Kernel() mast be executed after the initialisation.
 * @remark	Code can be executed between initialisation and start of the kernel. But, note that you can only execute code that does not depend on an active OpenSwarm.
 */
void Sys_Init_Kernel(){

#ifdef EPUCK_USED
    e_init_port(); //Set all pins and ports
#endif

    //Init Scheduling
    Sys_Init_SystemTimer(Sys_Scheduler_RoundRobin);//start the system timer + interrupt = HDI - hardware dependent implementaion
    Sys_Init_Process_Management();

    //Init Events
    Sys_Register_Event(SYS_EVENT_TERMINATION);

    //init I/O Devices
    Sys_Init_IOManagement();
    Sys_Init_Clock();
    
#ifdef EPUCK_USED
#ifdef SYS_MOTOR_USED
    Sys_Init_Motors();
#endif
#ifdef SYS_UART1_USED
    Sys_Init_UART1();
#endif
#ifdef SYS_REMOTECONTROL_USED
    Sys_Init_RemoteControl();
#endif
#ifdef SYS_CAMERA_USED
   // Sys_Init_Camera();
#endif
#endif
}

/**
 *
 * This Function starts all functions of the operating system. The system MUST HAVE BEEN INITIALISED before.
 * @pre	 System must be initialised with Sys_Init_Kernel().
 * @remark	Code can be executed between initialisation and start of the kernel. But, note that you can only execute code that does not depend on an active OpenSwarm.
 */
void Sys_Start_Kernel(void){

    Sys_Start_SystemTimer();

    Sys_Start_IOManagement();
    
#ifdef EPUCK_USED
#ifdef SYS_MOTOR_USED
#endif
#ifdef SYS_UART1_USED
    Sys_Start_UART1();
#endif
#ifdef SYS_REMOTECONTROL_USED
    Sys_Start_RemoteControl();
#endif
#ifdef SYS_CAMERA_USED
    Sys_Start_Camera();
#endif
#endif
}

