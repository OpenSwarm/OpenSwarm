/*! \file
 *  \brief     	It declares general preprocessor variables and types.
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

#define EPUCK_USED /*!< defines that the currently used platform is the e-puck*/
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
    #include "platform/e-puck/DSPIC30F6014A_HDI.h"

    #define UART1_RX _RF2
    #define UART1_TX _RF3
    #define UART2_RX _RF4
    #define UART2_TX _RF5
    
    #define UART1_RX_DIR _TRISF2
    #define UART1_TX_DIR _TRISF3
    #define UART2_RX_DIR _TRISF4
    #define UART2_TX_DIR _TRISF5
#endif
        
#define SYS_EVENT_TERMINATION       0x01    /*!< ID of the event that signal a general termination event*/
#define SYS_EVENT_IO_MOTOR_LEFT     0x02    /*!< ID of the event that controls the left motor speed/direction*/
#define SYS_EVENT_IO_MOTOR_RIGHT    0x03    /*!< ID of the event that controls the right motor speed/direction*/
#define SYS_EVENT_IO_CAMERA         0x04    /*!< ID of the event that is emmited by the camera*/
#define SYS_EVENT_IO_REMOECONTROL   0x05    /*!< ID of the event that is sent after receiving a remote control signal*/
#define SYS_EVENT_IO_TO_BLUETOOTH   0x06    /*!< ID of the event that sends data via Bluetooth*/
#define SYS_EVENT_1ms_CLOCK         0x07    /*!< ID of the event that signals 1ms timer ticks*/

#define ALL_FUNCTIONS ((pEventHandlerFunction) 0xFFFFFFFE) /*!< the value to indicate all event handler*/

/**
 * @brief defines a system-wide colours
 *
 * This enum defines system-wide colours. (it is based on one bit for red, blue, and green). In total, 8 colours are defined with the first three bits.
 */
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
typedef signed short sint;/*!< e-puck specific valued for the default signed integer*/
typedef unsigned short uint;/*!< e-puck specific valued for the default unsigned integer*/
#endif

typedef void (*pFunction)(void);	/*!< Defines a pointer to a function with no return value and argument*/
typedef void (*pByteFunction)(uint8);	/*!< Defines a pointer to a function with no return value and one argument*/

typedef void (*pUART_reader) (uint8 data); /*!< Defines a pointer to a function with no return value and one argument*/

#ifdef	__cplusplus
}
#endif

#endif	/* DEFINITIONS_H */

