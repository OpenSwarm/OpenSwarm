#include "ComModule.h"

#include <stdlib.h>
#include "./motor_led/e_epuck_ports.h"
#include "./I2C/e_I2C_protocol.h"
#include "./I2C/e_I2C_master_module.h"

#define COM_MODULE_I2C_ADDR	(0x3F<<1)

#define BUFFER_DATA_LENGTH 		(COM_MODULE_MAXSIZE + 14)


#define STATUS_REG_ADDR   		0x00  // status like PACKET_LOST_FLAG,TX_IDLE,PACKET_READY
#define CONFIG_REG_ADDR   		0x01  // config like ..... hardware att. sleep...
#define SEND_REG_ADDR     		0x02  // <> 0 -> send sendbuffer
#define SOFTATT_REG_ADDR  		0x03
#define OWNGROUP_REG_ADDR  		0x04
#define OWNADDRL_REG_ADDR 		0x05
#define OWNADDRH_REG_ADDR 		0x06
#define SEND_BUFFER_START 		0x07
#define SEND_BUFFER_END   		(SEND_BUFFER_START + BUFFER_DATA_LENGTH)
#define REC_BUFFER_START  		(SEND_BUFFER_END + 1)
#define REC_BUFFER_END    		(REC_BUFFER_START + BUFFER_DATA_LENGTH)

#define AM_MSGTYPE			0x0A // AM_OSCOPE

#define AM_MSGTYPE_IN_PACKET_OFFSET	0x08
#define ADDRLDATA_IN_PACKET_OFFSET	0x06
#define ADDRHDATA_IN_PACKET_OFFSET	0x07
#define TYPEDATA_IN_PACKET_OFFSET	0x08
#define GROUPDATA_IN_PACKET_OFFSET	0x09
#define FIRSTDATA_IN_PACKET_OFFSET	0x0A // how many byte after packet start should I write data ?

//status reg
#define PACKET_READY_FLAG	0x01 // bit 1
#define TX_IDLE_FLAG		0x02 // bit 2
#define PACKET_LOST_FLAG	0x04 // bit 3
#define TX_SEND_ERROR		0x08 // bit 4

//config reg
#define HARDWAREATT_SET_FLAG	0x01 //bit 1
#define RADIO_ENABLED_FLAG		0x80 //bit 8

// send reg
#define REQUEST_TO_SEND_FLAG	0x01

unsigned char ReadRegister(unsigned char registeraddr) {
	return (0x00FF & e_i2cp_read(COM_MODULE_I2C_ADDR, registeraddr));	
}

void WriteRegister(unsigned char registeraddr, unsigned char value) {
	e_i2cp_write(COM_MODULE_I2C_ADDR, registeraddr, value);
}

void InitComModule(unsigned char owngroup, unsigned int ownaddress, unsigned char hardwareattenuatormode, unsigned char softwareattenuatorvalue) {
	e_i2cp_init();
	e_i2cp_enable();
                
	while (IsModulePlugged() == 0); // wait till module online

	SetOwnGroup(owngroup);
	SetOwnAddress(ownaddress);
	SetHardwareAttenuator(hardwareattenuatormode);
	SetSoftwareAttenuator(softwareattenuatorvalue);
	SetRadioEnabledState(1);
}

int IsModulePlugged() {
	unsigned char value;
	value = GetStatus();
	if (value == 0xFF) // value read are always 0xff when module not plugged in
		return 0;
	return 1;
}

void SetRadioEnabledState(unsigned char mode) {
	unsigned char old_value;	
	old_value = ReadRegister(CONFIG_REG_ADDR);
	if (mode != 0)
		WriteRegister(CONFIG_REG_ADDR, old_value |= RADIO_ENABLED_FLAG);
	else
		WriteRegister(CONFIG_REG_ADDR, old_value &= ~RADIO_ENABLED_FLAG);
}

void SetHardwareAttenuator(unsigned char attenuatormode) {
	unsigned char old_value;	
	old_value = ReadRegister(CONFIG_REG_ADDR);
	if (attenuatormode != 0)
		WriteRegister(CONFIG_REG_ADDR, old_value |= HARDWAREATT_SET_FLAG);
	else
		WriteRegister(CONFIG_REG_ADDR, old_value &= ~HARDWAREATT_SET_FLAG);
}


