/*
 * File:   i2c_data.h
 * Author: Stefan M. Trenkwalder
 *
 * Created on 10 August 2015, 15:53
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
 */

#ifndef SYSTEM_IO_I2C_DATA_H
#define	SYSTEM_IO_I2C_DATA_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../definitions.h"
    
typedef enum { I2C_IDLE = 0, I2C_IS_STARTING, I2C_STARTED, I2C_IS_READING, I2C_IS_SENDING, I2C_SENT, I2C_ACKNOWLEDGED, I2C_IS_STOPPING, I2C_ERROR} sys_I2C_state;
typedef enum { I2C_IDLE_MODE = 0, I2C_WRITING_ADDRESS_MODE, I2C_READING_BYTES_MODE, I2C_WRITING_BYTES_MODE, I2C_ERROR_MODE} sys_I2C_mode;

//static sys_I2C_state sys_i2c_state = I2C_IDLE;
//static sys_I2C_mode sys_i2c_mode = I2C_IDLE_MODE;

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

