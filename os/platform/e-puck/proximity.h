/* 
 * File:   proximity.h
 * Author: strenkwalder
 *
 * Created on 04 March 2016, 17:05
 */

#ifndef PROXIMITY_H
#define	PROXIMITY_H

#ifdef	__cplusplus
extern "C" {
#endif

void Sys_Init_Proximity(void);
void Sys_Start_Proximity(void);
    
uint Sys_Get_Prox(uint index);
uint Sys_Get_MaxProx(uint index);

#ifdef	__cplusplus
}
#endif

#endif	/* PROXIMITY_H */

