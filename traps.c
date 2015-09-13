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
#include "HDI_epuck_ports.h"
#include "definitions.h"

/******************************************************************************/
/* Trap Function Prototypes                                                   */
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

