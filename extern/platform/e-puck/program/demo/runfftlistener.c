/********************************************************************************

			Programm to demonstrate how FFT works				          
			Version 2.0 août 2007				                          
			Michael Bonani, Jonathan Besuchet


This file is part of the e-puck library license.
See http://www.e-puck.org/index.php?option=com_content&task=view&id=18&Itemid=45

(c) 2004-2007 Michael Bonani, Jonathan Besuchet

Robotics system laboratory http://lsro.epfl.ch
Laboratory of intelligent systems http://lis.epfl.ch
Swarm intelligent systems group http://swis.epfl.ch
EPFL Ecole polytechnique federale de Lausanne http://www.epfl.ch

**********************************************************************************/

/*! \file
 * \brief The frequency recognizer using FFT
 * \section sect1 Introduction
 * The runfftlistener programm is made to illustrate how you can use the
 * FFT package of the library. The goal is to determine the frequency of
 * the sound comming from the microphone number 0. If the frequency is 
 * under 900Hz the e-puck will turn left. If the frequency is between
 * 900Hz and 1800Hz the e-puck will go forward. If the frequency is over
 * 1800Hz the e-puck will turn right.
 * 
 * \section sect2 Playing the demo
 * First of all, move the selector to the position 9 to 15 and reset the e-puck.
 * To play the demo, you have two allternatives:
 * - You are a good whistler. In this case you can drive the e-puck with
 * your mouse by whistling in the good frequency.
 * - You do not know whistlering. In this case you can play the "sound1.mp3"
 * or "sound2.mp3" which are in the folder "demo" to drive the e-puck.
 *
 * \section sect3 Video of the demo
 * - Driving the e-puck by whistlering and playing sound on PC: http://www.youtube.com/watch?v=bfHFo79uZGY
 *
 * \author Code: Michael Bonani, Jonathan Besuchet \n Doc: Jonathan Besuchet
 */

#include <p30Fxxxx.h>
#include <dsp.h>

#include "math.h"

#include "motor_led/e_epuck_ports.h"
#include "motor_led/e_init_port.h"
#include "a_d/advance_ad_scan/e_ad_conv.h"
#include "fft/e_fft.h"
#include "fft/e_fft_utilities.h"
#include "motor_led/advance_one_timer/e_motors.h"
#include "motor_led/advance_one_timer/e_agenda.h"
#include "motor_led/advance_one_timer/e_led.h"


/* Extern definitions */
/* Définitions externes des variables globales, des différents tableaux ou seront stockés les signaux des différents micros et la FFT du micro choisi, et
définitions de fonctions utiles de moyennage et de copie d'un buffer à l'autre*/ 
/* Typically, the input signal to an FFT routine is a complex array containing samples of an input signal. */
/* For this example, we will provide the input signal in an array declared in Y-data space. */
extern fractcomplex sigCmpx[FFT_BLOCK_LENGTH] __attribute__ ((section (".ydata, data, ymemory"),aligned (FFT_BLOCK_LENGTH * 2 *2)));      		
/* Access to the mic. samples */
extern int e_mic_scan[3][FFT_BLOCK_LENGTH];


/*! \brief Localize the bigger pic of the array
 * \param spectre The array in which the FFT was made
 * \param spectre_length The length of the scan in the array
 * \return The index of the bigger pic detected
 */
int localise_pic_max(fractcomplex *spectre, int spectre_length)
{			
	int i = 0 ;
	long ampl_max= 0 ;	// Initialisation de l'amplitude maximale
	long ampl_courante = 0 ;  //Initialisation de l'amplitude courante
	int pic_max;
	for (i = 0; i < spectre_length/2; i++)	
	{
		ampl_courante = spectre[i].real*spectre[i].real+spectre[i].imag*spectre[i].imag ; // Calcul de l'amplitude de la FFT à la position i courante
		
		if (ampl_courante>ampl_max)						// Si l'amplitude courante est plus grande que l'amplitude maximale mémorisée jusqu'ici...
			{
				pic_max = i ;							// La position du Maxima est mémorisée dans k_max_1
				ampl_max = ampl_courante ;				// La valeur de l'amplitude maximum est remplacée par la valeur courante
			}
	}
	return pic_max;
}

