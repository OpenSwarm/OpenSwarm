/****************************************************************
* 	   LIS sensor extension module on e-puck			 				*
*		Walter Karlen &	Alessandro Ambuehl						*
* Version 1.2 24.8.2007 WK										*
* 		separated sensext from sharp, new definitions			* 
*		Removed from main loop
* Version 1.1 10.2006	AA										*
*		Initial release											*
****************************************************************/

/*attention: betwen the e-puck and the Sharp, an inverter buffer
inverts the control lines (LED 1-5 and Vin)*/

//#include "a_d/e_ad_conv.h"
//#include "motor_led/e_epuck_ports.h"
#include "e_sensext.h"
#include "e_sharp.h"
//#include "e_devantech.h"
//#include <a_d/advance_ad_scan/e_ad_conv.h>
#include <a_d/advance_ad_scan/e_acc.h>
#include <motor_led/advance_one_timer/e_agenda.h>
#include <I2C/e_I2C_master_module.h>
#include <I2C/e_I2C_protocol.h>

static int sensext_wait=0;

void e_stop_sensext_wait(void)
{
	sensext_wait=0;
}

void e_start_sensext_wait(void)
{
	sensext_wait=1;
}

int e_get_sensext_wait(void)
{
	return sensext_wait;
}

void e_init_sensext(void)
{
	e_activate_agenda(e_stop_sensext_wait,0);
}

int e_sensext_process(int* sensext_param, unsigned int* sensext_value) //check pointer
{

	unsigned char sensext_byteH, sensext_byteL;
//	unsigned int sensext_dist=0, sensext_light=0;
	unsigned char revision =255;
	
	if(sensext_param[0]==-1)			// Read the Devantech sensor
	{
		e_i2cp_enable();
	 
		//	sensext_dist=e_get_dist_devantech(I2C_ADDR_SRF10);
		e_i2cp_write (I2C_ADDR_SRF10, 0, 81); 		// start ranging comand - cm
		while ((revision==255))					// Cecking for completion of ranging (according datasheet)
		{
			revision = e_i2cp_read(I2C_ADDR_SRF10, 0);
		}
		sensext_byteH=e_i2cp_read(I2C_ADDR_SRF10, 2);
		sensext_byteL=e_i2cp_read(I2C_ADDR_SRF10, 3);
		//sensext_dist=sensext_byteH;
		//sensext_dist=(sensext_dist<<8)+sensext_byteL;
		sensext_value[0] = sensext_byteH<<8;
		sensext_value[0] += sensext_byteL;
		sensext_value[1] = (unsigned char)e_i2cp_read(I2C_ADDR_SRF10, 1); 
//sensext_dist = sensext_byteH<<8;
//	sensext_dist += sensext_byteL;
//		sensext_light = (unsigned char)e_i2cp_read(I2C_ADDR_SRF10, 1); 
//sensext_value[0] =	sensext_dist;
//sensext_value[1] = sensext_light;
		e_i2cp_disable();
	
		return TRUE;
		
 	//	sprintf(buffer,"w,%u,%u\r\n", sensext_dist, sensext_light);
	//					uart_send_text(buffer);		
	}
	else if((sensext_param[0]>=0)&&(sensext_param[0]<=31))		// read the analog sensors
	{
		SHARP_LED1 = !(sensext_param[0]&1);				// mask the bit. all 5 leds are individually set to 0 or 1
		SHARP_LED2 = !(sensext_param[0]&2);
		SHARP_LED3 = !(sensext_param[0]&4);
		SHARP_LED4 = !(sensext_param[0]&8);
		SHARP_LED5 = !(sensext_param[0]&16);
									
		if (sensext_param[1]!=sensext_param[0]) 
		{	//reset sensor (5ms) and wait 30 msec for stability when sensor configuration changes //!!!WALTER!!!!!
				e_sharp_off();
				//SHARP_VIN = 1; 
				e_start_sensext_wait();									
				e_set_agenda_cycle(e_stop_sensext_wait, 50);	// function to call after 5 ms 
				while(e_get_sensext_wait());
				e_set_agenda_cycle(e_stop_sensext_wait, 0);
				e_sharp_on();
				//SHARP_VIN = 0; 
				e_start_sensext_wait();									
				e_set_agenda_cycle(e_stop_sensext_wait, 300);	// function to call after 30 ms (25ms for sharp stability & 5ms for conversion)
				while(e_get_sensext_wait());
				e_set_agenda_cycle(e_stop_sensext_wait, 0);
		} 	
		sensext_value[0] = e_get_acc(0);							// read the sharp output 
	//	sprintf(buffer,"w,%d\r\n", sensext_dist);
	//	uart_send_text(buffer);	
		sensext_param[1]=sensext_param[0];		
	return TRUE;

	} else if(sensext_param[0]==-2)			// read the cmps03
	{
		e_i2cp_enable();
				
		
		sensext_byteL=e_i2cp_read(I2C_ADDR_CMPS03, 3); //!!!WALTER!!!!!

		sensext_byteH=e_i2cp_read(I2C_ADDR_CMPS03, 2); //!!!WALTER!!!!!
	//	sensext_value[0] =sensext_byteH;
	//	sensext_value[0] =(sensext_dist<<8)+sensext_byteL; 
		sensext_value[0] = sensext_byteH<<8;
		sensext_value[0] += sensext_byteL;		//1/10° //!!!WALTER!!!!!
		sensext_value[1] = (unsigned char)e_i2cp_read(I2C_ADDR_CMPS03, 1); //360°/255 //!!!WALTER!!!!!
		e_i2cp_disable();
//		sprintf(buffer,"w,%d,%d\r\n", sensext_dist, sensext_light);			// compass bearing, word and byte
//								uart_send_text(buffer);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
