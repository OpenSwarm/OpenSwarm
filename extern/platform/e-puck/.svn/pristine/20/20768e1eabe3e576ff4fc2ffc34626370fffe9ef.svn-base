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
 * \brief Package to mange the FFT.
 * 
 * In this package the FFT (fast fourier transform) is done with
 * the special dsPic 30fxxxx instructions.
 * \n Before calling the e_doFFT_asm() function, you must to fill
 * the sigCmpx array with your values. This array is stocked in the
 * Y memory of the dsPic.
 * \author Code & doc: Jonathan Besuchet
 */

/*! \defgroup fft FFT
 * 
 * \section intro_sec Introduction
 * The fast fourier transform (FFT) is really usefull, especially
 * to work with the microphones data. This package contains all
 * you need to perform the FFT.
 * \n The dsPic has some specials instructions (MAC instructions)
 * which are used here.
 *
 * \section howitwork_sec How it works
 * To do the FFT on your data, first make this:
 * - choose the size of the array in which the FFT will be done. 
 *   You have to choose one of the following values: 64, 128, 256 or
 *   512.
 * - put your choice in the \ref FFT_BLOCK_LENGTH (it's in the 
 *   file e_fft.h).
 * 
 * Then you just have
 * - to copy your data in the sigCmpx array with
 * the e_fast_copy(int* in_array, int* out_array, int size) function
 * - to call the e_doFFT_asm(fractcomplex* sigCmpx) function
 *
 * A little code to illustrate this.
 * \code
 * 	e_ad_scan_on();
 *	// waiting all the 512 data (here we scan the microphones)
 * while(!e_ad_is_array_filled());
 *	e_ad_scan_off();
 *
 *	// We put the mean to zero
 *	e_subtract_mean(e_mic_scan[0], FFT_BLOCK_LENGTH, LOG2_BLOCK_LENGTH);
 *	// We copy the array of micro zero to the FFT array
 *	e_fast_copy(e_mic_scan[0], (int*)sigCmpx, FFT_BLOCK_LENGTH);
 *	
 *	// The result is saved => we can launch a new acquisition
 *	e_ad_scan_on();
 *	// Now we are doing the FFT
 *	e_doFFT_asm(sigCmpx);
 * \endcode
 * 
 * \author Doc: Jonathan Besuchet
 */

#ifndef FFT_H
#define FFT_H

#include <dsp.h>

/* Constant Definitions */

// Number of frequency points in the FFT */
#define FFT_BLOCK_LENGTH 256 
    
// Number of "Butterfly" Stages in FFT processing
#if (FFT_BLOCK_LENGTH == 64)
#define LOG2_BLOCK_LENGTH 	6
#endif
#if (FFT_BLOCK_LENGTH == 128)
#define LOG2_BLOCK_LENGTH 	7
#endif
#if (FFT_BLOCK_LENGTH == 256)
#define LOG2_BLOCK_LENGTH 	8
#endif
#if (FFT_BLOCK_LENGTH == 512)
#define LOG2_BLOCK_LENGTH 	9
#endif

void e_doFFT_asm(fractcomplex* sigCmpx);

#endif // FFT_H
