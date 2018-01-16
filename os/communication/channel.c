#include "channel.h"

#include "../platform/e-puck/adc.h"
#include "../platform/e-puck/leds.h"
#include "../platform/e-puck/motors.h"
#include "../platform/e-puck/selector.h"

#include "../interrupts.h"

typedef enum {
    dark_stationary = 0,
    dark_moving = 1,
    led_stationary = 2            
} sensor_situation;

static uint sensorThresholds[8][4] = {  {0xFFFF,0xFFFF,0xFFFF,0xFFFF},
                                        {0xFFFF,0xFFFF,0xFFFF,0xFFFF},
                                        {0xFFFF,0xFFFF,0xFFFF,0xFFFF},
                                        {0xFFFF,0xFFFF,0xFFFF,0xFFFF},
                                        {0xFFFF,0xFFFF,0xFFFF,0xFFFF},
                                        {0xFFFF,0xFFFF,0xFFFF,0xFFFF},
                                        {0xFFFF,0xFFFF,0xFFFF,0xFFFF},
                                        {0xFFFF,0xFFFF,0xFFFF,0xFFFF}};


static uint ambient[8] = {0xFFFF};
static int offset_motors[8] = {0xFFFF};
static int offset_leds[8] = {0xFFFF};

typedef void (*initFunctions)(uint, uint);


void InitLEDOffset(uint sensor, uint value);
void InitMotorOffset(uint sensor, uint value);
void InitAmbient(uint sensor, uint value);

const static initFunctions initFuncs[3] = {InitAmbient, InitMotorOffset, InitLEDOffset};

static sensor_situation current_situation = dark_stationary;

void InitSensor0(uint);
void InitSensor1(uint);
void InitSensor2(uint);
void InitSensor3(uint);
void InitSensor4(uint);
void InitSensor5(uint);
void InitSensor6(uint);
void InitSensor7(uint);
inline void InitSensor(uint,uint);
//void InitSensorSituations(void);
sensor_situation getCurrentSituation(uint);

static uint init_interation = 0;

#define THRESHOLD_L 4
static uint variable_threshold = THRESHOLD_L;

void Sys_Start_ChannelCalibration(){
    
    variable_threshold = THRESHOLD_L;
    
    clearLEDs();
    clearIRs();
    
    current_situation = dark_stationary;
    init_interation = 0;
    
    Sys_Start_AtomicSection();
        Sys_Subscribe_ADCChannelProcessor(Prx0, InitSensor0);
        Sys_Subscribe_ADCChannelProcessor(Prx1, InitSensor1);
        Sys_Subscribe_ADCChannelProcessor(Prx2, InitSensor2);
        Sys_Subscribe_ADCChannelProcessor(Prx3, InitSensor3);
        Sys_Subscribe_ADCChannelProcessor(Prx4, InitSensor4);
        Sys_Subscribe_ADCChannelProcessor(Prx5, InitSensor5);
        Sys_Subscribe_ADCChannelProcessor(Prx6, InitSensor6);
        Sys_Subscribe_ADCChannelProcessor(Prx7, InitSensor7); 
    Sys_End_AtomicSection(); 
        
    while(init_interation < 8000){
        ;
    }
        
    Sys_Start_AtomicSection();
        current_situation = dark_moving;
        init_interation = 0;
        Sys_Set_StepsRight(-WHOLETURN);
        Sys_Set_StepsLeft(WHOLETURN); 
    Sys_End_AtomicSection(); 
    
    while(Sys_Get_StepsLeft() || Sys_Get_StepsRight() ){ // !=
        ;
    }
            
    Sys_Start_AtomicSection();
        current_situation = led_stationary;
        init_interation = 0;
        setLEDs(); 
        Sys_Set_LeftWheelSpeed(0);
        Sys_Set_RightWheelSpeed(0);
    Sys_End_AtomicSection(); 
    
    
    while(init_interation < 8000){
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
    
    
    clearLEDs();
    clearIRs();
}
/*
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
}*/

//uint Sys_ComThreshold(){
//    return sensorThreshold - THRESHOLD_L;
//}

int getOffset(uint sensor){
    int offset = 0;
    
    if(Sys_Get_LeftWheelSpeed() || Sys_Get_RightWheelSpeed()){
        offset += offset_motors[sensor];
    }
    
    switch(sensor){
            case 0:
                if(FRONT_LED == 1 || LED0 == 1 || LED1 == 1){
                    offset += offset_leds[sensor];
                }
                break;
            case 1:
                if(FRONT_LED == 1 || LED1 == 1){
                    offset += offset_leds[sensor];
                }
                break;
            case 2:
                if(LED1 == 1 || LED2 == 1 || LED3 == 1){
                    offset += offset_leds[sensor];
                }
                break;
            case 3:
                if(LED3 == 1 || LED4 == 1){
                    offset += offset_leds[sensor];
                }
                break;
            case 4:
                if(LED4 == 1 || LED5 == 1){
                    offset += offset_leds[sensor];
                }
                break;
            case 5:
                if(LED5 == 1 || LED6 == 1 || LED7 == 1){
                    offset += offset_leds[sensor];
                }
                break;
            case 6:
                if(LED6 == 1 || LED7 == 1){
                    offset += offset_leds[sensor];
                }
                break;
            case 7:
                if(FRONT_LED == 1 || LED0 == 1 || LED7 == 1){
                    offset += offset_leds[sensor];
                }
                break;
            default:
                if(FRONT_LED == 1 || LED0 == 1 || LED1 == 1 || LED2 == 1 || LED3 == 1 || LED4 == 1 || LED5 == 1 || LED6 == 1 || LED7 == 1){
                    offset += offset_leds[sensor];
                }
                break;
    }
    
    return offset;
}

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
    return (uint) ((int)ambient[sensor]-(getOffset(sensor)+(int) variable_threshold));//4*THRESHOLD_L*Sys_Get_Selector();
}

