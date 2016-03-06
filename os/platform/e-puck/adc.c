#include "adc.h"

#include "../../definitions.h"
#include "../../interrupts.h"

#define ADC_CHANNELS 16

static ADC_pre_processor adc_preprocessors[ADC_CHANNELS] = {0};

inline void Sys_Init_ADC(void){
    
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
    
    ADCON3bits.SAMC     = 31; //: Auto-Sample Time bits
    ADCON3bits.ADRC     = 1;    //internal clock
    ADCON3bits.ADCS     = 63; //A/D Conversion Clock Select bits (TCY/2 * (ADCS+1))
    
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
    
    PULSE_IR0 = 1;
    PULSE_IR1 = 1;
    PULSE_IR2 = 1;
    PULSE_IR3 = 1;

}

inline void Sys_Start_ADC(void){
    IFS0bits.ADIF       = 0; //ADC interrupt flag
    IEC0bits.ADIE       = 1; //enable IRQ
    ADCON1bits.ADON     = 1;

}

void __attribute__((interrupt, auto_psv)) _ADCInterrupt(void){
    
    volatile unsigned int *adcbuf;
    adcbuf = &ADCBUF0;
    int i;
    
    Sys_Start_AtomicSection();
    Sys_Inc_InterruptCounter();
    
    for( i = 0; i < ADC_CHANNELS-2; i++){
        if(adc_preprocessors[i+2] != 0){//+2 because I do not collect the debug ADC
            adc_preprocessors[i+2](adcbuf[i]);
        }
    }
    Sys_End_AtomicSection();
    IFS0bits.ADIF = 0;  //After conversion ADIF is set to 1 and must be cleared
}

void Sys_Subscribe_ADCChannelProcessor(channel c, ADC_pre_processor func){
    int i;
    for(i = 0; i < ADC_CHANNELS; i++){
        if(c & 0x0001){//is the last bit a 1 ?
            adc_preprocessors[i] = func;
        }
        
        c = c >> 1;
    }
}



void Sys_Reset_ADCProcessors(){
    int i;
    for(i = 0; i < ADC_CHANNELS; i++){
        adc_preprocessors[i] = 0;
    }
}

inline void Sys_Stop_ADC(){
    IFS0bits.ADIF       = 0; //ADC interrupt flag
    IEC0bits.ADIE       = 0; //enable IRQ
    ADCON1bits.ADON     = 0;
}

inline void Sys_Reset_ADC(){
    Sys_Reset_ADCProcessors();
}