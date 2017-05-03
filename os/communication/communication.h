/* 
 * File:   communication.h
 * Author: strenkwalder
 *
 * Created on 02 May 2017, 22:48
 */

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#ifdef	__cplusplus
extern "C" {
#endif

void Sys_Init_Communication();
void Sys_Start_Communication();
void Sys_Stop_Communication();
    
void Sys_Send_Message(void *data, size_t length);




#ifdef	__cplusplus
}
#endif

#endif	/* COMMUNICATION_H */

