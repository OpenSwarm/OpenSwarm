/********************************************************************************

			Programm which detects the shocks enduring by the e-puck      
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
 * \brief The shocks detector
 * \section sect1 Introduction
 * This demo uses the acceleromter to detect the direction of the shocks
 * enduring by the e-puck.
 * \n When the e-puck has deteced a shock, he plays a sound and show you
 * the direction of this shock by turning on the appropriated LED.
 * 
 * \section sect2 Playing the demo
 * First of all, move the selector to the position 0 and reset the e-puck.
 * \n Then you only have to hit the e-puck and a LED will turn on to
 * show you the direction of the shock that the e-puck has detected. The
 * e-puck will also play a sound.
 * \n If you let the e-puck falling, he detects the "zero_G" and beginn to
 * cry.
 *
 * \section sect3 Video of the demo
 * The video of this demo: http://www.youtube.com/watch?v=NCK9gRL9mb4
 *
 * \author Code: Michael Bonani, Jonathan Besuchet \n Doc: Jonathan Besuchet
 */

#include "p30f6014A.h"
#include "stdio.h"
#include "string.h"

#include "codec/e_sound.h"
#include "motor_led/e_init_port.h"
#include "motor_led/advance_one_timer/e_led.h"
#include "motor_led/advance_one_timer/e_motors.h"
#include "uart/e_uart_char.h"
#include "a_d/advance_ad_scan/e_ad_conv.h"
#include "a_d/advance_ad_scan/e_acc.h"

#include "math.h"
#include "utility.h"

#define STATE_NORMAL (0)
#define STATE_FREEFALL (1)
#define STATE_SHOCK (2)

#define PI 3.14159265358979

//#define MOVE //option if you want that e-puck move

extern int e_dci_unavailable;
extern int e_stop_flag;


/*! \brief The "main" function of the demo */
void run_accelerometer() {
	int accx, accy, accz;
	long ampl;
	long amplavg;
//	char buffer[80];
	int state;
	int lednum;
	double angle;
	int soundsel;

	// Init sound
	e_init_port();
    e_init_ad_scan(ALL_ADC);
	e_init_sound();
#ifdef MOVE
	e_init_motors();
	e_start_agendas_processing();
#endif

	// Calibrate accelerometers
	e_set_led(8, 1);
	e_acc_calibr();
	e_set_led(8, 0);

#ifdef MOVE
	// Move forwards
	e_set_speed_left(100);
	e_set_speed_right(100);
#endif
	// Detect free fall and shocks
	state=STATE_NORMAL;
	amplavg=1000;
	while (1) {

		accx = e_get_acc(0);
		accy = e_get_acc(1);
		accz = e_get_acc(2) + 744; //744 is 1g

		if ((accz<0) && (accz>-600)) {accz=0;}
		ampl=((long)(accx)*(long)(accx))+((long)(accy)*(long)(accy))+((long)(accz)*(long)(accz));
		amplavg=(amplavg>>2)+ampl;

		if (! e_dci_unavailable) {
			if (state!=STATE_NORMAL) {
				state=STATE_NORMAL;
				e_set_led(8, 0);
				e_set_body_led(0);
			}
		}

		if (amplavg<1000) {
			if (state!=STATE_FREEFALL) {
				state=STATE_FREEFALL;
				e_stop_flag=1;
				while (e_dci_unavailable);
//				sprintf(buffer, "Free fall: %ld, (%d, %d, %d) -> (%ld)\r\n", amplavg, accx, accy, accz, ampl);
//				e_send_uart1_char(buffer, strlen(buffer));
				e_play_sound(11028, 8016);
				e_set_body_led(1);
				e_set_led(8, 0);
			}
		} else if (amplavg>4000000) {
			if (state!=STATE_SHOCK) {
				state=STATE_SHOCK;
				e_stop_flag=1;
				while (e_dci_unavailable);
//				sprintf(buffer, "Shock: %ld, (%d, %d, %d) -> (%ld)\r\n", amplavg, accx, accy, accz, ampl);
//				e_send_uart1_char(buffer, strlen(buffer));
				soundsel=(accx & 3);
				if (soundsel==0) {
					e_play_sound(0, 2112);
				} else if (soundsel==1) {
					e_play_sound(2116, 1760);
				} else {
					e_play_sound(3878, 3412);
				}
				e_set_body_led(0);

				angle=atan2(accy, accx);
				lednum=floor(atan2(accy, accx)/PI*4+PI/2+PI/8);
				while (lednum>8) {lednum=lednum-8;}
				while (lednum<0) {lednum=lednum+8;}
//				sprintf(buffer, "(x=%d, y=%d) -> angle=%f, led=%d\r\n", accx, accy, angle, lednum);
//				e_send_uart1_char(buffer, strlen(buffer));
				e_set_led(lednum, 1);
			}
		}
	}
}

