/*!
 * \file
 * \ingroup remotecontrol
 * \ingroup hdi
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \author  Yuri Kaszubowski Lopes <yurikazuba@gmail.com>
 * 
 * \version 1.0
 *
 * \date 27 August 2015
 * 
 * \brief  Hardware dependent implementations to receive and decode messages from a remote control
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 */

#ifndef SZSTEM_IO_REMOTECONTROL_HDI_H
#define SZSTEM_IO_REMOTECONTROL_HDI_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "../../definitions.h"
    

#define RC_WAIT_FOR_QUARTERBIT 4  /*!< Cycles that are needed to wait a quarter of a single bit duration */
#define RC_WAIT_FOR_HALFBIT    8  /*!< Cycles that are needed to wait a half of a single bit duration */
#define RC_WAIT_FOR_BIT        18 /*!< Cycles that are needed to wait a single bit duration  */
#define RC_WAIT_INITIALLY      RC_WAIT_FOR_BIT+RC_WAIT_FOR_QUARTERBIT /*!< Cycles that are needed to wait at the beginning of a message */
#define RC_NOT_STARTED -1         /*!< The initial state of the state machine to decode a remote control message */
    
extern bool message_arriving; 
extern sint8 waiting_cycles;

extern uint rx_buffer;

extern bool isNewDataAvailable;

extern sint8 receiving_bit;
    
inline void Sys_Init_RemoteControl_HDI(void);
inline void Sys_Start_RemoteControl_HDI(void);

#ifdef  __cplusplus
}
#endif

#endif  /* SZSTEM_IO_REMOTECONTROL_H */

