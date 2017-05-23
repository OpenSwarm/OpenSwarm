#include "channel.h"

#include "../platform/e-puck/adc.h"
#include "../platform/e-puck/leds.h"
#include "../platform/e-puck/motors.h"

#include "../interrupts.h"

uint sensorThreshold = 0xFFFF;

void InitSensor(uint);
void InitSensors(void);

static uint min_value = 0xFFFF;
static uint init_interation = 0;

#define THRESHOLD_L 20

void Sys_Start_ChannelCalibration(){
    
    clearLEDs();
    clearIRs();
    
    Sys_Start_AtomicSection();
        Sys_Subscribe_ADCChannelProcessor(Prx0, InitSensor);
        Sys_Subscribe_ADCChannelProcessor(Prx1, InitSensor);
        Sys_Subscribe_ADCChannelProcessor(Prx2, InitSensor);
        Sys_Subscribe_ADCChannelProcessor(Prx3, InitSensor);
        Sys_Subscribe_ADCChannelProcessor(Prx4, InitSensor);
        Sys_Subscribe_ADCChannelProcessor(Prx5, InitSensor);
        Sys_Subscribe_ADCChannelProcessor(Prx6, InitSensor);
        Sys_Subscribe_ADCChannelProcessor(Prx7, InitSensor); 
        Sys_Subscribe_ADCFinish(ProxDone, InitSensors); 
    Sys_End_AtomicSection(); 
        
    while(init_interation < 1024){
        ;
    }
    
    Sys_Set_StepsRight(-WHOLETURN);
    Sys_Set_StepsLeft(WHOLETURN); 
    
    while(Sys_Get_StepsLeft() || Sys_Get_StepsRight() ){ // !=
        ;
    }
       
    setLEDs(); 
    Sys_Set_StepsRight(WHOLETURN);
    Sys_Set_StepsLeft(-WHOLETURN); 
    
    while(Sys_Get_StepsLeft() || Sys_Get_StepsRight() ){ // !=
        ;
    }
    
    uint iterationstep = init_interation + 1024;
    
    if(iterationstep < init_interation ){
        while(init_interation >= iterationstep){
            ;
        }
    }
    
    while(init_interation < iterationstep){
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

void InitSensor(uint value){
    Sys_Start_AtomicSection();
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

uint Sys_ComThreshold(){
    return sensorThreshold - THRESHOLD_L;
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
