/**************************************************************************
* 			Programm to follow balls using the camera				      *
*			Version 2.0 août 2007				                          *
*			Jonathan Besuchet, Alain Balleret				              *
*									                                      *
**************************************************************************/

/*! \file
 * \brief Following balls using the camera
 * \section sect_follow1 Introduction
 * This demo show you how you can drive the camera to make vision based
 * algorithmes.
 * This programm uses the camera to follow balls. There are three differents
 * configurations:
 * - Following the green balls: put the selector on position 6. Now the e-puck
 * will follow the first green ball he saw indifferently of the color.
 * - Following the red balls: put the selector on position 7. Now the e-puck
 * will follow the first red ball he saw indifferently of the color.
 * - Following all the balls: put the selector on position 8. Now the e-puck
 * will follow the first ball he saw indifferently of the color.
 * \image html cam.gif
 *
 * \section sect_follow2 How it works
 * The goal of this programm is to search a ball, when one is found the e-puck
 * has to go in it direction and has to stop at a specified distance. This is
 * done by the following steps:
 * - First of all we have to capture an image. The image is 4 pixels height and
 * 480 pixels width. We take one pixel each for pixels then the image is finally
 * 1 pixel height and 120 pixels width => stored in array 1x120.
 * - We normalize the array to 10 (the mean value of the array is now ten).
 * - We search a pic in the array which correspond of a ball (look at the
 * picture below to see how is the values when a ball is detected).
 * - We work with the motors to do two thinks at the same time: 1) to put this
 * pic on the center of the array => it would place the e-puck across the ball;
 * 2) to adjust the thickness of this pic at the desired value => it would place
 * the e-puck on the good distance of the ball. To have the best control we use
 * a PI (proportionnal intergral) regulator.
 * \subsection pic The graphe of the array when a ball is detected
 * \image html pic.gif
 * \subsection subsect1 Follow the good color
 * When we want to follow all the balls, the camera is used in gray scale mode.
 * \n When we want to follow the green ball for exemple, the camera is used in
 * color mode. In color mode, we have three values: 1 for the red, 1 one
 * for the green, 1 for the blue. Then to follow the green ball we only have to
 * take the red constituent, because the value of green in the red constituent
 * is near zero => it makes a pic in the array.
 * \n When we want to follow the red ball, it's the same principle: we take the
 * green constituent,...
 *
 * \section sect_follow3 Playing the demo
 * The programm works with the contrast of the color between the ball and the 
 * environement, so to have the best result you should place the e-puck in an
 * arena which has white wall and good luminosity.
 * \n To play the demo select the configuration you want (follow all, green, 
 * red balls) and let's go. The e-puck will turn on himself until he finds a
 * ball.
 *
 * \section sect_follow4 Video of the demo
 * - Following all the balls: http://www.youtube.com/watch?v=Pga71leqf1A
 * - Following the green balls: http://www.youtube.com/watch?v=FGXBy9FOnmw
 *
 * \author Jonathan Besuchet
 */

#ifndef _FOLLOW_BALL
#define _FOLLOW_BALL

void run_follow_ball(void);
void run_follow_ball_green(void);
void run_follow_ball_red(void);

#endif
