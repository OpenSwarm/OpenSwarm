/********************************************************************************

			Programm to demonstrate all the e-puck's capabilities
			Version 2.0 août 2007				                          
			Michael Bonani, Jonathan Besuchet 


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2004-2007 Michael Bonani, Jonathan Besuchet

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \brief The main file of the demo.
 *
 * This file regroups all the programmes demo. You can choose the one you
 * want to try by moving the selector.
 * \n \n Here is the the demo corresponding to the selector position:
 * - Selector position 0: Show the ground direction. Look at rungrounddirection.h for more information.
 * - Selector position 1: Shock detection. Look at runaccelerometer.h for more information.
 * - Selector position 2: Locate the sound source. Look at runlocatesound.h for more information.
 * - Selector position 3: Follow the wall. Look at runwallfollow.h for more information.
 * - Selector position 4: Follow what is detected by the two front proximities detectors. Look at runbreitenberg_adv.h for more information.
 * - Selector position 5: Avoid the obstacles. Look at runbreitenberg_adv.h for more information.
 * - Selector position 6: Follow all balls. Look at runfollowball.h for more information.
 * - Selector position 7: Follow the red ball. Look at runfollowball.h for more information.
 * - Selector position 8: Follow the green ball. Look at runfollowball.h for more information.
 * - Other selector position: Command the e-puck with the sound. Look at runfftlistener.h for more information.
 *
 * \warning When you have made your selection with the selector, YOU MUST reset
 * the e-puck (the blue button near the selector) to make your choice effective.
 * \section sect_selector_pos The selector position:
 * \image html selector.gif
 * \author Code: Michael Bonani, Jonathan Besuchet \n Doc: Jonathan Besuchet
 */

/*! \mainpage Demo documentation
 * \image html logo.gif
 * \section intro_sec Introduction
 * The programm "Demo" is made to demonstrate some capabilities of
 * the e-puck and to illustrate how you can use the library.
 * \n \n This demo is, in fact, a compilation of nine demos. You can choose
 * the demo you want to play by moving the selector on the back of the e-puck.
 * \n \n Here is the the demo corresponding to the selector position:
 * - Selector position 0: Show the ground direction. Look at rungrounddirection.h for more information.
 * - Selector position 1: Shock detection. Look at runaccelerometer.h for more information.
 * - Selector position 2: Locate the sound source. Look at runlocatesound.h for more information.
 * - Selector position 3: Follow the wall. Look at runwallfollow.h for more information.
 * - Selector position 4: Follow what is detected by the two front proximities detectors. Look at runbreitenberg_adv.h for more information.
 * - Selector position 5: Avoid the obstacles. Look at runbreitenberg_adv.h for more information.
 * - Selector position 6: Follow all balls. Look at runfollowball.h for more information.
 * - Selector position 7: Follow the red ball. Look at runfollowball.h for more information.
 * - Selector position 8: Follow the green ball. Look at runfollowball.h for more information.
 * - Other selector position: Command the e-puck with the sound. Look at runfftlistener.h for more information.
 * \warning When you have made your selection with the selector, YOU MUST reset
 * the e-puck (the blue button near the selector) to make your choice effective.
 * \section sect_selector_pos2 The selector position:
 * \image html selector.gif
 *
 * \section link_sec External links
 * - http://www.e-puck.org/                 The official site of the e-puck
 * - https://gna.org/projects/e-puck/       The developpers area at gna
 * - http://lsro.epfl.ch/                   The site of the lab where the e-puck has been created
 * - http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45 The license
 *
 * \author Doc: Jonathan Besuchet
 */

#include "p30f6014A.h"
#include "stdio.h"
#include "string.h"

#include "uart/e_uart_char.h"
#include "motor_led/e_init_port.h"

#include "ComModule.h"
#include "runcollaboration.h"
#include "runaccelerometer.h"
#include "runbreitenberg_adv.h"
#include "runlocatesound.h"
#include "runwallfollow.h"
#include "runfollowball.h"
#include "runfftlistener.h"
#include "rungrounddirection.h"
#include "utility.h"

#define PI 3.14159265358979

/*! \brief The main function of the application
 *
 * The main function launch one of the following self-governing function
 * depending of the selector position: run_accelerometer(); run_locatesound();
 * run_wallfollow(); run_grounddirection(); run_breitenberg_follower();
 * run_breitenberg_shocker(); run_follow_ball_green(); run_follow_ball_red();
 * run_follow_ball(); run_fft_listener();
 */
int main() {
	char buffer[80];
	int selector;

	//system initialization 
	e_init_port();
	e_init_uart1();

	//Reset if Power on (some problem for few robots)
	if (RCONbits.POR) {
		RCONbits.POR=0;
		__asm__ volatile ("reset");
	}

	// Decide upon program
	selector=getselector();
	sprintf(buffer, "Starting with selector pos %d\r\n", selector);
	e_send_uart1_char(buffer, strlen(buffer));
	
	if (selector==0) {
		run_grounddirection();
	} else if (selector==1) {
		run_accelerometer();
	} else if (selector==2) {
		run_locatesound();
	} else if (selector==3) {
		run_wallfollow();
	} else if (selector==4) {
		run_breitenberg_follower();
	} else if (selector==5) {
		run_breitenberg_shocker();
	} else if (selector==6) {
		run_follow_ball();
	} else if (selector==7) {
		run_follow_ball_red();
	} else if (selector==8) {
		run_follow_ball_green();
	} else
		run_fft_listener();
	
	while(1);
	return 0;
}

