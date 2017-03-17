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
 * \brief It defines functions to control and use the ADC-channels
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#include "adc.h"

#include "../../definitions.h"
#include "../../interrupts.h"
#include "../../system.h"

#define ADC_CHANNELS 16 /*!< Maximum amount of ADC-channels*/

static ADC_pre_processor adc_preprocessors[ADC_CHANNELS] = {0};  /*!< Defines the a ADC-preProcessor for each channel*/
static PRX_finisher prx_finisher = 0;
static MIC_finisher mic_finisher = 0;
static ACC_finisher acc_finisher = 0;

uint16 sys_random_number = 0; /*!< Defines a value for an random number -> gets defined by the ADC noise*/

/**
 *
 * This Function initialises all ADC-channels. All channels are sampled (except Debug-pins)---i.e. Microphone (3x), Accelerometer (3-axis), Proximity-sensors (8x).
 * 
 */
inline void Sys_Init_ADC(void){
    
    Sys_Reset_ADCProcessors();
    
    IEC0bits.ADIE = 0;
    IFS0bits.ADIF = 0; //ADC interrupt flag
    
    ADCON1bits.ADON     = 0;    // turn ADC off
        
    //empty control registers
    ADCON1 = 0;
    ADCON2 = 0;
    ADCON3 = 0;
    ADCHS = 0;    
    
    ADCON1bits.ADSIDL   = 0;    // Continue module operation in Idle mode
    ADCON1bits.FORM     = 0b00; // get integer (1 is fractional)
    ADCON1bits.SSRC     = 0b111;// Internal counter ends sampling and starts conversion (auto convert)
    ADCON1bits.ASAM     = 1;    // Sampling begins immediately after last conversion completes. SAMP bit is auto set
    ADCON1bits.SAMP     = 0;    // A/D sample/hold amplifiers are holding
    
    ADCON2bits.VCFG     = 0b000;//references AVDD AVSS
    ADCON2bits.CSCNA    = 1;    // Do not scan inputs - not sure
    ADCON2bits.SMPI     = 13;  // 14-1: Interrupts at the completion of conversion for each 14th sample/convert sequence
    ADCON2bits.BUFM     = 0;    // Buffer configured as one 16-word buffer ADCBUF(15...0)
    ADCON2bits.ALTS     = 0;    // Always use MUX A input multiplexer settings
    
    ADCON3bits.SAMC     = 31; //: Auto-Sample Time bits - bits between sampling and conversion//31
    ADCON3bits.ADRC     = 1;    //internal clock
    ADCON3bits.ADCS     = 63; //A/D Conversion Clock Select bits (TCY/2 * (ADCS+1)) // 63
    
    ADCHSbits.CH0NA     = 0;    //Select VREF- for CH0- input
            
    ADPCFGbits.PCFG0 = 1; // Debug = digital
    ADPCFGbits.PCFG1 = 1; // Debug 
    ADPCFGbits.PCFG2 = 0; // Microphone 1 = analog
    ADPCFGbits.PCFG3 = 0; // Microphone 2
    ADPCFGbits.PCFG4 = 0; // Microphone 3
    ADPCFGbits.PCFG5 = 0; // Accelerometer X
    ADPCFGbits.PCFG6 = 0; // Accelerometer Y
    ADPCFGbits.PCFG7 = 0; // Accelerometer Z
    ADPCFGbits.PCFG8 = 0; // Proximity 0
    ADPCFGbits.PCFG9 = 0; // Proximity 1
    ADPCFGbits.PCFG10 = 0;// Proximity 2
    ADPCFGbits.PCFG11 = 0;// Proximity 3
    ADPCFGbits.PCFG12 = 0;// Proximity 4
    ADPCFGbits.PCFG13 = 0;// Proximity 5
    ADPCFGbits.PCFG14 = 0;// Proximity 6
    ADPCFGbits.PCFG15 = 0;// Proximity 7
    
    
    ADCSSLbits.CSSL0 = 0; // Debug = no input
    ADCSSLbits.CSSL1 = 0; // Debug
    ADCSSLbits.CSSL2 = 1; // Microphone 1
    ADCSSLbits.CSSL3 = 1; // Microphone 2
    ADCSSLbits.CSSL4 = 1; // Microphone 3
    ADCSSLbits.CSSL5 = 1; // Accelerometer X
    ADCSSLbits.CSSL6 = 1; // Accelerometer Y
    ADCSSLbits.CSSL7 = 1; // Accelerometer Z
    ADCSSLbits.CSSL8 = 1; // Proximity 0
    ADCSSLbits.CSSL9 = 1; // Proximity 1
    ADCSSLbits.CSSL10 = 1;// Proximity 2
    ADCSSLbits.CSSL11 = 1;// Proximity 3
    ADCSSLbits.CSSL12 = 1;// Proximity 4
    ADCSSLbits.CSSL13 = 1;// Proximity 5
    ADCSSLbits.CSSL14 = 1;// Proximity 6
    ADCSSLbits.CSSL15 = 1;// Proximity 7

}

