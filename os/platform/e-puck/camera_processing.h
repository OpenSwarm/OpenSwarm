/*!
 * \file
 * \ingroup camera
 * \ingroup shefpuck
 * \author  Yuri Kaszubowski Lopes <yurikazuba@gmail.com>
 * \version 1.0
 *
 * \brief External set of functions to assist the use of the camera. (provided by \ref shefpuck ) 
 * \date 2014
 */

/*! \defgroup shefpuck Shefpuck
 * \brief External set of functions to assist the programming of the e-Puck 
 * 
 * \author  Yuri Kaszubowski Lopes <yurikazuba@gmail.com>
 * 
 * This file is part of shefpuck.
 * 
 * This library is in development.
 *  
 * \section License
 * shefpuck is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *  
 * shefpuck is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License along with shefpuck.  If not, see <http://www.gnu.org/licenses/>.
 * Copyright (C) 2014-2015 Yuri Kaszubowski Lopes - yurikazuba@gmail.com
 * 
 * \note Due to the use of the e-puck library while processing the camera input, this module is used to process a camera frame into a virtual simple line of sight sensor value. This module, as well as the functions used from the e-puck library, will be replaced.
 * 
 */

void convertRGB565ToRGB888(unsigned char rgb565[], unsigned char rgb888[]);
void getRGB565at( char* buffer, unsigned char rgb585[], int x, int y );
void getRGB888at( char* buffer, unsigned char rgb888[], int x, int y );
char nearestNeighborRGB(unsigned char* rbg888, char flag);

char brushedColorFromRGB565(unsigned char rgb565[], char flag);
char getBrushedColorAt( char* buffer, char flag, int x, int y, int w );
