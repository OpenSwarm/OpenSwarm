/*!
 * \file
 * \ingroup i2c
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 10 August 2015
 * 
 * \brief  It declares functions to read and write on the I2C interface.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */
/*! \defgroup i2c I2C interface
 * \ingroup io
 * \ingroup epuck
 * \brief Functions to read from and write on the I2C interface.
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * Inter-Integrated Circuit bus is a multi-master, multi-slave, serial bus (see also https://en.wikipedia.org/wiki/I%C2%B2C )
 *   
 * \section i2c_usage Usage
 * The I2C interface can be initialised and started with Sys_Init_I2C() and Sys_Start_I2C() respectively. 
 * Similarly, it can be paused, continued, or stopped by Sys_Pause_I2C(), Sys_Contine_I2C(), or Sys_Stop_I2C() respectively.
 * While the interface is running, data can be written with Sys_I2C_SentBytes(uint8, uint8 *, uint16). Values can be read with Sys_I2C_Read(uint8 , uint8 *, uint16, pByteFunction) where the request message has also to be specified.
 * 
 * \todo testing and debugging of this module.
 * 
 * \note This module is currently untested. It might not work or includes some bugs. The interrupt handler _MI2CInterrupt() is also out commented, because it might interfere with the e-Puck library used in the camera module.
 * 
 * \section i2c_license License
 * LICENSE: adapted FreeBSD License (see http://openswarm.org/license)\n
 * Copyright (c) 2015, Stefan M. Trenkwalder\n
 * All rights reserved. 
 */

#ifndef SYSTEM_IO_I2C_H
#define	SYSTEM_IO_I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../../definitions.h"

inline void Sys_Init_I2C(void);
inline void Sys_Start_I2C(void);
inline void Sys_Pause_I2C(void);
inline void Sys_Contine_I2C(void);
inline void Sys_Stop_I2C(void);

void Sys_I2C_SentBytes(uint8 address, uint8 *bytes, uint length);
void Sys_I2C_Read(uint8 address, uint8 *intern_address, uint length, pByteFunction bytehandler);


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_I2C_H */

