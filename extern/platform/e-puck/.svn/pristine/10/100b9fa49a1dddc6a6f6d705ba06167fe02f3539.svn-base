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

/*! \defgroup matlab Matlab communication
 * 
 * \section intro_sec Introduction
 * This package contains all the ressources you need to communicate with matlab
 * through bluetooth.
 * \n \n To make the communication possible, you have to follow this steps:
 * - Open matlab and set the default direcories to "...\library\matlab\matlab files\"
 * - Connect your e-puck to your PC with bluetooth
 * - Call the matlab function "OpenEpuck('COMX')"; X is the number of the port on which
 * the e-puck is connected.
 * 
 * \subsection intro_subsec Sending data from matlab
 * If you want to send data from matlab you only have to call the matlab function
 * "EpuckSendData(data, dataType)"
 * - data is the array of value you want to send;
 * - dataType is a string argument which can be 'int8' to send char or 'int16' to send
 * int;
 * 
 * On the e-puck side, to receive the data, you have to call the appropiate function
 * depending of the data type you will receive. For exemple if matlab send int data,
 * you have to call: \ref e_receive_int_from_matlab(int *data, int array_size).
 *
 * \subsection intro_subsec2 Sending data from e-puck
 * Now if you want to send data from e-puck to matlab you have to call the function
 * \ref e_send_int_to_matlab(int* data, int array_size) (send int data) on e-puck side and call
 * "EpuckGetData" on matlab side. This function make the data convertion automatically,
 * so call it to receive both of 'char' or 'int' data.
 * \sa matlab
 * \author Doc: Jonathan Besuchet
 */

#ifndef MATLAB_H
#define MATLAB_H

void e_send_int_to_matlab(int* data, int array_size);
void e_send_char_to_matlab(char * data, int array_size);
int e_receive_int_from_matlab(int* data, int array_size);
int e_receive_char_from_matlab(char* data, int array_size);

#endif // MATLAB_H
