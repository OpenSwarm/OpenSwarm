/* 
 * File:   physical_HDI.h
 * Author: strenkwalder
 *
 * Created on 23 May 2017, 23:25
 */

#ifndef PHYSICAL_HDI_H
#define	PHYSICAL_HDI_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../../definitions.h"
    
#define ADCs_PER_BIT 1
    
inline void Sys_Init_PhysicalSendingChannel(void);
inline void Sys_Start_PhysicalSendingChannel(void);
inline void Sys_Stop_PhysicalSendingChannel(void);
inline void Sys_Pause_PhysicalSendingChannel(void);
inline void Sys_Continue_PhysicalSendingChannel(void);
inline void Sys_Disable_PhysicalSendingChannelInterrupt(void);
inline void Sys_Enable_PhysicalSendingChannelInterrupt(void);
inline void Sys_Force_PhysicalSendingChannelInterrupt(void);

void Sys_Register_SendingFunction(pFunction func);


#ifdef	__cplusplus
}
#endif

#endif	/* PHYSICAL_HDI_H */

