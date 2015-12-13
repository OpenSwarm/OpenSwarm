/*
 * File:   definitions.h
 * Author: Stefan M. Trenkwalder
 *
 * Created on 08 July 2014, 14:55
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

#include <stdbool.h>
#include <stdint.h>

#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define EPUCK_USED
//#define KILOBOT_USED
    
    
#ifdef EPUCK_USED 
    #if defined(__XC16__)
        #include <xc.h>
    #elif defined(__C30__)
        #if defined(__dsPIC30F__)
            #include <p30Fxxxx.h>
        #endif
    #endif

    #include <p30F6014A.h>   

    #include "platform/e-puck/library/motor_led/e_epuck_ports.h"

    #define UART1_RX _RF2
    #define UART1_TX _RF3
    #define UART2_RX _RF4
    #define UART2_TX _RF5
    
    #define UART1_RX_DIR _TRISF2
    #define UART1_TX_DIR _TRISF3
    #define UART2_RX_DIR _TRISF4
    #define UART2_TX_DIR _TRISF5
#endif
    
//RC-5 coding -- Toshiba RC-3910
#define RC_BUTTON_STANDBY       12

#define RC_BUTTON_SCREEN        11
#define RC_BUTTON_LANG          15
#define RC_BUTTON_SUBTTL        31
#define RC_BUTTON_INTERNET      46

#define RC_BUTTON_RED           55
#define RC_BUTTON_GREEN         54
#define RC_BUTTON_YELLOW        50
#define RC_BUTTON_BLUE          52

#define RC_BUTTON_0             0
#define RC_BUTTON_1             1
#define RC_BUTTON_2             2
#define RC_BUTTON_3             3
#define RC_BUTTON_4             4
#define RC_BUTTON_5             5
#define RC_BUTTON_6             6
#define RC_BUTTON_7             7
#define RC_BUTTON_8             8
#define RC_BUTTON_9             9
#define RC_BUTTON_TELE_TEXT     60
#define RC_BUTTON_SWAP          34

#define RC_BUTTON_OK            53
#define RC_BUTTON_CURSOR_UP     20
#define RC_BUTTON_CURSOR_DOWN   19
#define RC_BUTTON_CURSOR_LEFT   21
#define RC_BUTTON_CURSOR_RIGHT  22
#define RC_BUTTON_BACK          10
#define RC_BUTTON_MENU          48
#define RC_BUTTON_EPG           47
#define RC_BUTTON_FAV           40

#define RC_BUTTON_SOURCE        56
#define RC_BUTTON_INFO          18
#define RC_BUTTON_PRESETS       14
#define RC_BUTTON_SLEEP         42

#define RC_BUTTON_VOLUME_UP     16
#define RC_BUTTON_VOLUME_DOWN   17
#define RC_BUTTON_MUTE          13
#define RC_BUTTON_CHANNEL_UP    32
#define RC_BUTTON_CHANNEL_DOWN  33

////
#define RC_BUTTON_PAUSE         48
#define RC_BUTTON_REWIND        50
#define RC_BUTTON_WIND          52
#define RC_BUTTON_PLAY          53
#define RC_BUTTON_STOP          54
#define RC_BUTTON_RECORD        55
    
#define SYS_EVENT_TERMINATION       0x01
#define SYS_EVENT_IO_MOTOR_LEFT     0x02
#define SYS_EVENT_IO_MOTOR_RIGHT    0x03
#define SYS_EVENT_IO_CAMERA         0x04
#define SYS_EVENT_IO_REMOECONTROL   0x05
#define SYS_EVENT_IO_TO_BLUETOOTH   0x06
#define SYS_EVENT_1ms_CLOCK         0x07

#define ALL_FUNCTIONS ((pEventHandlerFunction) 0xFFFFFFFE)

typedef enum sys_colour {   BLACK   = 0b00000000,
                    RED     = 0b00000100,
                    YELLOW  = 0b00000110,
                    GREEN   = 0b00000010,
                    CYAN    = 0b00000011,
                    BLUE    = 0b00000001,
                    MAGENTA = 0b00000101,
                    WHITE   = 0b00000111
} sys_colour;

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef signed char  sint8;
typedef signed short sint16;
typedef signed int   sint32;

#ifdef EPUCK_USED
typedef signed short sint;
typedef unsigned short uint;
#endif

typedef void (*pFunction)(void);
typedef void (*pByteFunction)(uint8);

typedef void (*pUART_reader) (uint8 data);

#ifdef	__cplusplus
}
#endif

#endif	/* DEFINITIONS_H */

