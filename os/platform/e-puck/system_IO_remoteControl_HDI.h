/* 
 * File:   szstem_IO_remoteControl.h
 * Author: strenkwalder
 *
 * Created on 08 September 2015, 12:32
 */

#ifndef SZSTEM_IO_REMOTECONTROL_HDI_H
#define SZSTEM_IO_REMOTECONTROL_HDI_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "../../../os/definitions.h"
    

#define RC_WAIT_FOR_QUARTERBIT 4
#define RC_WAIT_FOR_HALFBIT    8
#define RC_WAIT_FOR_BIT        18
#define RC_WAIT_INITIALLY      RC_WAIT_FOR_BIT+RC_WAIT_FOR_QUARTERBIT
#define RC_NOT_STARTED -1
    
extern bool message_arriving;
extern sint8 waiting_cycles;

extern uint16 rx_buffer;

extern bool isNewDataAvailable;

extern sint8 receiving_bit;
    
inline void Sys_Init_RemoteControl_HDI(void);
inline void Sys_Start_RemoteControl_HDI(void);

#ifdef  __cplusplus
}
#endif

#endif  /* SZSTEM_IO_REMOTECONTROL_H */

