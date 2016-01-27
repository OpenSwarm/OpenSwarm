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
 * \brief  Hardware dependent implementations to drive motors
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 */

#include "motors_HDI.h"

/**
 *
 * This function sets the left motor phase
 * 
 * @param[in] phase indicates the phase of the left motor
 */
inline void Sys_LeftMotor_SetPhase_HDI(sint8 phase){
    switch (phase){
        case 0:
        {
            MOTOR1_PHA = 0;
            MOTOR1_PHB = 1;
            MOTOR1_PHC = 1;
            MOTOR1_PHD = 0;
            break;
        }
        case 1:
        {
            MOTOR1_PHA = 1;
            MOTOR1_PHB = 0;
            MOTOR1_PHC = 1;
            MOTOR1_PHD = 0;
            break;
        }
        case 2:
        {
            MOTOR1_PHA = 1;
            MOTOR1_PHB = 0;
            MOTOR1_PHC = 0;
            MOTOR1_PHD = 1;
            break;
        }
        case 3:
        {
            MOTOR1_PHA = 0;
            MOTOR1_PHB = 1;
            MOTOR1_PHC = 0;
            MOTOR1_PHD = 1;
            break;
        }
        case -1:
        default:
        {
            MOTOR1_PHA = 0;
            MOTOR1_PHB = 0;
            MOTOR1_PHC = 0;
            MOTOR1_PHD = 0;
            break;
        }

    }
}

/**
 *
 * This function sets the right motor phase
 * 
 * @param[in] phase indicates the phase of the right motor
 */
inline void Sys_RightMotor_SetPhase_HDI(sint8 phase){
    switch (phase){
        case 0:
        {
            MOTOR2_PHA = 0;
            MOTOR2_PHB = 1;
            MOTOR2_PHC = 1;
            MOTOR2_PHD = 0;
            break;
        }
        case 1:
        {
            MOTOR2_PHA = 1;
            MOTOR2_PHB = 0;
            MOTOR2_PHC = 1;
            MOTOR2_PHD = 0;
            break;
        }
        case 2:
        {
            MOTOR2_PHA = 1;
            MOTOR2_PHB = 0;
            MOTOR2_PHC = 0;
            MOTOR2_PHD = 1;
            break;
        }
        case 3:
        {
            MOTOR2_PHA = 0;
            MOTOR2_PHB = 1;
            MOTOR2_PHC = 0;
            MOTOR2_PHD = 1;
            break;
        }
        case -1:
        default:
        {
            MOTOR2_PHA = 0;
            MOTOR2_PHB = 0;
            MOTOR2_PHC = 0;
            MOTOR2_PHD = 0;
            break;
        }

    }
}
