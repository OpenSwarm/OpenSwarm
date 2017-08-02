/*!
 * \file
 * \ingroup prox
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * \version 1.0
 *
 * \date Jan 2016
 * 
 * \brief It defines functions to measure distances to objects. (Proximity-sensors)
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */
#include "adc.h"
#include "../../definitions.h"
#include "../../events/events.h"

#define PROX_CALIBRATION_N 11 /*!< The number of elements in the liniarisiation array*/

uint sys_calibration_value[8] = {0}; /*!< struct to store calibration values for each sensor (maximum value)*/
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
                                    {100, 53  }};/*!< linearisation array (mm, sensor value)*/

uint Sys_Prox_trasform(uint, uint);
void Sys_Prox0_preprocessor(uint);
void Sys_Prox1_preprocessor(uint);
void Sys_Prox2_preprocessor(uint);
void Sys_Prox3_preprocessor(uint);
void Sys_Prox4_preprocessor(uint);
void Sys_Prox5_preprocessor(uint);
void Sys_Prox6_preprocessor(uint);
void Sys_Prox7_preprocessor(uint);

uint sys_distances[8] = {0}; /*!< last transformed sensor reading (stored in mm)*/
uint sys_raw[8] = {0}; /*!< last transformed sensor reading (stored in mm)*/

/**
 *
 * This function initialises the Proximity module. It registers all events SYS_EVENT_IO_PROX_[0-7] and subscribes preprocessors to each channel.
 * 
 */
void Sys_Init_Proximity(void){

    Sys_Init_ADC();//make sure ADCs are initialised

    Sys_Register_Event(SYS_EVENT_IO_PROX_0);
    Sys_Register_Event(SYS_EVENT_IO_PROX_1);
    Sys_Register_Event(SYS_EVENT_IO_PROX_2);
    Sys_Register_Event(SYS_EVENT_IO_PROX_3);
    Sys_Register_Event(SYS_EVENT_IO_PROX_4);
    Sys_Register_Event(SYS_EVENT_IO_PROX_5);
    Sys_Register_Event(SYS_EVENT_IO_PROX_6);
    Sys_Register_Event(SYS_EVENT_IO_PROX_7);

    Sys_Subscribe_ADCChannelProcessor(Prx0, Sys_Prox0_preprocessor);
    /*   
    Sys_Subscribe_ADCChannelProcessor(Prx1, Sys_Prox0_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx2, Sys_Prox0_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx3, Sys_Prox0_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx4, Sys_Prox7_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx5, Sys_Prox7_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx6, Sys_Prox7_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx7, Sys_Prox7_preprocessor);
    */
    Sys_Subscribe_ADCChannelProcessor(Prx1, Sys_Prox1_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx2, Sys_Prox2_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx3, Sys_Prox3_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx4, Sys_Prox4_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx5, Sys_Prox5_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx6, Sys_Prox6_preprocessor);
    Sys_Subscribe_ADCChannelProcessor(Prx7, Sys_Prox7_preprocessor); 
}

/**
 *
 * This function is empty. This module doesn't need to be started.
 * 
 */
void Sys_Start_Proximity(void){
}

/**
 *
 * This function is empty. This module doesn't need to be stopped.
 * 
 */
void Sys_Stop_Proximity(void){
}

/**
 *
 * This function deactivates the Proximity module. This includes unregistering events and removing ADC preprocessors.
 * 
 */
void Sys_Deactivate_Proximity(void){
    Sys_Unregister_Event(SYS_EVENT_IO_PROX_0);
    Sys_Unregister_Event(SYS_EVENT_IO_PROX_1);
    Sys_Unregister_Event(SYS_EVENT_IO_PROX_2);
    Sys_Unregister_Event(SYS_EVENT_IO_PROX_3);
    Sys_Unregister_Event(SYS_EVENT_IO_PROX_4);
    Sys_Unregister_Event(SYS_EVENT_IO_PROX_5);
    Sys_Unregister_Event(SYS_EVENT_IO_PROX_6);
    Sys_Unregister_Event(SYS_EVENT_IO_PROX_7);
    
    Sys_Reset_ADCProcessor(PRXchannels); 
}

/**
 *
 * This function transforms the sensor value into a linearised distance value.
 * 
 * @return the distance to the object in mm (or return 0xFFFF if out of range)
 * 
 */
uint Sys_Prox_trasform(uint i, uint value){
    if(i > 8)
        return 0;
    
    if(sys_calibration_value[i] < value){//if this value is the maximum value that has been received
        sys_calibration_value[i] = value;
    }
    
    uint cal = sys_calibration_value[i] - value;//calibrate the 0-distance
    
    if(cal == 0){
        return 0;//object touches robot
    }
    
    int j;
    for(j = 0; j < PROX_CALIBRATION_N; j++){//check which tuple is closest 
        if( sys_transformation[j][1] > cal){
            continue;
        }
        //if you found it -> linearise towards the smaller stored point
        return sys_transformation[j][0] - ((10 * (cal - sys_transformation[j][1])) / (sys_transformation[j-1][1]-sys_transformation[j][1]));
    }
    
    //if the sensor value is too small -> out of range
    return 0xFFFF;
    
}

/**
 *
 * This function is the preprocessor for proximity sensor 0. It linearises the obtained value and sends it as event to subscribers.
 * 
 * @param[in] value the obtain sensor value
 * 
 */
