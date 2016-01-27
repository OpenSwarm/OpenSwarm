/*!
 * \file
 * \ingroup motors
 * \ingroup hdi
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * \version 1.0
 *
 * \date 27 August 2015
 * 
 * \brief  Hardware dependent implementations to  drive motors
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 */

#ifndef SYSTEM_IO_MOTORS_HDI_H
#define	SYSTEM_IO_MOTORS_HDI_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define MOTORPHASE_RESET -1 /*!< the reset value for the motor phase*/
    
#include "../../definitions.h"
    
inline void Sys_LeftMotor_SetPhase_HDI(sint8 phase);
inline void Sys_RightMotor_SetPhase_HDI(sint8 phase);

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_MOTORS_HDI_H */

