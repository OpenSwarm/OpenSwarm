/* 
 * File:   definitions.h
 * Author: Stefan
 *
 * Created on 08 July 2014, 14:55
 */

#include <stdbool.h>
#include <stdint.h>

#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SYS_EVENT_TERMINATION 0x01
#define SYS_EVENT_IO_MOTOR_LEFT 0x02
#define SYS_EVENT_IO_MOTOR_RIGHT 0x03

#define ALL_FUNCTIONS ((pEventHandlerFunction) 0xFFFFFFFE)

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef signed char  sint8;
typedef signed short sint16;
typedef signed int   sint32;


typedef void (*pFunction)(void);


#ifdef	__cplusplus
}
#endif

#endif	/* DEFINITIONS_H */

