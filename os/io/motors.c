/**
 * @file motors.c
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
 * This file provides the function needed to actuate the motors
 */

#include "io.h"
#include "motors.h"
#include "../platform/e-puck/system_IO_motors_HDI.h"

#include "../events/events.h"

#include "../definitions.h"
#include <stdbool.h>
#include <stdlib.h>

#define MAX_WHEEL_SPEED 128
#define POWER_SAVE_WAIT 15

typedef struct sys_motors_s{
    sint16 speed;
} sys_motors;

void Sys_LeftMotor_Controller(void);
void Sys_RightMotor_Controller(void);

bool Sys_LeftMotor_EventHandler(uint16, uint16, sys_event_data *);
bool Sys_RightMotor_EventHandler(uint16, uint16, sys_event_data *);

static sys_motors left_motor;
static sys_motors right_motor;

void Sys_Init_Motors(){
    bool occured_error = false;
    Sys_Init_IOManagement();

    left_motor.speed = 0;
    right_motor.speed = 0;

    if(!Sys_Register_IOHandler(&Sys_LeftMotor_Controller)){
        occured_error = true;
    }
    if(occured_error || !Sys_Register_IOHandler(&Sys_RightMotor_Controller)){
        occured_error = true;
    }
    if(!Sys_Register_Event(SYS_EVENT_IO_MOTOR_LEFT)){
        occured_error = true;
    }
    if(occured_error || !Sys_Register_Event(SYS_EVENT_IO_MOTOR_RIGHT)){
        occured_error = true;
    }
    if(occured_error || !Sys_Subscribe_to_Event(SYS_EVENT_IO_MOTOR_LEFT, 0, &Sys_LeftMotor_EventHandler, 0)){
        occured_error = true;
    }
    if(occured_error || !Sys_Subscribe_to_Event(SYS_EVENT_IO_MOTOR_RIGHT, 0, Sys_RightMotor_EventHandler, 0)){
        occured_error = true;
    }


    if(occured_error){
        Sys_Unregister_IOHandler(Sys_LeftMotor_Controller);
        Sys_Unregister_IOHandler(Sys_RightMotor_Controller);
        Sys_Unregister_Event(SYS_EVENT_IO_MOTOR_LEFT);
        Sys_Unregister_Event(SYS_EVENT_IO_MOTOR_RIGHT);
        Sys_Unsubscribe_from_Event(SYS_EVENT_IO_MOTOR_LEFT, 0);
        Sys_Unsubscribe_from_Event(SYS_EVENT_IO_MOTOR_RIGHT, 0);
        return;
    }
}

inline void Sys_LeftMotor_Reset(){
    Sys_LeftMotor_SetPhase_HDI(MOTORPHASE_RESET);
}

inline void Sys_RightMotor_Reset(){
    Sys_RightMotor_SetPhase_HDI(MOTORPHASE_RESET);
}

void Sys_LeftMotor_Controller(){

   static uint8 phase = 0;		 // phase can be 0 to 3
   static sint16 next_phase = 0;
   static uint8 power_saving = 0;
   
   if(left_motor.speed == 0){
       Sys_LeftMotor_Reset();
       return;
   }

   if(left_motor.speed < 0){
       if(--next_phase <= 0){
            phase--;
            next_phase = (10*MAX_WHEEL_SPEED)/abs(left_motor.speed);
            power_saving = 0;
       } else {
           if(power_saving >= POWER_SAVE_WAIT){
            Sys_LeftMotor_Reset();
            return;
           }
           power_saving++;//
       }
   }else{
       if(--next_phase <= 0){
           phase++;
           next_phase = (10*MAX_WHEEL_SPEED)/left_motor.speed;
            power_saving = 0;
       } else {
           if(power_saving >= POWER_SAVE_WAIT){
            Sys_LeftMotor_Reset();
            return;
           }
           power_saving++;
       } 
   }

   phase %= 4;
    Sys_LeftMotor_SetPhase_HDI(phase);
}

void Sys_RightMotor_Controller(){

   static sint8 phase = 0;		 // phase can be 0 to 3
   static sint16 next_phase = 0;
   static uint8 power_saving = 0;

   if(right_motor.speed == 0){
        Sys_RightMotor_Reset();
       return;
   }

   if(right_motor.speed < 0){
       if(--next_phase <= 0){
            phase--;
            next_phase = (10*MAX_WHEEL_SPEED)/abs(right_motor.speed);
            power_saving = 0;
       } else {
           if(power_saving >= POWER_SAVE_WAIT){
            Sys_RightMotor_Reset();
            return;
           }
           power_saving++;
       }
   }else{
       if(--next_phase <= 0){
           phase++;
           next_phase = (10*MAX_WHEEL_SPEED)/right_motor.speed;
            power_saving = 0;
       } else {
           if(power_saving >= POWER_SAVE_WAIT){
            Sys_RightMotor_Reset();
            return;
           }
           power_saving++;
       }
   }

   phase %= 4;
  // set the phase on the port pins

  Sys_RightMotor_SetPhase_HDI(phase);
}

bool Sys_LeftMotor_EventHandler(uint16 pid, uint16 eventID, sys_event_data *data/*mm/s*/){
    sint16 *speed = (sint16 *) data->value;
    Sys_Set_LeftWheelSpeed( (MAX_WHEEL_SPEED * speed[0])/MAX_WHEEL_SPEED_MM_S);
    return true;
}

bool Sys_RightMotor_EventHandler(uint16 pid, uint16 eventID, sys_event_data *data /*mm/s*/){
    sint16 *speed = (sint16 *) data->value;

    Sys_Set_RightWheelSpeed( (MAX_WHEEL_SPEED * speed[0])/MAX_WHEEL_SPEED_MM_S);

    return true;

}

void Sys_Set_LeftWheelSpeed(sint16 speed){
    if(speed > MAX_WHEEL_SPEED){
        left_motor.speed = MAX_WHEEL_SPEED;
        return;
    }else if(speed < -MAX_WHEEL_SPEED){
        left_motor.speed = -MAX_WHEEL_SPEED;
        return;
    }
    left_motor.speed = speed;
}
void Sys_Set_RightWheelSpeed(sint16 speed){
    if(speed > MAX_WHEEL_SPEED){
        right_motor.speed = MAX_WHEEL_SPEED;
        return;
    }else if(speed < -MAX_WHEEL_SPEED){
        right_motor.speed = -MAX_WHEEL_SPEED;
        return;
    }
    right_motor.speed = speed;
}


sint16 Sys_Get_LeftWheelSpeed(void){
    return left_motor.speed;
}
sint16 Sys_Get_RightWheelSpeed(void){
    return left_motor.speed;
}
