/**
 * @file system_IO_motors_HDI.c
 * @author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Created on 30 July 2015
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
 * This file provides functions to drive the motors
 */

#include "system_IO_motors_HDI.h"

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
