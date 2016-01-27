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
 * \brief  It defines function to drive motors
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#include "motors.h"
#include "motors_HDI.h"

#include "../../io/io.h"
#include "../../events/events.h"

#include "../../definitions.h"
#include <stdlib.h>

#define MAX_WHEEL_SPEED 128 	/*!< Maximum wheel speed in steps*/
#define POWER_SAVE_WAIT 15      /*!< amount of steps needed to move the motor one step further */

/**
 * @brief This struct contains the speed for a motor.
 */
typedef struct sys_motors_s{
    sint16 speed;/*!< speed of one motor*/
} sys_motors;

void Sys_LeftMotor_Controller(void);
void Sys_RightMotor_Controller(void);

bool Sys_LeftMotor_EventHandler(uint, uint, sys_event_data *);
bool Sys_RightMotor_EventHandler(uint, uint, sys_event_data *);

static sys_motors left_motor;/*!< wheel speed for the left motor*/
static sys_motors right_motor;/*!< wheel speed for the right motor*/

/**
 *
 * This function initialises the motor module including both left and right motor.
 *
 */
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

/**
 *
 * This function resets the left motor to a reset state.
 *
 */
inline void Sys_LeftMotor_Reset(){
    Sys_LeftMotor_SetPhase_HDI(MOTORPHASE_RESET);
}

/**
 *
 * This function resets the right motor to a reset state.
 *
 */
inline void Sys_RightMotor_Reset(){
    Sys_RightMotor_SetPhase_HDI(MOTORPHASE_RESET);
}

/**
 *
 * This function controls the speed of the left motor. The speed is set by moving the to the next step within the appropriate time step.
 *
 */
void Sys_LeftMotor_Controller(){

   static uint8 phase = 0;		 // phase can be 0 to 3
   static sint16 next_phase = 0;
   static uint8 power_saving = 0;
   
   if(left_motor.speed == 0){//if stop -> reset speed to that no current is used
       Sys_LeftMotor_Reset();
       return;
   }

   if(left_motor.speed < 0){//orientation
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

/**
 *
 * This function controls the speed of the right motor. The speed is set by moving the to the next step within the appropriate time step.
 *
 */
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

/**
 *
 * This function sets the left motor speed that is received by the  event SYS_EVENT_IO_MOTOR_LEFT.
 *
 * @param[in] pid the process id to which the event handler is registered
 * @param[in] eventID the event id which identifies the event that is handled
 * @param[in] data the event data that contain the motor speed.
 */
bool Sys_LeftMotor_EventHandler(uint pid, uint eventID, sys_event_data *data/*mm/s*/){
    sint16 *speed = (sint16 *) data->value;
    Sys_Set_LeftWheelSpeed( (MAX_WHEEL_SPEED * speed[0])/MAX_WHEEL_SPEED_MM_S);
    return true;
}

/**
 *
 * This function sets the right motor speed that is received by the  event SYS_EVENT_IO_MOTOR_RIGHT.
 *
 * @param[in] pid the process id to which the event handler is registered
 * @param[in] eventID the event id which identifies the event that is handled
 * @param[in] data the event data that contain the motor speed.
 */
bool Sys_RightMotor_EventHandler(uint pid, uint eventID, sys_event_data *data /*mm/s*/){
    sint16 *speed = (sint16 *) data->value;

    Sys_Set_RightWheelSpeed( (MAX_WHEEL_SPEED * speed[0])/MAX_WHEEL_SPEED_MM_S);

    return true;

}

/**
 *
 * This function sets the value for the speed of the left motor.
 * 
 * @param speed of the left wheel
 */
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

/**
 *
 * This function sets the value for the speed of the right motor.
 * 
 * @param speed of the right wheel
 */
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

/**
 *
 * This function returns the speed of the left motor.
 * 
 */
sint16 Sys_Get_LeftWheelSpeed(void){
    return left_motor.speed;
}

/**
 *
 * This function returns the speed of the right motor.
 * 
 */
sint16 Sys_Get_RightWheelSpeed(void){
    return left_motor.speed;
}
