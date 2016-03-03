#include "adc.h"

#include "../../definitions.h"

void Sys_Init_ADC(void){
    
    //empty control registers
    ADCON1 = 0;
    ADCON2 = 0;
    ADCON3 = 0;
    
    ADCON1bits.ADON     = 0;    // turn ADC off
    ADCON1bits.ADSIDL   = 0;    // Continue module operation in Idle mode
    ADCON1bits.FORM     = 0b00; // get integer (1 is fractional)
    ADCON1bits.SSRC     = 0b111;// Internal counter ends sampling and starts conversion (auto convert)
    ADCON1bits.ASAM     = 1;    // Sampling begins immediately after last conversion completes. SAMP bit is auto set
    ADCON1bits.SAMP     = 0;    // A/D sample/hold amplifiers are holding
    
    ADCON2bits.VCFG     = 0b000;//references AVDD AVSS
    ADCON2bits.CSCNA    = 0;    // Do not scan inputs - not sure
    ADCON2bits.SMPI     = 0xF;  // Interrupts at the completion of conversion for each 16th sample/convert sequence
    ADCON2bits.BUFM     = 0;    // Buffer configured as one 16-word buffer ADCBUF(15...0)
            
    
}

void Sys_Start_ADC(void);

void Sys_Subscribe_ADCChannelProcessor(channel c, ADC_pre_processor func);