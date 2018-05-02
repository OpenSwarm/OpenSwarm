/*!
 * \file
 * \ingroup adc
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * \version 1.0
 *
 * \date Jan 2016
 * 
 * \brief It declares functions to control and use the ADC-channels
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

/*! \defgroup adc ADC
 * \ingroup io
 * \ingroup epuck
 * \brief Functions to control and use the ADC-channels. 
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * The ADC module controls which inputs are measured and how fast they are sampled (on an \ref epuck ). This module samples the following:
 * - Microphone 1
 * - Microphone 2
 * - Microphone 3
 * - Accelerometer (x-axis)
 * - Accelerometer (y-axis)
 * - Accelerometer (z-axis)
 * - Phototransistor 0 (= Proximity sensor)
 * - Phototransistor 1 (= Proximity sensor)
 * - Phototransistor 2 (= Proximity sensor)
 * - Phototransistor 3 (= Proximity sensor)
 * - Phototransistor 4 (= Proximity sensor)
 * - Phototransistor 5 (= Proximity sensor)
 * - Phototransistor 6 (= Proximity sensor)
 * - Phototransistor 7 (= Proximity sensor)
 * 
 * It also provides 8/16-bit random numbers generated from ADC noise.
 * 
 * \section adc_usage Usage
 * After the initialisation with Sys_Init_ADC() and start with Sys_Init_ADC(), the ADC samples all 14 inputs continuously. Each value obtained is then processed by an pre-processor, if one was registered by Sys_Subscribe_ADCChannelProcessor(channel, ADC_pre_processor). For each channel there can be a separate pre-processor is required.
 * In case the conversion needs to be stopped, Sys_Stop_ADC() provides this option. Sys_Reset_ADC() and Sys_Reset_ADCProcessors() remove all subscribed pre-processors. A single preprocessor can be removed with Sys_Reset_ADCProcessor(channel c). Sys_Deactivate_ADC() can disable the ADC module and removes all related data structures.
 * 
 * The random numbers can be obtained by Sys_Rand8() Sys_Rand16(). These functions do not need initialisation (no srand or similar) and change every time new ADC values are obtained. If several random numbers are needed in a short period, we recommend the use of srand and rand from stdlib with a seed obtained by sensor noise. 
 * 
 * \section adc_license License
 * LICENSE: adapted FreeBSD License (see http://openswarm.org/license)\n
 * Copyright (c) 2016, Stefan M. Trenkwalder\n
 * All rights reserved. 
 */

#ifndef ADC_H
#define	ADC_H

#include "../../definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define Dbg0    channel0 /*!< Defines the channel for debug0-pin*/
#define Dbg1    channel1 /*!< Defines the channel for debug1-pin*/
#define Mic0    channel2 /*!< Defines the channel for mic0-pin*/
#define Mic1    channel3 /*!< Defines the channel for mic1-pin*/
#define Mic2    channel4 /*!< Defines the channel for mic2-pin*/
#define AccX    channel5 /*!< Defines the channel for accX-pin*/
#define AccY    channel6 /*!< Defines the channel for accY-pin*/
#define AccZ    channel7 /*!< Defines the channel for accZ-pin*/
#define Prx0   channel8 /*!< Defines the channel for proximity0-pin*/
#define Prx1   channel9 /*!< Defines the channel for proximity1-pin*/
#define Prx2   channelA /*!< Defines the channel for proximity2-pin*/
#define Prx3   channelB /*!< Defines the channel for proximity3-pin*/
#define Prx4   channelC /*!< Defines the channel for proximity4-pin*/
#define Prx5   channelD /*!< Defines the channel for proximity5-pin*/
#define Prx6   channelE /*!< Defines the channel for proximity6-pin*/
#define Prx7   channelF /*!< Defines the channel for proximity7-pin*/
    
#define  ADC_ACQUISITION_TIME 31
#define  ADC_CONVERSION_TIME 14
#define  ADC_SCALER_TIME 64//16
    
typedef enum {
    channel0 = 0b0000000000000001,
    channel1 = 0b0000000000000010,
    channel2 = 0b0000000000000100,
    channel3 = 0b0000000000001000,
    channel4 = 0b0000000000010000,
    channel5 = 0b0000000000100000,
    channel6 = 0b0000000001000000,
    channel7 = 0b0000000010000000,
    channel8 = 0b0000000100000000,
    channel9 = 0b0000001000000000,
    channelA = 0b0000010000000000,
    channelB = 0b0000100000000000,
    channelC = 0b0001000000000000, 
    channelD = 0b0010000000000000,
    channelE = 0b0100000000000000,
    channelF = 0b1000000000000000,
    PRXchannels = 0b1111111100000000,
    MICchannels = 0b0000000000011100,
    ACCchannels = 0b0000000011100000,
    DBGchannels = 0b0000000000000011,
    ALLchannels = 0xFFFF,
} channel; /*!< Defines the channels for the adc*/
   

typedef enum {
    ProxDone   = 0b1000,
    MicDone    = 0b0100,
    AccDone    = 0b0010,
    DbgDone    = 0b0001,
    AllDone    = 0b1111,
} sensorGroup; /*!< Defines the groups of sensor channels*/

typedef void (*ADC_pre_processor)(uint); /*!< Defines the structure of an ADC-preProcessor*/

inline void Sys_Init_ADC(void);
inline void Sys_Start_ADC(void);
inline void Sys_Stop_ADC(void);
inline void Sys_Reset_ADC(void);
inline void Sys_Deactivate_ADC(void);

void Sys_Subscribe_ADCChannelProcessor(channel c, ADC_pre_processor func);
void Sys_Subscribe_ADCFinish(sensorGroup flag, pFunction func);

void Sys_Reset_ADCProcessors(void);
void Sys_Reset_ADCProcessor(channel c);

uint8 Sys_Rand8();
uint16 Sys_Rand16();


#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

