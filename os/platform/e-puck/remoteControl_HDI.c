/*!
 * \file
 * \ingroup remotecontrol
 * \ingroup hdi
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \author  Yuri Kaszubowski Lopes <yurikazuba@gmail.com>
 * 
 * \version 1.0
 *
 * \date 27 August 2015
 * 
 * \brief  Hardware dependent implementations to receive and decode messages from a remote control
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 */

#include "remoteControl_HDI.h"

#include "../../definitions.h"
#include "../../interrupts.h"
#include "../../io/io.h"
#include "remoteControl.h"
#include "../../events/events.h"

bool message_arriving = false; /*!< A flag that is set as soon as a messgage is recieved */
sint8 waiting_cycles = 20;/*!< The cycles that need to be waited until the next stage (set for 100us) */

uint rx_buffer = 0;/*!< The initial state of the state machine to decode a remote control message */

bool isNewDataAvailable = false;/*!< a flag to indicate that a new message was received */

sint8 receiving_bit = RC_NOT_STARTED;/*!< State indicator (for the state machine) */

/**
 *
 * This function initialises the handler of the remote control to receive signals from the remote control.
 *
 */
inline void Sys_Init_RemoteControl_HDI(void){
    REMOTE_DIR = INPUT_PIN; 
    
    IPC0bits.INT0IP = SYS_IRQP_REMOTECONTROL;
    INTCON2bits.INT0EP = 1;     //use falling edge
    IFS0bits.INT0IF = 0;        //clear to IRQ flag
    
    if(!Sys_Register_IOHandler(Sys_Receive_RemoteControl_Msg)){// no error handling used
        return;
    }
    
    Sys_Register_Event(SYS_EVENT_IO_REMOECONTROL);
}

/**
 *
 * This function start the handler of the remote control to receive signals from the remote control.
 *
 */
inline void Sys_Start_RemoteControl_HDI(void){
    IFS0bits.INT0IF = 0;        ///clear to IRQ flag
    IEC0bits.INT0IE = 1;        //enable external INT0  
}

/**
 *
 * This function is executed at the arrival of a new remote control message.
 *
 */
void __attribute__((__interrupt__, auto_psv))  _INT0Interrupt(void){
    //When a message arrives deactivate interrupt -> is activated at the end of the message
    IEC0bits.INT0IE = 0;
        IFS0bits.INT0IF = 0;
    message_arriving = true;
        waiting_cycles = RC_WAIT_INITIALLY;
    return;
}
