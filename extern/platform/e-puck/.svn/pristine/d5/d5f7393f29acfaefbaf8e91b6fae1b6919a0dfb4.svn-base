/****************************************************************
* 			Devantech sensor of e-puck							*
*			Version 1.0 7.2006									*
*			Alessandro Ambuehl									*
****************************************************************/

#include "e_devantech.h"
#include "./../../I2C/e_I2C_master_module.h"
#include "./../../I2C/e_I2C_protocol.h"


void e_init_devantech(void)
{
	e_i2c_init();						// init I2C communication
	e_i2c_enable();					// enable interrupts of I2C
}

void e_disable_devantech(void)
{
	e_i2c_disable();				// disable I2C communication
}

// start ranging, wait & return the distance in cm
unsigned int e_get_dist_devantech(char device_add)
{
	unsigned int dist;
	unsigned char byte1, byte0;
	unsigned char revision =255;
	e_i2cp_write (device_add, 0, 81); 		// start ranging comand - cm
	
	while ((revision==255))					// Cecking for completion of ranging (according datasheet)
	{
		revision = e_i2cp_read(device_add, 0);
	}
	byte1=e_i2cp_read(device_add, 2);
	byte0=e_i2cp_read(device_add, 3);
	dist=byte1;
	dist=(dist<<8)+byte0;
	return dist;
}

// start ranging, wait & return the delay in uS
unsigned int e_get_delay_devantech(char device_add)
{
	int delay;
	char revision = 255;
	e_i2cd_write (device_add, 0, 82); // start ranging comand - us
	// Cecking for completion of ranging (according datasheet)
	while ((revision=255))
	{
		revision = e_i2cd_readb(device_add, 0);
	}
	
	delay=e_i2cd_readw(device_add, 2);
	return delay;
}

// returns the software revision of the sensor
char e_get_sr_devantech(char device_add)
{
	char sr;
	sr=e_i2cd_readb(device_add, 0);
	return sr;
}

// returns the light intensity
// Warning: the light intensity is not updated with this command
unsigned int e_get_light_devantech(char device_add)
{
	char light;
	light=e_i2cp_read(device_add, 1);
	return light;
}

// Set the analogue gain of the sensor
void e_set_gain_devantech(char device_add, char gain)
{
	e_i2cd_write (device_add, 1, gain);
}

// Set the maximl range (distance) 
void e_set_range_devantech(char device_add, char range)
{
	e_i2cd_write (device_add, 2, range);
}

// Write 1 byte to register reg of devantec sensor
void e_i2cd_write (char device_add, char reg, char value)
{
	e_i2c_start();
	e_i2c_write(device_add);    // Writing the device (slave) address
	e_i2c_write(reg);     		// Device register address
	e_i2c_write(value);       // Data to device
	e_i2c_stop();             // Ending the communication
	e_i2c_reset();
}

//read 1 byte from the register reg of devantec sensor
char e_i2cd_readb(char device_add, char reg)
{
	char value;

	e_i2c_start();				// start
	e_i2c_write(device_add);   	// Device address
	e_i2c_write(reg);     		// Register address

	e_i2c_restart();			// 2nd start
	e_i2c_write(device_add+1);    // Device address
 	e_i2c_read(&value);    		// read single byte
	e_i2c_nack();				// only 1 byte is being read, so send nack
	e_i2c_stop();             	// end read cycle
	e_i2c_reset();

   	return value;
}

// sequantial random read of 2 bytes (word)
/*unsigned int e_i2cd_readw(char device_add, char reg)
{
	char byte1, byte2;
	unsigned int dist=0;

	e_i2c_start();				// start
	e_i2c_write(device_add);   	// Device address
	e_i2c_write(reg);     		// Register address

	e_i2c_restart();			// 2nd start
	e_i2c_write(device_add+1);    // Device address
 	e_i2c_read(&byte1);    		// read byte 1
	e_i2c_ack();				// send ack
	e_i2c_read(&byte2);    		// read byte 2
	e_i2c_nack();				// end of word, so send nack
	e_i2c_stop();             	// end read cycle
	e_i2c_reset();

	dist=(unsigned int)byte1;
	dist=dist<<8;
	dist=dist+(unsigned int)byte2;
	dist=byte1;
	return dist;		
}*/
