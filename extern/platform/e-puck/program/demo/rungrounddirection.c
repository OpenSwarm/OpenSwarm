/********************************************************************************

			Programm to show the gravity direction					      
			Version 3.0 7 janvier 2005			                          
			Lucas Meier


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2004-2007 Lucas Meier

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \brief Showing the ground direction
 * \section sect_dir1 Introduction
 * This programm works with the accelerometer to show you the ground
 * direction by turning on the appropriate LED.
 * 
 * \section sect_dir2 Playing the demo
 * First of all, move the selector to the position 3 and reset the e-puck.
 * Now take the e-puck in the hand and turn it with the Z axis horizontal.
 *
 * \section sect_dir3 Video of the demo
 * The video of this demo: http://www.youtube.com/watch?v=a_4eymEv4bs
 *
 * \author Code: Lucas Meier \n Doc:  Jonathan Besuchet
 */

#include "math.h"
#include "stdlib.h"

#include "a_d/advance_ad_scan/e_ad_conv.h"
#include "a_d/advance_ad_scan/e_acc.h"
#include "motor_led/e_epuck_ports.h"
#include "motor_led/e_init_port.h"

#include "rungrounddirection.h"

/*! \brief The "main" function of the demo */
void run_grounddirection(void)
{
	e_init_port();
    e_init_ad_scan(ALL_ADC);
	e_acc_calibr();
	
	while(1)
	{
		e_display_angle();
	}
}
