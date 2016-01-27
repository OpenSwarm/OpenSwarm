/*!
 * \file
 * \ingroup remotecontrol
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \author  Yuri Kaszubowski Lopes <yurikazuba@gmail.com>
 * 
 * \version 1.0
 *
 * \date 27 August 2015
 * 
 * \brief  It declares functions to receive and decode messages from a remote control
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 */

/*! \defgroup remotecontrol Remote Control
 * \ingroup epuck
 * \ingroup io
 * 
 * \brief Functions to receive data from a remote control
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * This module is based on the RC-5 coding for the Toshiba RC-3910 (https://en.wikipedia.org/wiki/RC-5)
 *  
 * \section rc_usage Usage
 * After the initialisation with Sys_Init_RemoteControl(), the interface needs to be started to be able to receive or transmit bytes with Sys_Start_RemoteControl().
 * 
 * After this, any pressed button on the remote control is received as an event (SYS_EVENT_IO_REMOECONTROL).
 * 
 * \section rc_license License
 * LICENSE: adapted FreeBSD License (see http://openswarm.org/license)\n
 * Copyright (c) 2015, Stefan M. Trenkwalder\n
 * All rights reserved. 
 */
#ifndef SZSTEM_IO_REMOTECONTROL_H
#define SZSTEM_IO_REMOTECONTROL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "../../definitions.h"

//RC-5 coding -- Toshiba RC-3910
#define RC_BUTTON_STANDBY       12  /*!< Value for the standby button (RC-5 coding for a Toshiba RC-3910) */

#define RC_BUTTON_SCREEN        11  /*!< Value for the screen button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_LANG          15  /*!< Value for the language button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_SUBTTL        31  /*!< Value for the subtitle button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_INTERNET      46  /*!< Value for the internet button (RC-5 coding for a Toshiba RC-3910) */

#define RC_BUTTON_RED           55  /*!< Value for the red button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_GREEN         54  /*!< Value for the green button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_YELLOW        50  /*!< Value for the yellow button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_BLUE          52  /*!< Value for the blue button (RC-5 coding for a Toshiba RC-3910) */

#define RC_BUTTON_0             0  /*!< Value for the 0 button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_1             1  /*!< Value for the 1 button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_2             2  /*!< Value for the 2 button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_3             3  /*!< Value for the 3 button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_4             4  /*!< Value for the 4 button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_5             5  /*!< Value for the 5 button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_6             6  /*!< Value for the 6 button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_7             7  /*!< Value for the 7 button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_8             8  /*!< Value for the 8 button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_9             9  /*!< Value for the 9 button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_TELE_TEXT     60  /*!< Value for the tele text button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_SWAP          34  /*!< Value for the swap button (RC-5 coding for a Toshiba RC-3910) */

#define RC_BUTTON_OK            53  /*!< Value for the OK button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_CURSOR_UP     20  /*!< Value for the coursor up button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_CURSOR_DOWN   19  /*!< Value for the courser down button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_CURSOR_LEFT   21  /*!< Value for the courser left button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_CURSOR_RIGHT  22  /*!< Value for the courser right button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_BACK          10  /*!< Value for the back button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_MENU          48  /*!< Value for the menu button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_EPG           47  /*!< Value for the epg button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_FAV           40  /*!< Value for the favourite button (RC-5 coding for a Toshiba RC-3910) */

#define RC_BUTTON_SOURCE        56  /*!< Value for the source button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_INFO          18  /*!< Value for the info button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_PRESETS       14  /*!< Value for the preset button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_SLEEP         42  /*!< Value for the sleep button (RC-5 coding for a Toshiba RC-3910) */

#define RC_BUTTON_VOLUME_UP     16  /*!< Value for the volume up button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_VOLUME_DOWN   17  /*!< Value for the volume down button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_MUTE          13  /*!< Value for the mute button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_CHANNEL_UP    32  /*!< Value for the channel up button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_CHANNEL_DOWN  33  /*!< Value for the channel down button (RC-5 coding for a Toshiba RC-3910) */

////
#define RC_BUTTON_PAUSE         48  /*!< Value for the pause button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_REWIND        50  /*!< Value for the rewind button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_WIND          52  /*!< Value for the wind button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_PLAY          53  /*!< Value for the play button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_STOP          54  /*!< Value for the stop button (RC-5 coding for a Toshiba RC-3910) */
#define RC_BUTTON_RECORD        55  /*!< Value for the record button (RC-5 coding for a Toshiba RC-3910) */
    
inline void Sys_Init_RemoteControl(void);
inline void Sys_Start_RemoteControl(void);

bool Sys_RemoteC_Received_New_Data(void);
uint8 Sys_RemoteC_Get_CheckBit(void);
uint8 Sys_RemoteC_Get_Address(void);
uint8 Sys_RemoteC_Get_Data(void);

void Sys_Receive_RemoteControl_Msg(void);

#ifdef  __cplusplus
}
#endif

#endif  /* SZSTEM_IO_REMOTECONTROL_H */

