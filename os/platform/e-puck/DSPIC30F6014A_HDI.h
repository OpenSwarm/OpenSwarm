/* 
 * File:   HDI_DSPIC30F6014A.h
 * Author: Stefan
 *
 * Created on 07 July 2014, 14:47
 */

#ifndef HDI_DSPIC30F6014A_H
#define	HDI_DSPIC30F6014A_H

#include "library/motor_led/e_epuck_ports.h"

#ifdef	__cplusplus
extern "C" {
#endif

// Interrupt Vector Table
#define ADDRESS_IVT                 0x000004
#define ADDRESS_ITV_OSC_FAIL        ADDRESS_IVT+2
#define ADDRESS_ITV_ADDRESS_ERROR   ADDRESS_IVT+4
#define ADDRESS_ITV_STACK_ERROR     ADDRESS_IVT+6
#define ADDRESS_ITV_MATH_ERROR      ADDRESS_IVT+8
#define ADDRESS_IVT_T1              0x00001A


#define ADDRESS_AIVT                 0x000084
#define ADDRESS_AITV_OSC_FAIL        ADDRESS_AIVT+2
#define ADDRESS_AITV_ADDRESS_ERROR   ADDRESS_AIVT+4
#define ADDRESS_AITV_STACK_ERROR     ADDRESS_AIVT+6
#define ADDRESS_AITV_MATH_ERROR      ADDRESS_AIVT+8
#define ADDRESS_AIVT_T1              0x00009A



#ifdef	__cplusplus
}
#endif

#endif	/* HDI_DSPIC30F6014A_H */

