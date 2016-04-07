
#include "adc.h"
#include "../../definitions.h"
#include "../../events/events.h"

#define PROX_CALIBRATION_N 11

uint sys_calibration_value[8] = {0};
uint sys_transformation[PROX_CALIBRATION_N][2] = {   {0,  3788}, 
                                    {10,  3548},
                                    {20,  857 },
                                    {30,  474 },
                                    {40,  296 },
                                    {50,  205 },
                                    {60,  153 },
                                    {70,  113 },
                                    {80,  84  },
                                    {90,  65  },
                                    {100, 53  }};

uint Sys_Prox_trasform(uint, uint);
void Sys_Prox0_preprocessor(uint);
void Sys_Prox1_preprocessor(uint);
void Sys_Prox2_preprocessor(uint);
void Sys_Prox3_preprocessor(uint);
void Sys_Prox4_preprocessor(uint);
void Sys_Prox5_preprocessor(uint);
void Sys_Prox6_preprocessor(uint);
void Sys_Prox7_preprocessor(uint);

uint sys_distances[8] = {0};

void Sys_Init_Proximity(void){

    Sys_Init_ADC();

    Sys_Register_Event(SYS_EVENT_IO_PROX_0);
    Sys_Register_Event(SYS_EVENT_IO_PROX_1);
    Sys_Register_Event(SYS_EVENT_IO_PROX_2);
    Sys_Register_Event(SYS_EVENT_IO_PROX_3);
    Sys_Register_Event(SYS_EVENT_IO_PROX_4);
    Sys_Register_Event(SYS_EVENT_IO_PROX_5);
    Sys_Register_Event(SYS_EVENT_IO_PROX_6);
    Sys_Register_Event(SYS_EVENT_IO_PROX_7);

    Sys_Subscribe_ADCChannelProcessor(Prx0, Sys_Prox0_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx1, Sys_Prox1_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx2, Sys_Prox2_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx3, Sys_Prox3_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx4, Sys_Prox4_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx5, Sys_Prox5_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx6, Sys_Prox6_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx7, Sys_Prox7_preprocessor);    
       
}

void Sys_Start_Proximity(void){
    
    
}

uint Sys_Prox_trasform(uint i, uint value){
    if(sys_calibration_value[i] < value){
        sys_calibration_value[i] = value;
    }
    
    uint cal = sys_calibration_value[i] - value;
    
    if(cal == 0){
        return 0;
    }
    
    int j;
    for(j = 0; j < PROX_CALIBRATION_N; j++){
        if( sys_transformation[j][1] > cal){
            continue;
        }
        
        
        
        return sys_transformation[j][0] - ((10 * (cal - sys_transformation[j][1])) / (sys_transformation[j-1][1]-sys_transformation[j][1]));
    }
    
    return 0xFFFF;
    
}

void Sys_Prox0_preprocessor(uint value){
    static int lowpass = 0;
        
    uint dist = Sys_Prox_trasform(0, value);
    
    sys_distances[0] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        lowpass = 0;
        Sys_Send_IntEvent(SYS_EVENT_IO_PROX_0, dist);
    }else{
        lowpass++;
    }
}
void Sys_Prox1_preprocessor(uint value){
    static int lowpass = 0;
    
    uint dist = Sys_Prox_trasform(1, value);
    
    sys_distances[1] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        Sys_Send_IntEvent(SYS_EVENT_IO_PROX_1, dist);
        lowpass = 0;
    }else{
        lowpass++;
    }
}
void Sys_Prox2_preprocessor(uint value){
    static int lowpass = 0;
    uint dist = Sys_Prox_trasform(2, value);
    
    sys_distances[2] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        Sys_Send_IntEvent(SYS_EVENT_IO_PROX_2, dist);
        lowpass = 0;
    }else{
        lowpass++;
    }
}
void Sys_Prox3_preprocessor(uint value){
    static int lowpass = 0;
    
    uint dist = Sys_Prox_trasform(3, value);
    
    sys_distances[3] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        Sys_Send_IntEvent(SYS_EVENT_IO_PROX_3, dist);
        lowpass = 0;
    }else{
        lowpass++;
    }
}
void Sys_Prox4_preprocessor(uint value){
    static int lowpass = 0;
    
    uint dist = Sys_Prox_trasform(4, value);
    
    sys_distances[4] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        Sys_Send_IntEvent(SYS_EVENT_IO_PROX_4, dist);
        lowpass = 0;
    }else{
        lowpass++;
    }
}
void Sys_Prox5_preprocessor(uint value){
    static int lowpass = 0;
    
    uint dist = Sys_Prox_trasform(5, value);
    
    sys_distances[5] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        Sys_Send_IntEvent(SYS_EVENT_IO_PROX_5, dist);
        lowpass = 0;
    }else{
        lowpass++;
    }
    
}
void Sys_Prox6_preprocessor(uint value){
    static int lowpass = 0;
    
    uint dist = Sys_Prox_trasform(6, value);
    
    sys_distances[6] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        Sys_Send_IntEvent(SYS_EVENT_IO_PROX_6, dist);
        lowpass = 0;
    }else{
        lowpass++;
    }
    
}
void Sys_Prox7_preprocessor(uint value){
    static int lowpass = 0;
    
    uint dist = Sys_Prox_trasform(7, value);
    
    sys_distances[7] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        Sys_Send_IntEvent(SYS_EVENT_IO_PROX_7, dist);
        lowpass = 0;
    }else{
        lowpass++;
    }
    
}

uint Sys_Get_Prox(uint index){
    return sys_distances[index];
}
uint Sys_Get_MaxProx(uint index){
    return sys_calibration_value[index];
}