void Sys_Prox0_preprocessor(uint value){
    static int lowpass = 0;
        
    sys_raw[0] = value;
    uint dist = Sys_Prox_trasform(0, value);
    
    sys_distances[0] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        lowpass = 0;
        Sys_Send_Event(SYS_EVENT_IO_PROX_0, &dist, 2);
    }else{
        lowpass++;
    }
}

/**
 *
 * This function is the preprocessor for proximity sensor 1. It linearises the obtained value and sends it as event to subscribers.
 * 
 * @param[in] value the obtain sensor value
 * 
 */
void Sys_Prox1_preprocessor(uint value){
    static int lowpass = 0;
    
    sys_raw[1] = value;    
    uint dist = Sys_Prox_trasform(1, value);
    
    sys_distances[1] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        Sys_Send_Event(SYS_EVENT_IO_PROX_1, &dist, 2);
        lowpass = 0;
    }else{
        lowpass++;
    }
}

/**
 *
 * This function is the preprocessor for proximity sensor 2. It linearises the obtained value and sends it as event to subscribers.
 * 
 * @param[in] value the obtain sensor value
 * 
 */
void Sys_Prox2_preprocessor(uint value){
    static int lowpass = 0;
    sys_raw[2] = value;
    uint dist = Sys_Prox_trasform(2, value);
    
    sys_distances[2] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        Sys_Send_Event(SYS_EVENT_IO_PROX_2, &dist, 2);
        lowpass = 0;
    }else{
        lowpass++;
    }
}

/**
 *
 * This function is the preprocessor for proximity sensor 3. It linearises the obtained value and sends it as event to subscribers.
 * 
 * @param[in] value the obtain sensor value
 * 
 */
void Sys_Prox3_preprocessor(uint value){
    static int lowpass = 0;
    sys_raw[3] = value;
    uint dist = Sys_Prox_trasform(3, value);
    
    sys_distances[3] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        Sys_Send_Event(SYS_EVENT_IO_PROX_3, &dist, 2);
        lowpass = 0;
    }else{
        lowpass++;
    }
}

/**
 *
 * This function is the preprocessor for proximity sensor 4. It linearises the obtained value and sends it as event to subscribers.
 * 
 * @param[in] value the obtain sensor value
 * 
 */
void Sys_Prox4_preprocessor(uint value){
    static int lowpass = 0;
    sys_raw[4] = value;
    uint dist = Sys_Prox_trasform(4, value);
    
    sys_distances[4] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        Sys_Send_Event(SYS_EVENT_IO_PROX_4, &dist, 2);
        lowpass = 0;
    }else{
        lowpass++;
    }
}

/**
 *
 * This function is the preprocessor for proximity sensor 5. It linearises the obtained value and sends it as event to subscribers.
 * 
 * @param[in] value the obtain sensor value
 * 
 */
void Sys_Prox5_preprocessor(uint value){
    static int lowpass = 0;
    sys_raw[5] = value;
    uint dist = Sys_Prox_trasform(5, value);
    
    sys_distances[5] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        Sys_Send_Event(SYS_EVENT_IO_PROX_5, &dist, 2);
        lowpass = 0;
    }else{
        lowpass++;
    }
    
}

/**
 *
 * This function is the preprocessor for proximity sensor 6. It linearises the obtained value and sends it as event to subscribers.
 * 
 * @param[in] value the obtain sensor value
 * 
 */
void Sys_Prox6_preprocessor(uint value){
    static int lowpass = 0;
    sys_raw[6] = value;
    uint dist = Sys_Prox_trasform(6, value);
    
    sys_distances[6] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        Sys_Send_Event(SYS_EVENT_IO_PROX_6, &dist, 2);
        lowpass = 0;
    }else{
        lowpass++;
    }
    
}

/**
 *
 * This function is the preprocessor for proximity sensor 7. It linearises the obtained value and sends it as event to subscribers.
 * 
 * @param[in] value the obtain sensor value
 * 
 */
void Sys_Prox7_preprocessor(uint value){
    static int lowpass = 0;
    sys_raw[7] = value;
    uint dist = Sys_Prox_trasform(7, value);
    
    sys_distances[7] = dist;
    if(dist < 0xFFFF || lowpass == 25){
        Sys_Send_Event(SYS_EVENT_IO_PROX_7, &dist, 2);
        lowpass = 0;
    }else{
        lowpass++;
    }
    
}

/**
 *
 * This function returns the latest calculated distance value for a particular sensor.
 * 
 * @param[in]   index the index which sensor value is requested (Sensor 0 => index=0)
 * @return      returns the latest calculated distance value for sensor index
 */
uint Sys_Get_Prox(uint index){
    return sys_distances[index];
}

/**
 *
 * This function returns the latest maximum sensor value which is used to auto-calibrate the 0mm distance. This function was used for debugging.
 * 
 * @param[in]   index the index which value is requested (Sensor 0 => index=0)
 * @return      returns the maximum sensor value for sensor index
 */
uint Sys_Get_MaxProx(uint index){
    return sys_calibration_value[index];
}


/**
 *
 * This function returns the latest calculated distance value for a particular sensor.
 * 
 * @param[in]   index the index which sensor value is requested (Sensor 0 => index=0)
 * @return      returns the latest calculated distance value for sensor index
 */
uint Sys_Get_Raw(uint index){
    return sys_raw[index];
}
