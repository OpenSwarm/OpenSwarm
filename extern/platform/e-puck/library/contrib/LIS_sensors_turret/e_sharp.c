/****************************************************************
* 			Sharp sensor of e-puck								*
*		Alessandro Ambuehl	& Walter Karlen	(WK)				*
				
* Version 1.2 23.8.2007 	 WK									*
* 		removed sensext 										* 
* Version 1.1 10.2006		 AA									*
*		initial release 
****************************************************************/

/*attention: betwen the e-puck and the Sharp, an inverter buffer
inverts the control lines (LED 1-5 and Vin)*/

#include "a_d/e_ad_conv.h"
#include "motor_led/e_epuck_ports.h"
#include "e_sharp.h"
#include <a_d/advance_ad_scan/e_acc.h>

//#include <motor_led/advance_one_timer/e_agenda.h>
//#include <a_d/advance_ad_scan/e_ad_conv.h>


// initialisation du capteur et des modules liés
void e_init_sharp(void)
{

	SHARP_LED1_DIR = OUTPUT_PIN;
	SHARP_LED2_DIR = OUTPUT_PIN;
	SHARP_LED3_DIR = OUTPUT_PIN;
	SHARP_LED4_DIR = OUTPUT_PIN;
	SHARP_LED5_DIR = OUTPUT_PIN;
	SHARP_VIN_DIR =  OUTPUT_PIN;
	
	e_sharp_led_clear();
	e_sharp_off();

}


int e_get_dist_sharp()
{
	int dist;
	dist = e_get_acc(0);				// read the sharp output 
	return dist;
}

//not used
void e_set_sharp_led(unsigned int sharp_led_number, unsigned int value)
// sharp_led_number between 1 and 5, value 0 (off) or 1 (on)
// if sharp_led_number other than 1-5, all leds are set to value
{
	switch(sharp_led_number)
	{
		case 1: 
			{
			if(value>1)
				SHARP_LED1 = SHARP_LED1^1;
			else
				SHARP_LED1 = !value;
			break;
			}
		case 2: 
			{
			if(value>1)
				SHARP_LED2 = SHARP_LED2^1;
			else
				SHARP_LED2 = !value;
			break;
			}
		case 3: 
			{
			if(value>1)
				SHARP_LED3 = SHARP_LED3^1;
			else
				SHARP_LED3 = !value;
			break;
			}
		case 4: 
			{
			if(value>1)
				SHARP_LED4 = SHARP_LED4^1;
			else
				SHARP_LED4 = !value;
			break;
			}
		case 5: 
			{
			if(value>1)
				SHARP_LED5 = SHARP_LED5^1;
			else
				SHARP_LED5 = !value;
			break;
			}
		default: 
			SHARP_LED1 = SHARP_LED2 = SHARP_LED3 = SHARP_LED4 = SHARP_LED5 = !value;
	}
}

void e_sharp_led_clear(void)
{
//Inverter 1=^off  0=^on
	SHARP_LED1 = 1;
	SHARP_LED2 = 1;
	SHARP_LED3 = 1;
	SHARP_LED4 = 1;
	SHARP_LED5 = 1;
}

void e_sharp_on(void)
{
	SHARP_VIN = 0;
}
void e_sharp_off(void)
{
	SHARP_VIN = 1;
}



