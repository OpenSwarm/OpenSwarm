/**
 * @file traps.c
 * @author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Created on 07 July 2014
 *
 * LICENSE: adapted FreeBSD License
 * Copyright (c) 2015, Stefan M. Trenkwalder
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. If this or parts of this source code (as code or binary) is, in any form, used for an commercial product or service (in any form), this product or service must provide a clear notice/message to any user/customer that OpenSwarm was used in this product.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * @section DESCRIPTION
 *
 * This file includes all hardware dependent functions to catch hhardware traps.
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

