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

#ifndef SYSTEM_IO_I2C_HDI_H
#define	SYSTEM_IO_I2C_HDI_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../../definitions.h"

inline void Sys_I2C_Send_Start_HDI();
inline void Sys_I2C_Send_Restart_HDI(void);
inline void Sys_I2C_Send_Stop_HDI(void);
inline void Sys_I2C_Send_ACK_HDI(void);
inline void Sys_I2C_Send_NACK_HDI(void);
inline void Sys_I2C_Start_Reading_HDI(void);
inline char Sys_I2C_ReadByte_HDI(void);
inline void Sys_I2C_WriteByte_HDI(uint8 byte);

inline void Sys_Init_I2C_HDI(void);
inline void Sys_Start_I2C_HDI(void);
inline void Sys_Pause_I2C_HDI(void);
inline void Sys_Contine_I2C_HDI(void);
inline void Sys_Stop_I2C_HDI(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_I2C_H */

