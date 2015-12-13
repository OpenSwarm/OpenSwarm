/********************************************************************************

			Fast Fourier transform module
			August 2007: first version Jonathan Besuchet


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2007 Jonathan Besuchet

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \ingroup fft
 * \brief Some fft features.
 * 
 * Here you have two functions that are really usefull to work
 * with the FFT
 * \author Code & doc: Jonathan Besuchet
 */
#ifndef _FFT_UTILITIES
#define _FFT_UTILITIES

/*! \brief Copy an array to an other array, using REPEAT instruction
 * \param in_array The array from which you want to copy
 * \param out_array The destination array
 * \param size The number of element you want to copy
 */
void e_fast_copy(int* in_array, int* out_array, int size);

/*! \brief Substract the mean from the samples of the array (-> produce zero-mean samples)
 * \param array The array that you want to produce zero-mean samples
 * \param size The size of the array
 * \param log2size The log in base 2 of the size
 */
void e_subtract_mean(int* array, int size, int log2size);

#endif
