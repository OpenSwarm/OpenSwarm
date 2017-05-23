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


#ifdef	__cplusplus
}
#endif

#endif	/* COMMUNICATIONINIT_H */

