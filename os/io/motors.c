
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
