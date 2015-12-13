/**
 * @file system_IO_i2c_HDI.c
 * @author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Created on 10 August 2015
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
 * This file provides the hardware depended code to enable communication via I2C.
 */
#include "system_IO_i2c_HDI.h"
#include "../../../os/io/i2c_data.h"

#include "../../../os/definitions.h"
#include "../../../os/interrupts.h"

inline void Sys_Init_I2C_HDI(){

    I2CCONbits.I2CEN=0; // disable I2C

    //clear master and slave interrupt flags
    IFS0bits.MI2CIF=0;
    IFS0bits.SI2CIF=0;
    
    IEC0bits.MI2CIE=0; // disable I2C master interrupt
    IEC0bits.SI2CIE=0; // diseble I2C slave interrupt
    
    I2CBRG=150;		// frequency of SCL at 100kHz

    //TODO: have to check the priority levels of the others
    IPC3bits.MI2CIP=SYS_IRQP_I2C; // priority level

    I2CCONbits.I2CEN=1; // enable I2C
}


inline void Sys_Start_I2C_HDI(void){
    IFS0bits.MI2CIF=0; // clear master interrupt flag
    IEC0bits.MI2CIE=1; // enable I2C master interrupt
    I2CCONbits.I2CEN=1; // enable I2C
}

inline void Sys_Pause_I2C_HDI(void){
    IEC0bits.MI2CIE=0; // disable I2C master interrupt
}

inline void Sys_Contine_I2C_HDI(void){
    IEC0bits.MI2CIE=1; // enable I2C master interrupt
}

inline void Sys_Stop_I2C_HDI(void){
    IFS0bits.MI2CIF=0; // clear master interrupt flag
    IEC0bits.MI2CIE=0; // enable I2C master interrupt
    I2CCONbits.I2CEN=0; // disable I2C
}

inline void Sys_I2C_Send_Start_HDI(){
    if(I2CSTATbits.P){//was the stop-bit = P the last thing which was sent or received -> if yes i2c bus is in idlemode
        I2CCONbits.SEN=1; //send the start bit signal
    }
}

inline void Sys_I2C_Send_Restart_HDI(void){
    if(I2CSTATbits.S){//was the start-bit = S the last thing which was sent or received
        I2CCONbits.RSEN=1; //send the restart bit signal
    }
}

inline void Sys_I2C_Send_Stop_HDI(void){
    I2CCONbits.PEN=1;
}

inline void Sys_I2C_Send_ACK_HDI(void){
    // make sure I2C bus is inactive
    if(I2CCONbits.SEN || I2CCONbits.PEN || I2CCONbits.RCEN || I2CCONbits.ACKEN || I2CCONbits.RSEN)
        return;

    // send ACK
    I2CCONbits.ACKDT=0;
    I2CCONbits.ACKEN=1;
}

inline void Sys_I2C_Send_NACK_HDI(void){
    // make sure I2C bus is inactive
    if(I2CCONbits.SEN || I2CCONbits.PEN || I2CCONbits.RCEN || I2CCONbits.ACKEN || I2CCONbits.RSEN)
        return;

    // send ACK
    I2CCONbits.ACKDT=1;
    I2CCONbits.ACKEN=1;
}


inline void Sys_I2C_Start_Reading_HDI(){

    if((I2CCONbits.SEN || I2CCONbits.PEN || I2CCONbits.RCEN || I2CCONbits.ACKEN || I2CSTATbits.TRSTAT)){
        return;
    }

    // start receive mode for I2C
    I2CCONbits.RCEN=1;
}

inline char Sys_I2C_ReadByte_HDI(){
    return I2CRCV;
}

inline void Sys_I2C_WriteByte_HDI(uint8 byte){
    I2CTRN=byte;
}
