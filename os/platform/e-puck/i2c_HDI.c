/*!
 * \file
 * \ingroup i2c
 * \ingroup hdi
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 10 August 2015
 * 
 * \brief Hardware dependent implementations to read and write on the I2C interface.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#include "i2c_HDI.h"
#include "i2c_data.h"

#include "../../definitions.h"
#include "../../interrupts.h"

/**
 *
 * This function initialises the I2C interface.
 *
 */
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

/**
 *
 * This function starts the I2C interface.
 *
 */
inline void Sys_Start_I2C_HDI(void){
    IFS0bits.MI2CIF=0; // clear master interrupt flag
    IEC0bits.MI2CIE=1; // enable I2C master interrupt
    I2CCONbits.I2CEN=1; // enable I2C
}

/**
 *
 * This function pauses the I2C interface.
 *
 */
inline void Sys_Pause_I2C_HDI(void){
    IEC0bits.MI2CIE=0; // disable I2C master interrupt
}

/**
 *
 * This function continues the I2C interface.
 *
 */
inline void Sys_Contine_I2C_HDI(void){
    IEC0bits.MI2CIE=1; // enable I2C master interrupt
}

/**
 *
 * This function stops the I2C interface.
 *
 */
inline void Sys_Stop_I2C_HDI(void){
    IFS0bits.MI2CIF=0; // clear master interrupt flag
    IEC0bits.MI2CIE=0; // enable I2C master interrupt
    I2CCONbits.I2CEN=0; // disable I2C
}

/**
 *
 * This function sends a start bits.
 *
 */
inline void Sys_I2C_Send_Start_HDI(){
    if(I2CSTATbits.P){//was the stop-bit = P the last thing which was sent or received -> if yes i2c bus is in idlemode
        I2CCONbits.SEN=1; //send the start bit signal
    }
}

/**
 *
 * This function sends a restart bits.
 *
 */
inline void Sys_I2C_Send_Restart_HDI(void){
    if(I2CSTATbits.S){//was the start-bit = S the last thing which was sent or received
        I2CCONbits.RSEN=1; //send the restart bit signal
    }
}

/**
 *
 * This function sends a stop bits.
 *
 */
inline void Sys_I2C_Send_Stop_HDI(void){
    I2CCONbits.PEN=1;
}

/**
 *
 * This function sends a ack bits.
 *
 */
inline void Sys_I2C_Send_ACK_HDI(void){
    // make sure I2C bus is inactive
    if(I2CCONbits.SEN || I2CCONbits.PEN || I2CCONbits.RCEN || I2CCONbits.ACKEN || I2CCONbits.RSEN)
        return;

    // send ACK
    I2CCONbits.ACKDT=0;
    I2CCONbits.ACKEN=1;
}

/**
 *
 * This function sends a nack bits.
 *
 */
inline void Sys_I2C_Send_NACK_HDI(void){
    // make sure I2C bus is inactive
    if(I2CCONbits.SEN || I2CCONbits.PEN || I2CCONbits.RCEN || I2CCONbits.ACKEN || I2CCONbits.RSEN)
        return;

    // send ACK
    I2CCONbits.ACKDT=1;
    I2CCONbits.ACKEN=1;
}

/**
 *
 * This function sends a reading bits.
 *
 */
inline void Sys_I2C_Start_Reading_HDI(){

    if((I2CCONbits.SEN || I2CCONbits.PEN || I2CCONbits.RCEN || I2CCONbits.ACKEN || I2CSTATbits.TRSTAT)){
        return;
    }

    // start receive mode for I2C
    I2CCONbits.RCEN=1;
}

/**
 *
 * This function reads a byte.
 *
 */
inline char Sys_I2C_ReadByte_HDI(){
    return I2CRCV;
}

/**
 *
 * This function writes a byte.
 *
 * @param[in] byte the byte that has to be written
 */
inline void Sys_I2C_WriteByte_HDI(uint8 byte){
    I2CTRN=byte;
}
