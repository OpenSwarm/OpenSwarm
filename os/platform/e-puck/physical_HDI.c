#include "physical_HDI.h"

#include "../../system.h"
#include "adc.h"

#define PRESCALER 8

volatile pFunction Sys_PhysicalChannelSender = 0;

inline void Sys_Init_PhysicalSendingChannel(void){
    
    T4CONbits.T32 = 0; //two 16 bit interrupts
    
    IFS1bits.T5IF = 0; //unsets the Timer1 interrupt flag
    IEC1bits.T5IE = 0; //disable Timer1 interrupt -> T1IE = 0

    T5CONbits.TON = 0; //stops counting 
    
    TMR5 = 0;
    PR5 = 14*(ADC_SCALER_TIME * ADCs_PER_BIT)/(2 * PRESCALER) * (ADC_ACQUISITION_TIME+ADC_CONVERSION_TIME);

    // T5CON
    // [TON] [-] [TSIDL] [-] [-] [-] [-] [-] [-] [TGATE] [TCKPS1] [TCKPS0] [-] [TSYNC] [TCS] [-]
    // TON          = enables the Timer1
    // TSIDL        = Timer1 goes to sleep/idle (when processor goes sleeping)
    // TGATE        = Timer gets triggert from external source
    // TCKPS<0:1>   = sets timer prescaler [1, 8, 64, 256]
    // TSYNC        = enables the timer to be synchronised with external source (rising edge)
    // TCS          = sets clock source to external (1) or internal (0)
    T5CON = 0; //default
    T5CONbits.TCKPS = 0x01; //Prescaler 8
    
    
    IPC5bits.T5IP = 7;
}
inline void Sys_Start_PhysicalSendingChannel(void){
    Sys_Continue_PhysicalSendingChannel();
}

inline void Sys_Stop_PhysicalSendingChannel(void){
    Sys_Pause_PhysicalSendingChannel();
}

inline void Sys_Pause_PhysicalSendingChannel(void){
    IFS1bits.T5IF = 0; //unsets the Timer1 interrupt flag
    IEC1bits.T5IE = 0; //disable Timer1 interrupt -> T1IE = 0

    T5CONbits.TON = 0; //stops counting 
}


inline void Sys_Continue_PhysicalSendingChannel(void){
    IFS1bits.T5IF = 0; //unsets the Timer1 interrupt flag
    IEC1bits.T5IE = 1; //disable Timer1 interrupt -> T1IE = 0

    T5CONbits.TON = 1; //stops counting 
}
inline void Sys_Disable_PhysicalSendingChannelInterrupt(void){
    IFS1bits.T5IF = 0; //unsets the Timer1 interrupt flag
    IEC1bits.T5IE = 0; //disable Timer1 interrupt -> T1IE = 0
}
inline void Sys_Enable_PhysicalSendingChannelInterrupt(void){
    IEC1bits.T5IE = 1;  //enable Timer1 interrupt -> T1IE = 1
}
inline void Sys_Force_PhysicalSendingChannelInterrupt(void){
    T5CONbits.TON = 1; //enable Timer1 interrupt -> T1IE = 1
}

void __attribute__((interrupt,no_auto_psv)) _T5Interrupt(void){
    
 //   LED4 = LED0;
 //   LED0 = ~LED4;
    
    if(Sys_PhysicalChannelSender != 0){
        Sys_PhysicalChannelSender();
    }
    IFS1bits.T5IF = 0; //unsets the Timer1 interrupt flag
}

/**
 *
 * This Function starts the task-scheduling algorithm
 *
 */
void __attribute__((interrupt,no_auto_psv)) _AltT5Interrupt(void){
     
    if(Sys_PhysicalChannelSender != 0){
        Sys_PhysicalChannelSender();
    }    
    
    IFS1bits.T5IF = 0; //unsets the Timer1 interrupt flag
}

void Sys_Register_SendingFunction(pFunction func){
    Sys_PhysicalChannelSender = func;
}