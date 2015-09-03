
#include "system_IO.h"
#include "system_IO_motors.h"
#include "system_Events.h"

#include "HDI_epuck_ports.h"
#include <stdbool.h>
#include <stdlib.h>

#define MAX_WHEEL_SPEED 1288
#define MAX_WHEEL_SPEED_MM_S 129 /*mm/s*/

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
    if(occured_error || !Sys_Subscribe_to_Event(SYS_EVENT_IO_MOTOR_RIGHT, 0, &Sys_RightMotor_EventHandler, 0)){
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

void Sys_LeftMotor_Controller(){

   static uint8 phase = 0;		 // phase can be 0 to 3
   static uint16 next_phase = 0;

   if(left_motor.speed == 0){
       return;
   }

   if(left_motor.speed < 0){
       if(--next_phase <= 0){
            phase--;
            next_phase = MAX_WHEEL_SPEED/abs(left_motor.speed);
       }
   }else{
       if(--next_phase <= 0){
           phase++;
           next_phase = MAX_WHEEL_SPEED/left_motor.speed;
       }
   }

   phase %= 4;
  
  // set the phase on the port pins

  switch (phase)
  {
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

  }


}

void Sys_RightMotor_Controller(){

   static uint8 phase = 0;		 // phase can be 0 to 3
   static uint16 next_phase = 0;

   if(right_motor.speed == 0){
       return;
   }

   if(right_motor.speed < 0){
       if(--next_phase <= 0){
            phase--;
            next_phase = MAX_WHEEL_SPEED/abs(right_motor.speed);
       }
   }else{
       if(--next_phase <= 0){
           phase++;
           next_phase = MAX_WHEEL_SPEED/right_motor.speed;
       }
   }

   phase %= 4;
  // set the phase on the port pins

  switch (phase)
  {
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

  }
}

bool Sys_LeftMotor_EventHandler(uint16 pid, uint16 eventID, sys_event_data *data/*mm/s*/){
    sint16 *speed = (sint16 *) data;

    left_motor.speed = MAX_WHEEL_SPEED/MAX_WHEEL_SPEED_MM_S * (*speed);
    return true;
}

bool Sys_RightMotor_EventHandler(uint16 pid, uint16 eventID, sys_event_data *data /*mm/s*/){
    sint16 *speed = (sint16 *) data;

    right_motor.speed = MAX_WHEEL_SPEED/MAX_WHEEL_SPEED_MM_S * (*speed);

    return true;

}

void Sys_Set_LeftWheelSpeed(uint16 speed){
    left_motor.speed = speed;
}
void Sys_Set_RightWheelSpeed(uint16 speed){
    right_motor.speed = speed;
}