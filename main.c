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

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void task1();
void task2();

int16_t main(void)
{

    /* Configure the oscillator for the device */
    Sys_Init_Kernel();

    Sys_Start_Process_HDI(task1);
    Sys_Start_Process_HDI(task2);

    Sys_Start_Kernel();

    /* Initialize IO ports and peripherals */
    InitApp();

    sys_event_data * data = Sys_Wait_For_Event(TERMINATION);
    Sys_Clear_EventData(&data);
    
    int i = 0;
    while(1){//DO Nothing (do yonly things for testing)
        i++;
        i %= 100;
    }
}


void task1(){
    unsigned char x=0,y=0,z=0;
    while(1){
        x--;
        z++;
        y=x*z;
    }
}


void task2(){
    unsigned char a=0,b=0,c=0;
    bool ohno = false;
    while(1){

        a--;
        c++;
        b = a*a;
        c=b*a;

        if(ohno){
            Sys_Send_Event(TERMINATION, 0, 0);
        }
    }
}