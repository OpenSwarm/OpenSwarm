/*!
 * \file
 * \ingroup epuck
 * \ingroup hdi
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * \version 1.0
 *
 * \date 07 July 2014
 * 
 * \brief  Hardware dependent implementations to catch hardware traps.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 */


/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "../../../os/definitions.h"

/******************************************************************************/
/* Trap Function Prototypes                                                   */
/* Primary Interrupt Vector Names:                                            */
/*                                                                            */
/* _INT0Interrupt  _INT2Interrupt                                             */
/* _IC1Interrupt   _U2RXInterrupt                                             */
/* _OC1Interrupt   _U2TXInterrupt                                             */
/* _T1Interrupt    _SPI2Interrupt                                             */
/* _IC2Interrupt   _C1Interrupt                                               */
/* _OC2Interrupt   _IC3Interrupt                                              */
/* _T2Interrupt    _IC4Interrupt                                              */
/* _T3Interrupt    _IC5Interrupt                                              */
/* _SPI1Interrupt  _IC6Interrupt                                              */
/* _U1RXInterrupt  _OC5Interrupt                                              */
/* _U1TXInterrupt  _OC6Interrupt                                              */
/* _ADCInterrupt   _OC7Interrupt                                              */
/* _NVMInterrupt   _OC8Interrupt                                              */
/* _SI2CInterrupt  _INT3Interrupt                                             */
/* _MI2CInterrupt  _INT4Interrupt                                             */
/* _CNInterrupt    _C2Interrupt                                               */
/* _INT1Interrupt  _PWMInterrupt                                              */
/* _IC7Interrupt   _QEIInterrupt                                              */
/* _IC8Interrupt   _DCIInterrupt                                              */
/* _OC3Interrupt   _LVDInterrupt                                              */
/* _OC4Interrupt   _FLTAInterrupt                                             */
/* _T4Interrupt    _FLTBInterrupt                                             */
/* _T5Interrupt                                                               */
/******************************************************************************/

/* <Other function prototypes for debugging trap code may be inserted here> */

/* Use if INTCON2 ALTIVT=1 */
void __attribute__((interrupt,no_auto_psv)) _OscillatorFail(void);
void __attribute__((interrupt,no_auto_psv)) _AddressError(void);
void __attribute__((interrupt,no_auto_psv)) _StackError(void);
void __attribute__((interrupt,no_auto_psv)) _MathError(void);

/* Use if INTCON2 ALTIVT=0 */
void __attribute__((interrupt,no_auto_psv)) _AltOscillatorFail(void);
void __attribute__((interrupt,no_auto_psv)) _AltAddressError(void);
void __attribute__((interrupt,no_auto_psv)) _AltStackError(void);
void __attribute__((interrupt,no_auto_psv)) _AltMathError(void);

/* Default interrupt handler */
void __attribute__((interrupt,no_auto_psv)) _DefaultInterrupt(void);

/******************************************************************************/
/* Trap Handling                                                              */
/*                                                                            */
/* These trap routines simply ensure that the device continuously loops       */
/* within each routine.  Users who actually experience one of these traps     */
/* can add code to handle the error.  Some basic examples for trap code,      */
/* including assembly routines that process trap sources, are available at    */
/* www.microchip.com/codeexamples                                             */
/******************************************************************************/

/* Primary (non-alternate) address error trap function declarations */
/**
 * Oscillator fail trap.
 *
 * This function is called when an oscillator fail occurs. This should never happen.
 * 
 */
void __attribute__((interrupt,no_auto_psv)) _OscillatorFail(void)
{
    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;

        INTCON1bits.OSCFAIL = 0;        /* Clear the trap flag */
        while (1){
            ;
        }
}

/**
 * Address error trap.
 *
 * This function is called when an address error occurs. That means that a call address of a function or in the stack addresses an area outside the memory. Similarly, if a pointer points to memory outside the range, this trap happens.
 * 
 */
void __attribute__((interrupt,no_auto_psv)) _AddressError(void)
{
    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 1;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;

        INTCON1bits.ADDRERR = 0;        /* Clear the trap flag */
        while (1){

    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 1;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
        }
}

/**
 * Stack error trap.
 *
 * This function is called when an stack error occurs. That means that the stack pointer, stack pointer limit, or frame pointer are pointing outside their range.
 * 
 */
void __attribute__((interrupt,no_auto_psv)) _StackError(void)
{
    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;

        INTCON1bits.STKERR = 0;         /* Clear the trap flag */
        while (1){
    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
        }
}

/**
 * Math error trap.
 *
 * This function is called when an math error occurs. That means an illegal math operation was performed (such as division by 0 or NaN).
 * 
 */
void __attribute__((interrupt,no_auto_psv)) _MathError(void)
{
   FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
        INTCON1bits.MATHERR = 0;        /* Clear the trap flag */
        while (1){
    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 0;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
        }
}

/* Alternate address error trap function declarations */
/**
 * Alternative Oscillator fail trap.
 *
 * This function is called when an oscillator fail occurs. This should never happen.
 * 
 */
void __attribute__((interrupt,no_auto_psv)) _AltOscillatorFail(void)
{
    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
        INTCON1bits.OSCFAIL = 0;        /* Clear the trap flag */
        while (1){
    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 0;
    LED6 = 0;
    LED7 = 0;
        }
}

/**
 * Alternative address error trap.
 *
 * This function is called when an address error occurs. That means that a call address of a function or in the stack addresses an area outside the memory. Similarly, if a pointer points to memory outside the range, this trap happens.
 * 
 */
void __attribute__((interrupt,no_auto_psv)) _AltAddressError(void)
{
    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 0;
    LED7 = 0;
        INTCON1bits.ADDRERR = 0;        /* Clear the trap flag */
        while (1){
    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 0;
    LED7 = 0;
        }
}

/**
 * Alternative stack error trap.
 *
 * This function is called when an stack error occurs. That means that the stack pointer, stack pointer limit, or frame pointer are pointing outside their range.
 * 
 */
void __attribute__((interrupt,no_auto_psv)) _AltStackError(void)
{
    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 0;
        INTCON1bits.STKERR = 0;         /* Clear the trap flag */
        while (1){
    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 0;
        }
}

/**
 * Alternative math error trap.
 *
 * This function is called when an math error occurs. That means an illegal math operation was performed (such as division by 0 or NaN).
 * 
 */
void __attribute__((interrupt,no_auto_psv)) _AltMathError(void)
{
    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1;
        INTCON1bits.MATHERR = 0;        /* Clear the trap flag */
        while (1){
    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1;
        }
}

/******************************************************************************/
/* Default Interrupt Handler                                                  */
/*                                                                            */
/* This executes when an interrupt occurs for an interrupt source with an     */
/* improperly defined or undefined interrupt handling routine.                */
/******************************************************************************/
/**
 * Default interrupt service routine.
 *
 * This function is called when no other interrupt routine is specified. 
 */
void __attribute__((interrupt,no_auto_psv)) _DefaultInterrupt(void)
{
    FRONT_LED = 1;
    BODY_LED = 1;
    LED0 = 0;
    LED1 = 1;
    LED2 = 0;
    LED3 = 1;
    LED4 = 0;
    LED5 = 1;
    LED6 = 0;
    LED7 = 1;
        while (1){
            ;
        }
}

