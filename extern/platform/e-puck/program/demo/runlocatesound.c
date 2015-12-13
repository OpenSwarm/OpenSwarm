/********************************************************************************

			Programm which locate the direction of the sound		      
			Version 1.0 août 2007				                          
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
 * \author Code: Michael Bonani, Jonathan Besuchet \n Doc: Jonathan Besuchet
 */

#include "p30f6014A.h"
#include <math.h>
#include "motor_led/advance_one_timer/e_agenda.h"
#include "motor_led/advance_one_timer/e_motors.h"
#include "a_d/advance_ad_scan/e_ad_conv.h"
#include "a_d/advance_ad_scan/e_micro.h"

#include "runlocatesound.h"

#define PI 3.1415

/* defines used in main.c                          */
#define MEAN_MAX 100		// Length of the mean_table
#define MEAN_MAX_INV 0.01	// Inversed value of MEAN_MAX
#define PERCENT 0.1			// Defines range to distinguish between noise an signal

/* defines used in ad_conv_int.c					*/
//#define MIC_SAMP_NB 100		// number of microphon samples to store

/* defines used in find_direction					*/
//maximum_delta_t = 
//sampling_frequency[Hz] * distance_between_microphones[m] / speed_of_sound[m/s];
#define MAXIMUM_DELTA_T1 6.
#define MAXIMUM_DELTA_T2 5.

/* defines used in find_delta_t.c					*/
#define TAU_RANGE 14		// Needs to be a pair number

/* defines used in turn_to_direction.c */
#define TURN_SPEED 1000
#define STEPS_FOR_2PI 1300.

extern int e_mic_scan[3][MIC_SAMP_NB];				//Array to store the mic values
extern unsigned int e_last_mic_scan_id;				//ID of the last scan in the mic array
extern unsigned char is_ad_acquisition_completed;	//to check if the acquisition is done

//int new_sample;
float mean_table[3][MEAN_MAX];
int mean_nb;
float mean[3];
float signal_max[3], signal_min[3];