/**
 *
 * This Function starts the ADC sampling.
 * 
 */
inline void Sys_Start_ADC(void){
    IFS0bits.ADIF       = 0; //ADC interrupt flag
    IEC0bits.ADIE       = 1; //enable IRQ
    ADCON1bits.ADON     = 1;

}

/**
 *
 * The ADC interrupt routine reads the obtained ADC value and executes the preprocessors with it. It also uses the ADC noise to obtain a random value. 
 * 
 */
void __attribute__((interrupt, auto_psv)) _ADCInterrupt(void){
    
#ifndef SYS_ADC_USED    
    IFS0bits.ADIF       = 0; //ADC interrupt flag
    IEC0bits.ADIE       = 0; //enable IRQ
    ADCON1bits.ADON     = 0;
    return;
#endif
    
    volatile unsigned int *adcbuf;
    adcbuf = &ADCBUF0;
    int i;
    
    Sys_Start_AtomicSection();
    Sys_Inc_InterruptCounter();
    
    for( i = 0; i < ADC_CHANNELS-2; i++){
        if(adc_preprocessors[i+2] != 0){//+2 because I do not collect the debug ADC
            adc_preprocessors[i+2](adcbuf[i]);
        }
        sys_random_number = (sys_random_number << 1) | (adcbuf[i] & 0x0001); //use the last bit (adc noise) to generate the random number
    }
        
    if(prx_finisher != 0){
        prx_finisher();
    }
        
    if(mic_finisher != 0){
        mic_finisher();
    }
        
    if(acc_finisher != 0){
        acc_finisher();
    }
        
    
    Sys_End_AtomicSection();
    IFS0bits.ADIF = 0;  //After conversion ADIF is set to 1 and must be cleared
}


/**
 *
 * This function adds an ADC pre-processor.
 * 
 * @param[in] c ADC channel that requires a pre-processor
 * @param[in] func Pointer to the pre-processor for that channel
 */
void Sys_Subscribe_ADCChannelProcessor(channel c, ADC_pre_processor func){
    int i;
    for(i = 0; i < ADC_CHANNELS; i++){
        if(c & 0x0001){//is the last bit a 1 ?
            adc_preprocessors[i] = func;
        }
        
        c = c >> 1;
    }
}

/**
 *
 * This function resets all Preprocessors to 0. (no-preprocessor)
 * 
 */
void Sys_Reset_ADCProcessors(){
    int i;
    for(i = 0; i < ADC_CHANNELS; i++){
        adc_preprocessors[i] = 0;
    }
}

/**
 *
 * This function sets an ADC pre-processor to 0 (removing it). 
 * 
 * @param[in] c ADC channel that should have no pre-processor
 */
void Sys_Reset_ADCProcessor(channel c){
    Sys_Subscribe_ADCChannelProcessor(c, 0);
}

/**
 *
 * This function stops the ADC conversion.
 * 
 */
inline void Sys_Stop_ADC(){
    IFS0bits.ADIF       = 0; //ADC interrupt flag
    IEC0bits.ADIE       = 0; //enable IRQ
    ADCON1bits.ADON     = 0;
}

/**
 *
 * This function resets all Preprocessors to 0. (no-preprocessor)
 * 
 */
inline void Sys_Reset_ADC(){
    Sys_Reset_ADCProcessors();
}

/**
 *
 * This function deactivates the ADC module and stops any conversion.
 * 
 */
inline void Sys_Deactivate_ADC(void){
    Sys_Stop_ADC();
}

/**
 *
 * This function returns a 8-bit random number.
 * 
 * @return uint8 a random number generated by adc noise.
 * 
 */
uint8 Sys_Rand8(){
    return (uint8) sys_random_number & 0x00FF;
}

/**
 *
 * This function returns a 16-bit random number
 * 
 * @return uint16 a random number generated by adc noise.
 * 
 */
uint16 Sys_Rand16(){
    return sys_random_number;
}



void Sys_Subscribe_ProxFinisher(PRX_finisher func){
    prx_finisher = func;
}
void Sys_Subscribe_MicFinisher(MIC_finisher func){
    mic_finisher = func;
}
void Sys_Subscribe_AccFinisher(ACC_finisher func){
    acc_finisher = func;
}