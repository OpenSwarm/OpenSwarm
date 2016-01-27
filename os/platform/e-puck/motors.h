/*!
 * \file
 * \ingroup motors
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \author  Gabriel Kapellmann Zafra <gkapellmann@gmail.com >
 * 
 * \version 1.0
 *
 * \date 30 July 2015
 * 
 * \brief It declares functions to drive motors
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

/*! \defgroup motors Motor Control
 * \ingroup io
 * \ingroup epuck
 * \brief Functions to control the two stepper motors of the e-puck.
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * The motor control module controls the speed and direction of the two stepper motors
 * 
 * \section motors_usage Usage
 * After the initialisation with Sys_Init_Motors(), the motors can be used by setting the motor speed.
 * This can be done by sending the motor velocities via events to SYS_EVENT_IO_MOTOR_LEFT and SYS_EVENT_IO_MOTOR_RIGHT or by setting the speed directly by calling Sys_Set_LeftWheelSpeed(sint16) and Sys_Set_RightWheelSpeed(sint16).
 * The current speed can be obtained Sys_Get_LeftWheelSpeed() and Sys_Get_RightWheelSpeed().
 * 
 * \section motors_license License
 * LICENSE: adapted FreeBSD License (see http://openswarm.org/license)\n
 * Copyright (c) 2015, Stefan M. Trenkwalder\n
 * All rights reserved. 
 */

#ifndef SYSTEM_IO_MOTORS_H
#define	SYSTEM_IO_MOTORS_H

#include "../../io/io.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define MAX_WHEEL_SPEED_MM_S 129 /*mm/s*/ 	/*!< Maximum wheel speed in mm/s*/
    
void Sys_Init_Motors(void);

void Sys_Set_LeftWheelSpeed(sint16 speed);
void Sys_Set_RightWheelSpeed(sint16 speed);
sint16 Sys_Get_LeftWheelSpeed(void);
sint16 Sys_Get_RightWheelSpeed(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_MOTORS_H */

