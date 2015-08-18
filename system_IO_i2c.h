/* 
 * File:   system_IO_i2c.h
 * Author: strenkwalder
 *
 * Created on 10 August 2015, 15:53
 */

#ifndef SYSTEM_IO_I2C_H
#define	SYSTEM_IO_I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "definitions.h"

inline void Sys_Init_I2C(void);
inline void Sys_Start_I2C(void);
inline void Sys_Pause_I2C(void);
inline void Sys_Contine_I2C(void);
inline void Sys_Stop_I2C(void);

void Sys_I2C_SentBytes(uint8 address, uint8 *bytes, uint16 length);
void Sys_I2C_Read(uint8 address, uint8 *intern_address, uint16 length, pByteFunction bytehandler);


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_I2C_H */