void calculate_average(int *);
void init_micro(void);
void record_sound(void);
int check_for_event(void);
void filter_signal(void);
void calculate_average(int *current_sample);
//int get_micro (unsigned int micro_ID);
int get_micro_average(unsigned int micro_ID, unsigned int filter_size);
float calculate_direction(void);
int find_delta_t(int mic1_nb,int mic2_nb);
void show_led(float angle);
void turn_to_direction(float direction);


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		init sound
//
// Initialize everything necessary to record sound
//
// in:  void
// out: void
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void init_micro(void)  
{
	int j, k;
	
	mean_nb = 0;			// start to fill up the table from position 0
	while(!e_ad_is_acquisition_completed()); 		// wait until a new sample has been taken

	// initialize the table of average values
	// save signal level
	for (k=0; k<3; k++)
	{
		mean[k]=(float)e_get_micro((unsigned) k);     // start at a given average value
	}
	
    for( k=0; k<3; k++)
    {
		for (j=0; j<MEAN_MAX; j++)      // fill the mean_table with predefined values
		{
			mean_table[k][j]= mean[k] * MEAN_MAX_INV;
		}

	signal_min[k] = mean[k] - PERCENT * mean[k];	// predefine level for eventdetecting
	signal_max[k] = mean[k] + PERCENT * mean[k];
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		record sound
//
// Fills up the memory with the recorded sound from all
// three microphones at a sampling rate of about 25kHz
// 
// in:  void
// out: void
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void record_sound(void)
{
	e_last_mic_scan_id = 0;   // reset the scan_ID to 0 so that we have the full table with the sound
	while(!e_ad_is_acquisition_completed()); // e_last_mic_scan_id <= (MIC_SAMP_NB - 2) ); //wait until the table is full
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		check for event
//
// Checks if an event has happened. This function dynamically 
// takes the average of the soundstream and then checks, whether
// it is tresspassing a predefined treshold.
// 
// in:  void
// out: int (1: no event has occured)
//          (0: an event has occured!)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int check_for_event(void)
{
	int not_event;  
	int current_sample[3];
	
	// get one single sample for all 3 microphones
	current_sample[0]=e_get_micro((unsigned) 0);
	current_sample[1]=e_get_micro((unsigned) 1);
	current_sample[2]=e_get_micro((unsigned) 2);

    // Detect event on any of the 3 microphones
	not_event = (  ((current_sample[0]<signal_max[0]) && (current_sample[0]>signal_min[0])) ||
				   ((current_sample[1]<signal_max[1]) && (current_sample[1]>signal_min[1])) ||
				   ((current_sample[2]<signal_max[2]) && (current_sample[2]>signal_min[2])) ); 
					
	calculate_average(current_sample);		// dynamically calculates the new average value of the noise

	return not_event;   					// if no event, return 1 
	                    					// if event, return 0
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		filter signal
//
// Filters the signal, so that the detect_direction module 
// has an optimum signal.
// This includes shifting the signal around zero and then
// takes the absolute value.
// 
// in:  void
// out: void
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void filter_signal(void)
{
	int i, k;

	for (i=0; i<3; i++)  							// for all three mics
	{
		for (k = 0; k < MIC_SAMP_NB; k++)			// for the whole signal
		{
			e_mic_scan[i][k] -= mean[i];				// shift the signal down to around 0
			if (e_mic_scan[i][k] < 0)			        // take the absolute value 
				e_mic_scan[i][k] = -e_mic_scan[i][k];   // --> gives better values in the cross-correlation	
		}
	}
}	

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//		Private functions
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		calculate average
//
// Dynamically calculates the average of the sound stream. 
// This is necessary because the average signal coming from the 
// ADC is not always the same for all 3 microphones and tests
// have shown that it may shift during the exexution of the program
// 
// in:  pointer to the current sample
// out: void
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void calculate_average(int *current_sample)
{
	int k;
	
	while(!e_ad_is_acquisition_completed()); 				// wait until a new sample has been taken
	
	for( k=0; k<3; k++)				// for all three mics calculate average
    {
		mean[k] = mean[k] - mean_table[k][mean_nb];
		mean_table[k][mean_nb] = MEAN_MAX_INV * (float)current_sample[k];
		mean[k] += mean_table[k][mean_nb];
				
		// adapt treshold to detect an event
		signal_min[k] = mean[k] - PERCENT * mean[k];
		signal_max[k] = mean[k] + PERCENT * mean[k];
	}
	
	// ensure a circular memory usage
	if (mean_nb<MEAN_MAX-1)
		mean_nb++;
	else
		mean_nb = 0;
}

/**********************************************************************
 * Get the average on a given number of sample from a micro
 *
 * @param micro_ID		IN	micro's ID (0, 1, or 2)
 *							(use MICR0, MICR1, MICR2 defined in ad_conv_int.h)
 * @param filter_size	IN	number of sample to average
 * @return result		OUT last value of the micro
 **********************************************************************/
int get_micro_average(unsigned int micro_ID, unsigned int filter_size)
{
	long temp = 0;
	int i,j;

	// channel ID must be between 0 to 2 and 
	// filter_size must be between 1 to SAMPLE_NUMBER
	if ((micro_ID < 3) && 
		(filter_size > 0) && (filter_size <= MIC_SAMP_NB))
	{
		for (i=0, j=(MIC_SAMP_NB-(filter_size-(e_last_mic_scan_id+1)))%MIC_SAMP_NB ; i<filter_size ; i++, j=(j+1)%MIC_SAMP_NB)
		{
			temp += e_mic_scan[micro_ID][j];
		}
	}
	return ((int)(temp/filter_size));
}

float calculate_direction(void)
{
	int delta_t1, delta_t2;
	float direction, angle1, angle2;	
	
	// first get the phase-shift between the right and the left microphone
	delta_t1 = find_delta_t(0,1);
	
	// calculate the angle (between -90° and +90° where the sound is coming from)
	if (delta_t1 >= MAXIMUM_DELTA_T1)
			angle1 = PI * 0.5; 			// to avoid NaN of asin
	else if (delta_t1 <= -MAXIMUM_DELTA_T1)
			angle1 = -PI * 0.5; 		// to avoid NaN of asin
	else 
		angle1 = asin( (float)(delta_t1)/MAXIMUM_DELTA_T1 );
	
	
	// now if the signal is coming from the right, we check the phase-shift between the
	// left and the rear microphone in order to find out if the direction is 
	// angle1 or (180° - angle1)
	// if thes signal coming from the left, we make the same test with the right and the 
	// rear microphone	
	if(angle1 > 0)
	{
		delta_t2 = find_delta_t(2,1);   // phase shift between left and rear microphone

		if (delta_t2 >= MAXIMUM_DELTA_T2)
			angle2 = PI * 0.5; 			// to avoid NAN of asin
		else if (delta_t2 <= -MAXIMUM_DELTA_T2)
			angle2 =-PI * 0.5; 			// to avoid NAN of asin
		else 
			angle2 = asin( (float)delta_t2/MAXIMUM_DELTA_T2 );
		
		if(angle2 > PI/6.)  			// if the second angle is bigger than +30°
			direction = PI - angle1;   	// the direction = 90°-angle1
		else direction = angle1;	
	}
	else
	{
		delta_t2 = find_delta_t(0,2); 	// phase shift between right and rear microphone
		
		if (delta_t2 >= MAXIMUM_DELTA_T2)
			angle2 = PI * 0.5; 			// to avoid NAN of asin
		else if (delta_t2 <= -MAXIMUM_DELTA_T2)
			angle2 = -PI * 0.5; 		// to avoid NAN of asin
		else 
			angle2 = asin( (float)delta_t2/MAXIMUM_DELTA_T2 );
		
		if(angle2 < -PI/6.)				// if the second angle is smaller than -30°
			direction = PI - angle1;	// the direction = 90°-angle1
		else direction = angle1;
	}
	
	// We want an angle strictly between [0,2*PI]
	if (direction < 0)
		direction = 2*PI + direction;
	
	return direction;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		find delta t
//
// Finds the phase-shift between two signal.
// Basically, this function finds the maximum of the
// cross-correlation between two signals. 
//
// in:  int (microphone number of signal 1)
//      int (microphone number of signal 2)
// out: int (time expressed as number of samples taken)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int find_delta_t(int mic1_nb,int mic2_nb)
{
	int delta_t, tau, k; 
	long int correlation, max;
	//extern int mic_scan[3][MIC_SAMP_NB];
	
	int tau_min = -TAU_RANGE / 2;
	int tau_max = TAU_RANGE / 2;

	int save_sound_start = TAU_RANGE / 2 + 1;
	int save_sound_end = MIC_SAMP_NB - TAU_RANGE / 2 - 1;
	
	
	max = 0;
	
	for (tau = tau_min; tau < tau_max; tau++)
	{
		//reset the the correlation value
		correlation = 0;
	
	    // For each tau calculate the correlation between the two signals
	    for (k = save_sound_start; k < save_sound_end; k++)
	    {
		    correlation += (long int)(e_mic_scan[mic1_nb][k]) * (long int)(e_mic_scan[mic2_nb][k+tau] );
		}
		
		// find out if this correlation is the biggest one so far. --> If yes,
    	// save the value of tau --> This gives us the phaseshift between the
	    // signals   
        if (correlation > max)
        {
	        max = correlation;
	        delta_t = tau;
	    }

	}
	
	return delta_t;	
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		show led
//
// Lights up the LED in the appropriate direction
//
// in:  float (angle between 0 and 2PI clockwise)
// out: void
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void show_led(float angle)
{
	int led_nb = 6;
	long int i;
	
	// led_nb corresponds to the appropriate bit number in the LATA register
	if ( angle > (PI/8) )
		led_nb = 7;
	if ( angle > (3*PI/8) )
		led_nb = 9;
	if ( angle > (5*PI/8) )
		led_nb = 12;
	if ( angle > (7*PI/8) )
		led_nb = 10;
	if ( angle > (9*PI/8) )
		led_nb = 13;
	if ( angle > (11*PI/8) )
		led_nb = 14;
	if ( angle > (13*PI/8) )
		led_nb = 15;
	if ( angle > (15*PI/8) )
		led_nb = 6;
	
	// set the bit on PortA to illuminate the led
	LATA = 1 << led_nb;
	
	for (i=0;i<200000;i++);   // Wait to indicate the direction
	
	LATA = 0;	// turn all LEDs off
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		turn to direction
//
// Turns the robot to the appropriate direction
//
// in:  float (angle between 0 and 2PI clockwise)
// out: void
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void turn_to_direction(float direction)
{
	int end_turn;
		
		if (direction < PI)     // turn clockwise
		{
			e_set_steps_left(0);
			e_set_speed_left(TURN_SPEED);  // motor speed in steps/s
			e_set_speed_right(-TURN_SPEED);  // motor speed in steps/s
			
			// calculate how many steps the robot needs to turn
			end_turn = (int)(STEPS_FOR_2PI*(direction/(2*PI)));   
			while(e_get_steps_left() < end_turn);   // turn until done 
		}
		else 					// turn counterclockwise
		{
			e_set_steps_right(0);
			e_set_speed_left(-TURN_SPEED);  // motor speed in steps/s
			e_set_speed_right(TURN_SPEED);  // motor speed in steps/s
			
			// calculate how many steps the robot needs to turn
			end_turn = (int)(STEPS_FOR_2PI*((2*PI-direction)/(2*PI)));
			while(e_get_steps_right() < end_turn);   // turn until done
		}

		// stop motors
		e_set_speed_left(0);  // motor speed in steps/s
		e_set_speed_right(0);  // motor speed in steps/s
}

void run_locatesound() {
	int not_event;
	float direction;

	e_init_ad_scan(MICRO_ONLY);    			// initialize and start ad_conversion
    e_init_motors();
    e_start_agendas_processing();

	init_micro();

	while(1) {
		init_micro();
		not_event = 1;
		while (not_event)   	// do this loop until an event has occured
		{
			not_event = check_for_event();
		}
								// if there is an event then:
		record_sound();         // fill up the memory with the claping sound
					
		e_ad_scan_off();		// disable AD conversion (avoid interferes with the calculations)
					
		filter_signal();        // filters the signals
					
		direction = calculate_direction();   // do all the calculations where the sound is coming from
							
		e_ad_scan_on();			// enable AD conversion
							
		show_led(direction);    // indicate where the sound is coming from
							
		turn_to_direction(direction); // turn the robot to the direction the sound is coming from
	}								

}