uint Sys_GetThreshold(){
    return variable_threshold;//4*THRESHOLD_L*Sys_Get_Selector();
}

void Sys_SetThreshold(uint thres){
    variable_threshold = thres;
}

uint Sys_ComBackground(uint sensor){
    return ambient[sensor]-getOffset(sensor);//4*THRESHOLD_L*Sys_Get_Selector();
}

void Sys_SetComBackground(uint sensor, uint thres){
    ambient[sensor] = thres;//4*THRESHOLD_L*Sys_Get_Selector();
}

void Sys_UpdateBackground(uint sensor, int diff){
    
    int value = diff+getOffset(sensor);
    
    if(value < 0){
        ambient[sensor]--;
    }
    if(value > 0){
        ambient[sensor]++;
    }
}
/*
void InitSensor0(uint value){
    uint state = (uint) sys_init_state;
    if(sensorThreshold[state][0]== 0){
        sensorThreshold[state][0] = value;
        return;
    }
    
    if(init_sensor_additions < 1000){
        uint32 calc = ((uint32) sensorThreshold[state][0])*init_sensor_additions;
        calc += value;
    
        sensorThreshold[state][0] = calc/(init_sensor_additions+1);
    }else{
        if(value > sensorThreshold[state][0]){
            sensorThreshold[state][0]++;
        }else if(value < sensorThreshold[state][0]){
            sensorThreshold[state][0]--;
        }
    }
}
void InitSensor1(uint value){
    uint state = (uint) sys_init_state;
    
    if(sensorThreshold[state][1]== 0){
        sensorThreshold[state][1] = value;
        return;
    }
    
    if(init_sensor_additions < 1000){
        uint32 calc = ((uint32) sensorThreshold[state][1])*init_sensor_additions;
        calc += value;
    
        sensorThreshold[state][1] = calc/(init_sensor_additions+1);
    }else{
        if(value > sensorThreshold[state][1]){
            sensorThreshold[state][1]++;
        }else if(value < sensorThreshold[state][1]){
            sensorThreshold[state][1]--;
        }
    }
}
void InitSensor2(uint value){
    uint state = (uint) sys_init_state;
    if(sensorThreshold[state][2]== 0){
        sensorThreshold[state][2] = value;
        return;
    }
    
    if(init_sensor_additions < 1000){
        uint32 calc = ((uint32) sensorThreshold[state][2])*init_sensor_additions;
        calc += value;
    
        sensorThreshold[state][2] = calc/(init_sensor_additions+1);
    }else{
        if(value > sensorThreshold[state][0]){
            sensorThreshold[state][2]++;
        }else if(value < sensorThreshold[state][2]){
            sensorThreshold[state][2]--;
        }
    }
}
void InitSensor3(uint value){
    uint state = (uint) sys_init_state;
    if(sensorThreshold[state][3]== 0){
        sensorThreshold[state][3] = value;
        return;
    }
    
    if(init_sensor_additions < 1000){
        uint32 calc = ((uint32) sensorThreshold[state][3])*init_sensor_additions;
        calc += value;
    
        sensorThreshold[state][3] = calc/(init_sensor_additions+1);
    }else{
        if(value > sensorThreshold[state][3]){
            sensorThreshold[state][3]++;
        }else if(value < sensorThreshold[state][3]){
            sensorThreshold[state][3]--;
        }
    }
}
void InitSensor4(uint value){
    uint state = (uint) sys_init_state;
    if(sensorThreshold[state][4]== 0){
        sensorThreshold[state][4] = value;
        return;
    }
    
    if(init_sensor_additions < 1000){
        uint32 calc = ((uint32) sensorThreshold[state][4])*init_sensor_additions;
        calc += value;
    
        sensorThreshold[state][4] = calc/(init_sensor_additions+1);
    }else{
        if(value > sensorThreshold[state][4]){
            sensorThreshold[state][4]++;
        }else if(value < sensorThreshold[state][4]){
            sensorThreshold[state][4]--;
        }
    }
}
void InitSensor5(uint value){
    uint state = (uint) sys_init_state;
    if(sensorThreshold[state][5]== 0){
        sensorThreshold[state][5] = value;
        return;
    }
    
    if(init_sensor_additions < 1000){
        uint32 calc = ((uint32) sensorThreshold[state][5])*init_sensor_additions;
        calc += value;
    
        sensorThreshold[state][5] = calc/(init_sensor_additions+1);
    }else{
        if(value > sensorThreshold[state][5]){
            sensorThreshold[state][5]++;
        }else if(value < sensorThreshold[state][5]){
            sensorThreshold[state][5]--;
        }
    }
}
void InitSensor6(uint value){
    uint state = (uint) sys_init_state;
    if(sensorThreshold[state][6]== 0){
        sensorThreshold[state][6] = value;
        return;
    }
    
    if(init_sensor_additions < 1000){
        uint32 calc = ((uint32) sensorThreshold[state][6])*init_sensor_additions;
        calc += value;
    
        sensorThreshold[state][6] = calc/(init_sensor_additions+1);
    }else{
        if(value > sensorThreshold[state][6]){
            sensorThreshold[state][6]++;
        }else if(value < sensorThreshold[state][6]){
            sensorThreshold[state][6]--;
        }
    }
}
void InitSensor7(uint value){
    uint state = (uint) sys_init_state;
    if(sensorThreshold[state][7]== 0){
        sensorThreshold[state][7] = value;
        return;
    }
    
    if(init_sensor_additions < 1000){
        uint32 calc = ((uint32) sensorThreshold[state][7])*init_sensor_additions;
        calc += value;
    
        sensorThreshold[state][7] = calc/(init_sensor_additions+1);
    }else{
        if(value > sensorThreshold[state][7]){
            sensorThreshold[state][7]++;
        }else if(value < sensorThreshold[state][7]){
            sensorThreshold[state][7]--;
        }
    }
}

void InitAllSensors(void){
    init_sensor_additions++;
    
    if(init_sensor_additions > 1000){
        sys_init_state++;
        Sys_SetInitStateEnvironm(sys_init_state);
        init_sensor_additions = 0;
    }
}
*/


