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

    //RC-5 coding
#define RC_BUTTON_0             0
#define RC_BUTTON_1             1
#define RC_BUTTON_2             2
#define RC_BUTTON_3             3
#define RC_BUTTON_4             4
#define RC_BUTTON_5             5
#define RC_BUTTON_6             6
#define RC_BUTTON_7             7
#define RC_BUTTON_8             8
#define RC_BUTTON_9             9
#define RC_BUTTON_PAUSE         48
#define RC_BUTTON_REWIND        50
#define RC_BUTTON_WIND          52
#define RC_BUTTON_PLAY          53
#define RC_BUTTON_STOP          54
#define RC_BUTTON_RECORD        55
#define RC_BUTTON_VOLUME_UP     16
#define RC_BUTTON_VOLUME_DOWN   17
#define RC_BUTTON_CHANNEL_UP    32
#define RC_BUTTON_CHANNEL_DOWN  33
#define RC_BUTTON_STANDBY       12
#define RC_BUTTON_OK            87
#define RC_BUTTON_CURSOR_UP     80
#define RC_BUTTON_CURSOR_DOWN   81
#define RC_BUTTON_CURSOR_LEFT   85
#define RC_BUTTON_CURSOR_RIGHT  86
    
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

