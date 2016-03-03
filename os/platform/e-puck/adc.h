/* 
 * File:   adc.h
 * Author: strenkwalder
 *
 * Created on 03 March 2016, 13:48
 */

#ifndef ADC_H
#define	ADC_H

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
#define Prox0   channel8
#define Prox1   channel9
#define Prox2   channelA
#define Prox3   channelB
#define Prox4   channelC
#define Prox5   channelD
#define Prox6   channelE
#define Prox7   channelF
#define IR0     channel8
#define IR1     channel9
#define IR2     channelA
#define IR3     channelB
#define IR4     channelC
#define IR5     channelD
#define IR6     channelE
#define IR7     channelF
    
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
    channelF = 0b1000000000000000           
} channel;
    
typedef void (*ADC_pre_processor)(uint);

void Sys_Init_ADC(void);
void Sys_Start_ADC(void);

void Sys_Subscribe_ADCChannelProcessor(channel c, ADC_pre_processor func);

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

