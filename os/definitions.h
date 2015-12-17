/*! \file	definition.h
 *  \brief     	This file defines all preprocessor variables and project wide types.
 *  \author    	Stefan M. Trenkwalder
 *  \version   	1.0
 *  \date      	2015
 *  \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
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

typedef unsigned char  uint8; 	/*!< Defines an unsigned  8bit integer*/
typedef unsigned short uint16; 	/*!< Defines an unsigned 16bit integer*/
typedef unsigned int   uint32; 	/*!< Defines an unsigned 32bit integer*/
typedef signed char  sint8;	/*!< Defines a    signed  8bit integer*/
typedef signed short sint16;	/*!< Defines a    signed 16bit integer*/
typedef signed int   sint32;	/*!< Defines a    signed 32bit integer*/

#ifdef EPUCK_USED
typedef signed short sint;
typedef unsigned short uint;
#endif

typedef void (*pFunction)(void);	/*!< Defines a pointer to a function with no return value and argument*/
typedef void (*pByteFunction)(uint8);	/*!< Defines a pointer to a function with no return value and one argument*/

typedef void (*pUART_reader) (uint8 data); /*!< Defines a pointer to a function with no return value and one argument*/

#ifdef	__cplusplus
}
#endif

#endif	/* DEFINITIONS_H */

