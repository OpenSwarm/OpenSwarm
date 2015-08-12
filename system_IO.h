/* 
 * File:   system_IO.h
 * Author: strenkwalder
 *
 * Created on 28 July 2015, 11:21
 */

#ifndef SYSTEM_IO_H
#define	SYSTEM_IO_H

#ifdef	__cplusplus
extern "C" {
#endif

void Sys_Init_IOManagement(void);   //initialises the system timer and sets a scheduling algorithm
void Sys_Start_IOManagement(void);

void __attribute__((interrupt,no_auto_psv)) _T3Interrupt(void);
void __attribute__((interrupt,no_auto_psv)) _AltT3Interrupt(void);

inline void Sys_Stop_IOTimer(void);
inline void Sys_Continue_IOTimer(void);
inline void Sys_Reset_IOTimer(void);
inline void Sys_Disable_IOTimerInterrupt(void);
inline void Sys_Enable_IOTimerInterrupt(void);
inline void Sys_Force_IOTimerInterrupt(void);

bool Sys_Register_IOHandler(pFunction func);
void Sys_Unregister_IOHandler(pFunction func);

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_H */

