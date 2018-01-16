/* 
 * File:   communicationInit.h
 * Author: strenkwalder
 *
 * Created on 12 May 2017, 10:00
 */

#ifndef COMMUNICATIONINIT_H
#define	COMMUNICATIONINIT_H

#include "../definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif

    void Sys_Start_ChannelCalibration();
    uint Sys_ComThreshold(uint sensor);
    
    void Sys_SetThreshold(uint thres);
    uint Sys_GetThreshold();

    uint Sys_ComBackground(uint sensor);
    void Sys_SetComBackground(uint sensor, uint thres);
    void Sys_UpdateBackground(uint sensor, int diff);

#ifdef	__cplusplus
}
#endif

#endif	/* COMMUNICATIONINIT_H */

