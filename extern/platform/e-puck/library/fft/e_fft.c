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
 * \author Code & doc: Jonathan Besuchet
 */

#include <dsp.h>

#include "e_fft.h"

/* Twiddle Factor array in Program memory */
extern const fractcomplex twiddleFactors[FFT_BLOCK_LENGTH/2] __attribute__ ((space(auto_psv), aligned (FFT_BLOCK_LENGTH*2)));

/*! \brief Execute the FFT with dsPic special instructions 
 * \param sigCmpx The pointer of the begining of the array on
 * which you want to perform the FFT.
 */
void e_doFFT_asm(fractcomplex* sigCmpx){
	/* Perform FFT operation */
	FFTComplexIP (LOG2_BLOCK_LENGTH, &sigCmpx[0], (fractcomplex *) __builtin_psvoffset(&twiddleFactors[0]), (int) __builtin_psvpage(&twiddleFactors[0]));
	
	/* Store output samples are stored in bit-reversed order of their addresses. -> Reorder them */
	BitReverseComplex (LOG2_BLOCK_LENGTH, &sigCmpx[0]);
}
