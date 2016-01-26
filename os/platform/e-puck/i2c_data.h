/*!
 * \file
 * \ingroup i2c
 * \ingroup io
 * \ingroup epuck
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 10 August 2015
 * 
 * \brief  declares functions to manage the I2C queue.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#ifndef SYSTEM_IO_I2C_DATA_H
#define	SYSTEM_IO_I2C_DATA_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../../definitions.h"
    
typedef enum { I2C_IDLE = 0, I2C_IS_STARTING, I2C_STARTED, I2C_IS_READING, I2C_IS_SENDING, I2C_SENT, I2C_ACKNOWLEDGED, I2C_IS_STOPPING, I2C_ERROR} sys_I2C_state; 	/*!< States for the I2C state machine*/
typedef enum { I2C_IDLE_MODE = 0, I2C_WRITING_ADDRESS_MODE, I2C_READING_BYTES_MODE, I2C_WRITING_BYTES_MODE, I2C_ERROR_MODE} sys_I2C_mode; 	/*!< Modes of the I2C interface */


/**
 * @brief It is a single linked list element containing messages that need to be sent via I2C.
 *
 */
typedef struct sys_i2c_message_s {
    uint8           i2c_device_address; // 7-bit addr
    uint8 *         data;
    uint16          length;
    bool            write;//= not read
    pByteFunction   handler;
    struct sys_i2c_message_s *next;
} sys_i2c_message, sys_i2c_messages, sys_i2c_msg;

extern sys_i2c_messages *sys_i2c_msgs;

void Sys_I2C_AppendMessages(sys_i2c_msg *item);
void Sys_I2C_RemoveOldestMessage(sys_i2c_messages **list);
void Sys_I2C_FreeMessages(sys_i2c_messages *list);


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_I2C_H */

