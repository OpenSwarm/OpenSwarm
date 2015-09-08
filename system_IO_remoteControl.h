/* 
 * File:   szstem_IO_remoteControl.h
 * Author: strenkwalder
 *
 * Created on 08 September 2015, 12:32
 */

#ifndef SZSTEM_IO_REMOTECONTROL_H
#define	SZSTEM_IO_REMOTECONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "definitions.h"
    
inline void Sys_Init_RemoteControl(void);
inline void Sys_Start_RemoteControl(void);

bool Sys_RemoteC_Received_New_Data(void);
uint8 Sys_RemoteC_Get_CheckBit(void);
uint8 Sys_RemoteC_Get_Address(void);
uint8 Sys_RemoteC_Get_Data(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SZSTEM_IO_REMOTECONTROL_H */

