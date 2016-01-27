/*!
 * \file
 * \ingroup epuck
 * \ingroup hdi
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * \version 1.0
 *
 * \date 07 July 2014
 * 
 * \brief  declares  e-puck specific types and preprocessor variables
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 */

/*! \defgroup epuck e-puck specific modules
 * \brief Modules and functions that are needed to use the e-puck platform ( http://www.gctronic.com/doc/index.php/E-Puck )
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 *  
 * \section Provided Features
 * The e-puck provides the following features:
 * \subsection epuck_sensor Sensors:
 *      \subsubsection epuck_prox 8 infra-red proximity sensors
 *      The infra-red proximity sensors are currently under implementation. Therefore not ready yet.
 *      \subsubsection epuck_acc accelerometer
 *      The accelerometer weren't needed for many applications and, therefore, the priority to implement the accelerometer is small.
 *      \subsubsection epuck_mic 3 microphones
 *      The microphones weren't needed for many applications and, therefore, the priority to implement the microphones is small.
 *      \subsubsection epuck_camera camera:
 *      The camera functions can be found at \ref camera
 *      \subsubsection epuck_remote remote control receiver:
 *      This function is fully implemented (\ref remotecontrol ).
 * \subsection epuck_output Actuators:
 *      \subsubsection epuck_motors differential drive (\ref motors ).
 *      \subsubsection epuck_led leds:
 *      Hardware independent functions to control the LEDs are not yet implemented, due to it's simple nature. Currently you can use the MACROs LED0, LED1, ..., LED7, BODYLED, FRONTLED to set and unset these LEDs.
 *      \subsubsection epuck_speaker speaker:
 *      The speakers weren't needed for many applications and, therefore, the priority to implement the speakers is small.
 * \subsection epuck_com communication:
 *      \subsubsection epuck_bluetooth Bluetooth:
 *      The Bluetooth can be used by sending and receiving bytes via UART1 (\ref uart )
 *      \subsubsection epuck_ircom Infra-red communication
 *      The infra-red proximity sensors can be used to transmit and receive data. This function leads to a local broadcasting. However, this function is still under development.
 *
 * \section epuck_license License
 * LICENSE: adapted FreeBSD License (see http://openswarm.org/license)\n
 * Copyright (c) 2015, Stefan M. Trenkwalder\n
 * All rights reserved. 
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

