/*!
 * \file
 * \ingroup prox
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * \version 1.0
 *
 * \date Jan 2016
 * 
 * \brief It declares functions to measure distances to objects. (Proximity-sensors)
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

/*! \defgroup prox Proximity Sensors
 * \ingroup adc
 * \ingroup io
 * \ingroup epuck
 * \brief IFunctions to measure distances to objects. (Proximity-sensors)
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * This module take sensor values from the ADC ( \ref adc ) and transforms them into values in mm. It has been noticed that values above 10 cm are not reliable and the sensor value was limited to it. Each sensor value is sent as event to handlers. The following events are available:
 * - SYS_EVENT_IO_PROX_7
 * - SYS_EVENT_IO_PROX_6
 * - SYS_EVENT_IO_PROX_5
 * - SYS_EVENT_IO_PROX_4
 * - SYS_EVENT_IO_PROX_3
 * - SYS_EVENT_IO_PROX_2
 * - SYS_EVENT_IO_PROX_1
 * - SYS_EVENT_IO_PROX_0
 * 
 * \section adc_usage Usage
 * After the initialisation with Sys_Init_Proximity() and start with Sys_Start_Proximity(), the proximity values are calculated and send via events (see \ref events ). 
 * In case the transformation needs to be stopped, Sys_Deactivate_Proximity() can disable the module and removes all related data structures.
 * The buffered value from the previous measurement can be obtained by Sys_Get_Prox(uint). Sys_Get_MaxProx(uint) was used to return a measured distance with maximal value (closest), this value is used to auto-calibrate the 0 distance. However this function was only implemented for debugging.
 * 
 * \section adc_license License
 * LICENSE: adapted FreeBSD License (see http://openswarm.org/license)\n
 * Copyright (c) 2016, Stefan M. Trenkwalder\n
 * All rights reserved. 
 */

#ifndef PROXIMITY_H
#define	PROXIMITY_H

#ifdef	__cplusplus
extern "C" {
#endif

void Sys_Init_Proximity(void);
void Sys_Start_Proximity(void);
void Sys_Stop_Proximity(void);
void Sys_Deactivate_Proximity(void);
    
uint Sys_Get_Prox(uint index);
uint Sys_Get_MaxProx(uint index);

uint Sys_Get_Raw(uint index);

#ifdef	__cplusplus
}
#endif

#endif	/* PROXIMITY_H */