/*! \brief Get the max volume of the sound detected
 * \param spectre The array in which the FFT was made
 * \param pic_pos The index of the louder frequency
 * \return The amplitude of the louder frequency
 */
int get_volume(fractcomplex *spectre, int pic_pos)
{
	if(pic_pos < 0 || pic_pos >= FFT_BLOCK_LENGTH/2)
		return 0;
	return spectre[pic_pos].real*spectre[pic_pos].real+spectre[pic_pos].imag*spectre[pic_pos].imag;
}

/*! \brief Display the volume of the soud detected on the LEDs
 * \param volume The max volume of the sound detected
 */
void display_volume_on_led(int volume)
{
	e_led_clear();
	if(volume > 5 && volume < 20)
		e_set_led(4, 1);
	else if(volume >= 20 && volume < 50)
	{
		e_set_led(4, 1);
		e_set_led(3, 1);
		e_set_led(5, 1);
	} else if(volume >= 50 && volume < 200)
	{
		e_set_led(4, 1);
		e_set_led(3, 1);
		e_set_led(5, 1);
		e_set_led(2, 1);
		e_set_led(6, 1);
	} else if(volume >= 20 && volume < 300)
	{
		e_set_led(4, 1);
		e_set_led(3, 1);
		e_set_led(5, 1);
		e_set_led(2, 1);
		e_set_led(6, 1);
		e_set_led(1, 1);
		e_set_led(7, 1);
	} else if(volume >= 300)
	{
		e_set_led(8, 1);
	}
}

/*! \brief Calcul the corresponding frequency of the bigger pic detected
 * \param pic_pos The index of the bigger pic detected
 * \return The corresponding frequency
 */
int calcul_frequence(int pic_pos)
{
	return (pic_pos*33000)/FFT_BLOCK_LENGTH;
}

/*! \brief Set the speed of the e-puck
 *
 * Set the speed of the e-puck relatively of the frequency
 * of the sound detected.
 * - Turn left if the frequency is under 900Hz
 * - Turn right if the frequency is over 1800Hz
 * - Go forward if the sound is between 900Hz and 1800Hz
 *
 * \param frequency The frequence of the sound
 */
void set_speed(int frequency)
{
	if(frequency < 900)
		e_set_speed(300, 200);
	else if(frequency > 1800)
		e_set_speed(300, -200);
	else
		e_set_speed(400, 0);
}


/*! \brief The "main" function of the demo */
void run_fft_listener(void)
{
	int volume;
	int pos_pic;
	int frequency;

	// Initialisations
	e_init_port();
	e_init_motors();
	e_start_agendas_processing();
	e_init_ad_scan(MICRO_ONLY);
	
	e_set_speed(400, 0);

	while(1)
	{
		// Scanage des microphone	
		e_ad_scan_on();

		// Attente de l'acquisition de toute les valeurs
		while(!e_ad_is_array_filled());
		e_ad_scan_off();

		// Centrage du signal au point zéro (moyenne = 0)
		e_subtract_mean(e_mic_scan[0], FFT_BLOCK_LENGTH, LOG2_BLOCK_LENGTH);

		// Copie le signal du micro zero dans le buffer destiné à effectuer la FFT
		// Affecte les valeurs à sigCmpx.real (réels) et 0 à sigCmpx.imag (imaginaires)
		e_fast_copy(e_mic_scan[0], (int*)sigCmpx, FFT_BLOCK_LENGTH);
		
		// Le résultat est sauvegardé => On démarre une nouvelle acquisition
		e_ad_scan_on();

		// Execution de la FFT sur le buffer
		e_doFFT_asm(sigCmpx);

		// Recherche de la position K des deux fréquences maximum 				
		pos_pic = localise_pic_max(sigCmpx, FFT_BLOCK_LENGTH);
					
		volume = get_volume(sigCmpx, pos_pic);
		display_volume_on_led(volume);
		if(volume > 100)
		{
			frequency = calcul_frequence(pos_pic);
			set_speed(frequency);
		}
	}		
}
