#include "channel.h"

#include "../platform/e-puck/adc.h"
#include "../platform/e-puck/leds.h"
#include "../platform/e-puck/motors.h"
#include "../platform/e-puck/selector.h"

#include "../interrupts.h"

uint sensorThreshold = 0xFFFF;

typedef enum {
    dark_stationary = 0,
    dark_moving = 1,
    leds_moving = 2,
    led_stationary = 3            
} sensor_situation;

uint sensorThresholds[4] = {0xFFFF};
sensor_situation current_situation = dark_stationary;

void InitSensor(uint);
void InitSensorSituations(void);
sensor_situation getCurrentSituation(uint);

static uint min_value = 0xFFFF;
static uint init_interation = 0;

#define THRESHOLD_L 5
static uint variable_threshold = THRESHOLD_L;

void Sys_Start_ChannelCalibration(){
    
    variable_threshold = 4*THRESHOLD_L*Sys_Get_Selector();
    
    clearLEDs();
    clearIRs();
    
    current_situation = dark_stationary;
    init_interation = 0;
    
    Sys_Start_AtomicSection();
        Sys_Subscribe_ADCChannelProcessor(Prx0, InitSensor);
        Sys_Subscribe_ADCChannelProcessor(Prx1, InitSensor);
        Sys_Subscribe_ADCChannelProcessor(Prx2, InitSensor);
        Sys_Subscribe_ADCChannelProcessor(Prx3, InitSensor);
        Sys_Subscribe_ADCChannelProcessor(Prx4, InitSensor);
        Sys_Subscribe_ADCChannelProcessor(Prx5, InitSensor);
        Sys_Subscribe_ADCChannelProcessor(Prx6, InitSensor);
        Sys_Subscribe_ADCChannelProcessor(Prx7, InitSensor); 
        Sys_Subscribe_ADCFinish(ProxDone, InitSensorSituations); 
    Sys_End_AtomicSection(); 
        
    while(init_interation < 2048){
        ;
    }
    
    Sys_Start_AtomicSection();
        Sys_Set_StepsRight(-WHOLETURN);
        Sys_Set_StepsLeft(WHOLETURN); 
        current_situation = dark_moving;
        init_interation = 0;
    Sys_End_AtomicSection(); 
    
    
    while(Sys_Get_StepsLeft() || Sys_Get_StepsRight() ){ // !=
        ;
    }
    
    Sys_Start_AtomicSection();
        Sys_Set_StepsRight(-WHOLETURN);
        Sys_Set_StepsLeft(WHOLETURN); 
        setLEDs(); 
        current_situation = leds_moving;
        init_interation = 0;
    Sys_End_AtomicSection(); 
    
    while(Sys_Get_StepsLeft() || Sys_Get_StepsRight() ){ // !=
        ;
    }
            
    Sys_Start_AtomicSection();
        Sys_Set_LeftWheelSpeed(0);
        Sys_Set_RightWheelSpeed(0);
        setLEDs(); 
        current_situation = led_stationary;
        init_interation = 0;
    Sys_End_AtomicSection(); 
    
    
    while(init_interation < 2048){
        ;
    }
    
    Sys_Start_AtomicSection();
        Sys_Subscribe_ADCChannelProcessor(Prx0, 0);
        Sys_Subscribe_ADCChannelProcessor(Prx1, 0);
        Sys_Subscribe_ADCChannelProcessor(Prx2, 0);
        Sys_Subscribe_ADCChannelProcessor(Prx3, 0);
        Sys_Subscribe_ADCChannelProcessor(Prx4, 0);
        Sys_Subscribe_ADCChannelProcessor(Prx5, 0);
        Sys_Subscribe_ADCChannelProcessor(Prx6, 0);
        Sys_Subscribe_ADCChannelProcessor(Prx7, 0); 
        Sys_Subscribe_ADCFinish(ProxDone, 0); 
    Sys_End_AtomicSection(); 
}

void InitSensor(uint v){
    Sys_Start_AtomicSection();
        uint value = v;
        if( value < min_value){
            min_value = value;
        }
    Sys_End_AtomicSection();
}

