/**************************************************************************
* 			Programm which locate the direction of the sound		      *
*			Version 1.0 août 2007				                          *
*			Michael Bonani, Jonathan Besuchet				              *
*									                                      *
**************************************************************************/

/*! \file
 * \brief Locate the direction of the sound
 * \section sect1 Introduction
 * This demo uses the three microphones to locate the direction of a sound.
 * \n To determine the direction of the sound we use the fact that the sound
 * is detected by each microphone on different time. As we know the sound
 * speed, we can determine from where come the sound.
 * 
 * \section sect_sound2 Playing the demo
 * First of all, move the selector to the position 1 and reset the e-puck.
 * Clap your hands and the e-puck will turn on the LED which correspond to 
 * the direction of the sound (one second). Then the e-puck turn on himslef
 * and look in the direction where the sound was made.
 *
 * \section sect_sound3 Video of the demo
 * The video of this demo: http://www.youtube.com/watch?v=1RRqZMmfrfI
 *
 * \author Jonathan Besuchet
 */
#ifndef _LOCATES
#define _LOCATES

void run_locatesound();

#endif
