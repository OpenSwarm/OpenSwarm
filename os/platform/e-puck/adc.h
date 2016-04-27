/* 
 * File:   adc.h
 * Author: strenkwalder
 *
 * Created on 03 March 2016, 13:48
 */

#ifndef ADC_H
#define	ADC_H

#include "../../definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define Dbg0    channel0
#define Dbg1    channel1
#define Mic0    channel2
#define Mic1    channel3
#define Mic2    channel4
#define AccX    channel5
#define AccY    channel6
#define AccZ    channel7
#define Prx0   channel8
#define Prx1   channel9
#define Prx2   channelA
#define Prx3   channelB
#define Prx4   channelC
#define Prx5   channelD
#define Prx6   channelE
#define Prx7   channelF
    
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
} channel;
    
typedef void (*ADC_pre_processor)(uint);

inline void Sys_Init_ADC(void);
inline void Sys_Start_ADC(void);
inline void Sys_Stop_ADC(void);
inline void Sys_Reset_ADC(void);

void Sys_Subscribe_ADCChannelProcessor(channel c, ADC_pre_processor func);

void Sys_Reset_ADCProcessors(void);

uint8 Sys_Rand8();
uint16 Sys_Rand16();


#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