void InitSensor0(uint value){
    initFuncs[current_situation](0, value);
}
void InitSensor1(uint value){
    initFuncs[current_situation](1, value);
}
void InitSensor2(uint value){
    initFuncs[current_situation](2, value);
}
void InitSensor3(uint value){
    initFuncs[current_situation](3, value);
}
void InitSensor4(uint value){
    initFuncs[current_situation](4, value);
}
void InitSensor5(uint value){
    initFuncs[current_situation](5, value);
}
void InitSensor6(uint value){
    initFuncs[current_situation](6, value);
}
void InitSensor7(uint value){
    initFuncs[current_situation](7, value);
}

void InitAmbient(uint sensor, uint value){
    Sys_Start_AtomicSection();

    uint v_thres = ambient[sensor];
    if( v_thres == 0 || v_thres == 0xFFFF){
        ambient[sensor] = value;
        Sys_End_AtomicSection();
        return;
    }
    
    if(v_thres > value){
        ambient[sensor]--;
    }
    if(v_thres < value){
        ambient[sensor]++;
    }
    
    init_interation++;    
    
    Sys_End_AtomicSection();
}

void InitMotorOffset(uint sensor, uint value){
    Sys_Start_AtomicSection();

    uint v_ambient = ambient[sensor];
    int v_motors = offset_motors[sensor];
    if( v_motors == 0 || v_motors == 0xFFFF){
        offset_motors[sensor] = v_ambient-value;
        Sys_End_AtomicSection();
        return;
    }
    
    int offset = v_ambient-value;
    if(offset > v_motors){
        offset_motors[sensor]++;
    }
    
    if(offset < v_motors){
        offset_motors[sensor]--;
    }
    
    Sys_End_AtomicSection();
}

void InitLEDOffset(uint sensor, uint value){    
    Sys_Start_AtomicSection();

    uint v_ambient = ambient[sensor];
    int v_led = offset_leds[sensor];
    if( v_led == 0 || v_led == 0xFFFF){
        offset_leds[sensor] = v_ambient-value;
        Sys_End_AtomicSection();
        return;
    }
    
    int offset = v_ambient-value;
    if(offset > v_led){
        offset_leds[sensor]++;
    }
    
    if(offset < v_led){
        offset_leds[sensor]--;
    }
    
    init_interation++;
    
    Sys_End_AtomicSection();
}
