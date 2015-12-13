/**************************************************************************
* 			Titre	 													  *
*			Version 1.0 Date											  *
*			Author										                  *
*									                                      *
**************************************************************************/

/*! \file
 * \ingroup ComModule
 * \brief Radio communication	
 * \author Jonathan Besuchet
 */

/*! \defgroup ComModule Radio communication
 * 
 * \section comment_sect Introduction
 * This module is made for an extention of the e-puck.
 * No documentation is available for now.
 */

#ifndef _COM_MODULE
#define _COM_MODULE


#define COM_MODULE_HW_ATTENUATOR_25DB	1
#define COM_MODULE_HW_ATTENUATOR_0DB	0
#define COM_MODULE_DEFAULT_GROUP		0x7d

#define COM_MODULE_MAXSIZE 				108

#include "p30f6014A.h"

void InitComModule(unsigned char owngroup, unsigned int ownaddress, unsigned char hardwareattenuatormode, unsigned char softwareattenuatorvalue);
// OwnGroup : module will only receive packet of that group ID.
// hardwareAttenuator : COM_MODULE_HW_ATTENUATOR_25DB or COM_MODULE_HW_ATTENUATOR_0DB
// softAttenuator : value from 1 (full power) to 31 (-25dB)

int IsModulePlugged(); // return 0 if nothing connected, 1 if module is here.

void SetRadioEnabledState(unsigned char mode);// enable (1) or disable (0) the radio part of the module
void SetHardwareAttenuator(unsigned char AttenuatorMode);	// COM_MODULE_HW_ATTENUATOR_25DB or COM_MODULE_HW_ATTENUATOR_0DB
void SetSoftwareAttenuator(unsigned char AttenuatorValue); // value from 1 (full power) to 31 (-25dB)
void SetOwnGroup(unsigned char GroupID); // module will only receive packet of that group ID.
void SetOwnAddress(unsigned int ownaddress); // current address of the module

unsigned char GetRadioEnabledState(); // renturn if radio in enabled or not;
unsigned char GetHardwareAttenuator();	// return COM_MODULE_HW_ATTENUATOR_25DB or COM_MODULE_HW_ATTENUATOR_0DB
unsigned char GetSoftwareAttenuator(); // return value from 1 (full power) to 31 (-25dB)
unsigned char GetOwnGroup(); // return module will only receive packet of that group ID.

unsigned char GetStatus(); // return status register 
//bit 0 : PACKET_READY_FLAG : a packet is in receive buffer. auto clear when last byte of receive buffer is read
//bit 1 : TX_IDLE_FLAG 		: module is ready to send a new packet
//bit 2 : PACKET_LOST_FLAG  : a packet was in receive buffer when another packet arrived. 2.nd packet is lost. auto clear when last byte of receive buffer is read
//bit 3 : TX_SEND_ERROR		: error durint packet send. auto clear during next send attempt.

int SendPacket(unsigned char destinationgroup, unsigned int destinationaddress, unsigned char* packet, int packetsize); // return 1 if OK, 0 if error
// destinationGroup : the destination group of the packet 0xFF is broadcast group
// destinationaddress : the address of the destination module. 0xff is broadcast address
// packet : unsigned char array of data to be sent
// packetSize : number of fields of the unsigned char array to be sent. ( max is COM_MODULE_MAXSIZE bytes )

int IsPacketReady(unsigned char* packet, int* packetSize); // return 1 and the packet + size if new packet receive. 0 if not.
// packet : unsigned char array of data to put receive data in. must be COM_MODULE_MAXSIZE bytes array.
// packetSize : the effective data size receive.

#endif