void SetSoftwareAttenuator(unsigned char attenuatorvalue) {
	if (attenuatorvalue > 31)
		attenuatorvalue = 31;
	WriteRegister(SOFTATT_REG_ADDR, attenuatorvalue);
}

void SetOwnGroup(unsigned char owngroup) {
	WriteRegister(OWNGROUP_REG_ADDR, owngroup);
}

void SetOwnAddress(unsigned int ownaddress) {
	WriteRegister(OWNADDRL_REG_ADDR, ownaddress & 0xFF);
	ownaddress=ownaddress>>8;
	WriteRegister(OWNADDRH_REG_ADDR, ownaddress & 0xFF);
}

unsigned char GetHardwareAttenuator() {
	if (ReadRegister(CONFIG_REG_ADDR) & HARDWAREATT_SET_FLAG);
		return 1;
	return 0;
}

unsigned char GetRadioEnabledState() {
	if (ReadRegister(CONFIG_REG_ADDR) & RADIO_ENABLED_FLAG);
		return 1;
	return 0;
}

unsigned char GetSoftwareAttenuator() {
	return ReadRegister(SOFTATT_REG_ADDR);
}

unsigned char GetOwnGroup() {
	return ReadRegister(OWNGROUP_REG_ADDR);
}

unsigned int GetOwnAddress() {
	unsigned char lowbyte, highbyte;
	lowbyte=ReadRegister(OWNADDRL_REG_ADDR);
	highbyte=ReadRegister(OWNADDRH_REG_ADDR);
	return lowbyte | ((int)highbyte<<8);
}

unsigned char GetStatus() {
	return ReadRegister(STATUS_REG_ADDR);
}

int SendPacket(unsigned char destinationgroup, unsigned int destinationaddress, unsigned char* packet, int packetsize) {
	unsigned int i;
	if (packetsize > COM_MODULE_MAXSIZE)
		return 0; // not allowed -> return error

	while ((GetStatus() & TX_IDLE_FLAG) == 0); // wait till module is ready to transmit

	WriteRegister(SEND_BUFFER_START + AM_MSGTYPE_IN_PACKET_OFFSET, AM_MSGTYPE);
	WriteRegister(SEND_BUFFER_START + GROUPDATA_IN_PACKET_OFFSET, destinationgroup);
	WriteRegister(SEND_BUFFER_START + ADDRLDATA_IN_PACKET_OFFSET, destinationaddress & 0xFF);
	destinationaddress=destinationaddress>>8;
	WriteRegister(SEND_BUFFER_START + ADDRHDATA_IN_PACKET_OFFSET, destinationaddress & 0xFF);
	for (i=0; i<packetsize; i++) {
		WriteRegister(FIRSTDATA_IN_PACKET_OFFSET+SEND_BUFFER_START+i, packet[i]);
	}
	WriteRegister(SEND_REG_ADDR,1); // send packet
	// maybe wait here
	if (GetStatus() & TX_SEND_ERROR) // flag TX error set -> return error
		return 0;
	return 1; // return OK
}

int IsPacketReady(unsigned char* packet, int* packetsize) {
	unsigned int i;
	if ((packet == NULL) || (packetsize == NULL))
		return 0; // return withour reading NULL pointer means uC reset if write
	if (GetStatus() & PACKET_READY_FLAG) { // a packet is ready !
		*packetsize = ReadRegister(REC_BUFFER_START); // get size of received buffer
		if (*packetsize > COM_MODULE_MAXSIZE)
			return 0; // error in transmission. size is too big and impossible
		for (i = 0; i< *packetsize ; i++) {
			packet[i] = ReadRegister(REC_BUFFER_START+FIRSTDATA_IN_PACKET_OFFSET+i);
		}
		ReadRegister(REC_BUFFER_END); 
		return 1;
	}
	for(i=0; i<0x1000; i++); // wait
	return 0; // no packet ready -> 0
}
