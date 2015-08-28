/* 
 * File:   definitions.h
 * Author: Stefan Trenkwalder
 *
 * Created on 08 July 2014, 14:55
 *
 * LICENCE: adapted FreeBSD Licence
 * Copyright (c) 2015, Stefan Trenkwalder
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. If this or parts of this source code or binary code in any form is used for a commercial product or service in any form, this product or service must provide a clear notice/message readable for any user or customer that this product or service uses OpenSwarm.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdbool.h>
#include <stdint.h>

#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SYS_EVENT_TERMINATION       0x01
#define SYS_EVENT_IO_MOTOR_LEFT     0x02
#define SYS_EVENT_IO_MOTOR_RIGHT    0x03
#define SYS_EVENT_IO_CAMERA         0x04

#define ALL_FUNCTIONS ((pEventHandlerFunction) 0xFFFFFFFE)

enum sys_colour {   BLACK   = 0b00000000,
                    RED     = 0b00000100,
                    YELLOW  = 0b00000110,
                    GREEN   = 0b00000010,
                    CYAN    = 0b00000011,
                    BLUE    = 0b00000001,
                    MAGENTA = 0b00000101,
                    WHITE   = 0b00000111};

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef signed char  sint8;
typedef signed short sint16;
typedef signed int   sint32;


typedef void (*pFunction)(void);
typedef void (*pByteFunction)(uint8);


#ifdef	__cplusplus
}
#endif

#endif	/* DEFINITIONS_H */