void InitSensors(){
    
    if(sensorThreshold == 0){
        if( min_value != 0xFFFF ){
            sensorThreshold = min_value;
        }
        return;
    }
    
    Sys_Start_AtomicSection();
    if(init_interation < 2048){
        uint32 calc = ((uint32) sensorThreshold)*init_interation;
        calc += min_value;
    
        sensorThreshold = calc/(init_interation+1);
    }else{
        if(min_value > sensorThreshold){
            sensorThreshold++;
        }else if(min_value < sensorThreshold){
            sensorThreshold--;
        }
    }
    
    init_interation++;
    Sys_End_AtomicSection();
}
void InitSensorSituations(){
    
    if(sensorThresholds[current_situation] == 0){
        if( min_value != 0xFFFF ){
            sensorThresholds[current_situation] = min_value;
        }
        return;
    }
    
    Sys_Start_AtomicSection();
    uint32 calc = ((uint32) sensorThresholds[current_situation])*init_interation;
    calc += min_value;
    
    sensorThresholds[current_situation] = calc/(init_interation+1);
    
    init_interation++;
    Sys_End_AtomicSection();
}

//uint Sys_ComThreshold(){
//    return sensorThreshold - THRESHOLD_L;
//}

sensor_situation getCurrentSituation(uint sensor){
    if(Sys_Get_LeftWheelSpeed() || Sys_Get_RightWheelSpeed()){
        switch(sensor){
            case 0:
                if(FRONT_LED == 1 || LED0 == 1 || LED1 == 1){
                    return dark_moving;
                }
                break;
            case 1:
                if(FRONT_LED == 1 || LED1 == 1){
                    return dark_moving;
                }
                break;
            case 2:
                if(LED1 == 1 || LED2 == 1 || LED3 == 1){
                    return dark_moving;
                }
                break;
            case 3:
                if(LED3 == 1 || LED4 == 1){
                    return dark_moving;
                }
                break;
            case 4:
                if(LED4 == 1 || LED5 == 1){
                    return dark_moving;
                }
                break;
            case 5:
                if(LED5 == 1 || LED6 == 1 || LED7 == 1){
                    return dark_moving;
                }
                break;
            case 6:
                if(LED6 == 1 || LED7 == 1){
                    return dark_moving;
                }
                break;
            case 7:
                if(FRONT_LED == 1 || LED0 == 1 || LED7 == 1){
                    return dark_moving;
                }
                break;
            default:
                if(FRONT_LED == 1 || LED0 == 1 || LED1 == 1 || LED2 == 1 || LED3 == 1 || LED4 == 1 || LED5 == 1 || LED6 == 1 || LED7 == 1){
                    return dark_moving;
                }
                break;
            
        }
        
        return dark_moving;
    }
    
    switch(sensor){
        case 0:
            if(FRONT_LED == 1 || LED0 == 1 || LED1 == 1){
                return dark_moving;
            }
            break;
        case 1:
            if(FRONT_LED == 1 || LED1 == 1){
                return dark_moving;
            }
            break;
        case 2:
            if(LED1 == 1 || LED2 == 1 || LED3 == 1){
                return dark_moving;
            }
            break;
        case 3:
            if(LED3 == 1 || LED4 == 1){
                return dark_moving;
            }
            break;
        case 4:
            if(LED4 == 1 || LED5 == 1){
                return dark_moving;
            }
            break;
        case 5:
            if(LED5 == 1 || LED6 == 1 || LED7 == 1){
                return dark_moving;
            }
            break;
        case 6:
            if(LED6 == 1 || LED7 == 1){
                return dark_moving;
            }
            break;
        case 7:
            if(FRONT_LED == 1 || LED0 == 1 || LED7 == 1){
                return dark_moving;
            }
            break;
        default:
            if(FRONT_LED == 1 || LED0 == 1 || LED1 == 1 || LED2 == 1 || LED3 == 1 || LED4 == 1 || LED5 == 1 || LED6 == 1 || LED7 == 1){
                return dark_moving;
            }
            break;

    }
    
    return dark_stationary;
 //   dark_stationary = 0,
 //   dark_moving = 1,
 //   leds_moving = 2,
 //   led_stationary = 3      
}

uint Sys_ComThreshold(uint sensor){
    sensor_situation sit = getCurrentSituation(sensor);
    
    return sensorThresholds[sit]-variable_threshold; //4*THRESHOLD_L*Sys_Get_Selector();
}

void Sys_SetComThreshold(uint thres){
    variable_threshold = thres;
}

uint Sys_GetBaseSignal(uint sensor){ 
    sensor_situation sit = getCurrentSituation(sensor); 
     
    return sensorThresholds[sit]; 
} 
     
uint Sys_GetThreshold(){ 
    return variable_threshold; 
}
