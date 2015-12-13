/****************************************************************
* 			Devantech sensor of e-puck							*
*			Version 1.0 7.2006									*
*			Alessandro Ambuehl									*
****************************************************************/

/*! \file
 * \ingroup LIS_SENSOR_TURRET
 * \brief Devantech sensor of e-puck	
 * \author Jonathan Besuchet
 */

/*! \defgroup LIS_SENSOR_TURRET LIS sensor turret
 * 
 * \section comment_sect Introduction
 * This module is made for an extention of the e-puck.
 * No documentation is available for now.
 */

#ifndef _devantech
#define _devantech

#include "./../../I2C/e_I2C_master_module.h"
#include "./../../motor_led/e_epuck_ports.h"

// Initialisation & disable
void e_init_devantech(void);
void e_disable_devantech(void);

// Commands to initiate a ranging
unsigned int e_get_dist_devantech(char device_add);
unsigned int e_get_delay_devantech(char device_add);

// Optional informations
char e_get_sr_devantech(char device_add);		// Read the Software Revision
unsigned int e_get_light_devantech(char device_add);	// Read the light sensor. return 0x80 if not avaiable

// Settings
void e_set_gain_devantech(char device_add, char gain);
void e_set_range_devantech(char device_add, char range);

// write command
void e_i2cd_write (char device_add, char reg, char value);

// Read commands
char e_i2cd_readb(char device_add, char reg);					//read 1 byte
unsigned int e_i2cd_readw(char device_add, char reg);		//read 1 word

#endif
