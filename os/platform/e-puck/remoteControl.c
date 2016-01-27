/*!
 * \file
 * \ingroup remotecontrol
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \author  Yuri Kaszubowski Lopes <yurikazuba@gmail.com>
 * 
 * \version 1.0
 *
 * \date 27 August 2015
 * 
 * \brief  It defines functions to receive and decode messages from a remote control
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 */


#include "remoteControl.h"
#include "remoteControl_HDI.h"

#include <stdbool.h>

#include "../../io/io.h"
#include "../../events/events.h"
#include "../../definitions.h"
#include "../../interrupts.h"


/**
 *
 * This function initialises the handler of the remote control to receive signals from the remote control.
 *
 */
inline void Sys_Init_RemoteControl(void){
    Sys_Init_RemoteControl_HDI();
}

/**
 *
 * This function start the handler of the remote control to receive signals from the remote control.
 *
 */
inline void Sys_Start_RemoteControl(void){
    Sys_Start_RemoteControl_HDI();
}
//https://en.wikipedia.org/wiki/RC-5
/**
 *
 * This function reads a remote control signal and reads it's transmitted value. When a signal arrives, an external interrupt is triggered. The remaining values are obtained by using time not interrupts.
 *
 */
void Sys_Receive_RemoteControl_Msg(){
    if(!message_arriving){//is it the first bit?
        return;
    }
    
    if(waiting_cycles > 1){//counts ho long to wait for the next bit (works with 100us)
        waiting_cycles--;
        return;
    }

    uint8 value = REMOTE;//input
    static uint16 data_value=0;
        
    if (receiving_bit == RC_NOT_STARTED){//state machine
        if(value == 1){//this is only noise
            Sys_Start_AtomicSection();
                Sys_Start_RemoteControl();
                message_arriving = false;
                waiting_cycles = RC_WAIT_INITIALLY;
            Sys_End_AtomicSection();
            return;
        } else {
            waiting_cycles = RC_WAIT_FOR_HALFBIT; //because active low -> wait at the first half of the bit !!
            receiving_bit = 0;
            data_value = 0;
            return;
        }
    }

    if (receiving_bit < 12){
        Sys_Start_AtomicSection();
            data_value <<= 1;
            data_value += value;
            waiting_cycles  = RC_WAIT_FOR_BIT;
            receiving_bit++;
        Sys_End_AtomicSection();
        return;
    }
    
    Sys_Start_AtomicSection();
        Sys_Start_RemoteControl();

        message_arriving = false;
        receiving_bit = RC_NOT_STARTED;
        waiting_cycles = 20;

    Sys_End_AtomicSection();

    if(rx_buffer == data_value){
       return;
    }
    rx_buffer = data_value;
    isNewDataAvailable = true;
            
        //Sys_Memcpy( &data_value, &rx_buffer, 2 );

   uint8 val = rx_buffer & 0x003f;
   Sys_Send_Event(SYS_EVENT_IO_REMOECONTROL, &val, 1);//send data as event
}

/**
 *
 * This function returns true if a new remote control command has arrived
 *
 * @return bool true if a new remote control command has arrived
 */
bool Sys_HasRemoteC_Sent_New_Data() {
    return isNewDataAvailable;
}

/**
 *
 * This function returns the check bit value of an remote control to indicate if a button was pressed continuously or sequential.
 *
 * @return bit to indicate the check bit
 */
uint8 Sys_RemoteC_Get_CheckBit() {
    return (rx_buffer & 0x0800) >> 11;
}

/**
 *
 * This function returns the address of an remote control.
 *
 * @return address of the remote control
 */
uint8 Sys_RemoteC_Get_Address() {
    return (rx_buffer & 0x07c0) >> 6;
}

/**
 *
 * returns the value received by the remote control
 *
 * @return value received by the remote control
 */
uint8 Sys_RemoteC_Get_Data() {
    isNewDataAvailable = false;
    return rx_buffer & 0x003f;
}
