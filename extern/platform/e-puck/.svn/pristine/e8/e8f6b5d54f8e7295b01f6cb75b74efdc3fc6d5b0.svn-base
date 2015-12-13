/********************************************************************************

			To communicate with matlab
			August 2007: first version 
			Michael Bonani, Jonathan Besuchet


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2007 Michael Bonani, Jonathan Besuchet

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \ingroup matlab
 * \brief To communicate with matlab.
 *
 * This module manage the communication with matlab through bluetooth.
 * \author Code: Michael Bonani, Jonathan Besuchet, Doc: Jonathan Besuchet
 */

#include "matlab.h"

#include "./../motor_led/e_epuck_ports.h"
#include "./../uart/e_uart_char.h"

/*! \brief The function to send int values to matlab
 * \param data The array of int data you want to send
 * \param array_size The length of the array
 */	
void e_send_int_to_matlab(int* data, int array_size) {
	int size = 2*array_size;
	
	e_send_uart1_char("I",1);
	e_send_uart1_char((char *) &size, sizeof(int));
	e_send_uart1_char((char*)data, size);
	e_send_uart1_char("EOF\0",4);
	
	return;
}

/*! \brief The function to send char values to matlab
 * \param data The array of char data you want to send
 * \param array_size The length of the array
 */
void e_send_char_to_matlab(char* data, int array_size) {
	e_send_uart1_char("C",1);
	e_send_uart1_char((char *) &array_size, sizeof(int));
	e_send_uart1_char(data, array_size);
	e_send_uart1_char("EOF\0",4);
	
	return;
}

/*! \brief The function to receive int values from matlab
 * \param data The array of int data you want to fill
 * \param array_size The length of the array
 * \return The number of int stored
 */
int e_receive_int_from_matlab(int* data, int array_size) {
	char c;
	int i=0;
	int temp_size=0;
	int flush = 0;
	
	do{
    	if (e_getchar_uart1(&c)) {	
		    // The first byte is the length of the datas
	    	if(i==0) {
		    	// LSB
		    	temp_size = (int)c;
		    }
			else if(i==1) {
			    // MSB
			    temp_size += ((int)c)<<8;
			    if((temp_size>>1) > array_size) {
				    // ! We will receive more data than the buffer can store
				    // Store as max data as possible, but excedded data will be flushed !
				    flush = 1;
					LED4 = LED4^1;
				    temp_size = (array_size<<1);
				}
		    }
			else {
			    // Then the datas come
			    if((i%2) == 0){
					// LSB
			    	data[i/2-1] = (int)c;
				}
				else{
			    	// MSB
			    	data[(i-1)/2-1] = (data[(i-1)/2-1] & 0b0000000011111111) | ((int)c)<<8;
				}
			}
     		i++;
		}	
    }
    while(i<2 || i<=(temp_size+1));
    
    // Flush pending data
    if(flush) {
		while(e_getchar_uart1(&c));
	}
    
    // return the number of int. stored
    return (temp_size >> 1);
}

/*! \brief The function to receive char values from matlab
 * \param data The array of char data you want to fill
 * \param array_size The length of the array
 * \return The number of char stored
 */
int e_receive_char_from_matlab(char* data, int array_size) {
	char c;
	int i=0;
	int temp_size=0;
	int flush = 0;

	do{
    	if (e_getchar_uart1(&c)) {	
		    // The first byte is the length of the datas
	    	if(i==0) {
		    	// LSB
		    	temp_size = (int)c;
		    }
 			else if(i==1) {
			    // MSB
			    temp_size += ((int)c)<<8;
			    if(temp_size > array_size){
				    // ! We will receive more data than the buffer can store
				    // Store as max data as possible, but excedded data will be flushed !
				    flush = 1;
				    temp_size = array_size;
				}
		    }
			else {
			    // Then the datas come	
			   	data[i-2] = c;
			}
     		i++;
		}	
    }
    while(i<2 || i<=(temp_size+1));
    
    // Flush pending data
    if(flush) {
		while(e_getchar_uart1(&c));
	}
    
    // return the number of char. stored
    return temp_size;
}
