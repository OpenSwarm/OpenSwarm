/*
    This file is part of shefpuck.
    
    shefpuck is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    shefpuck is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
    
    You should have received a copy of the GNU Lesser General Public License
    along with shefpuck.  If not, see <http://www.gnu.org/licenses/>.
    
    Copyright (C) 2014-2015 Yuri Kaszubowski Lopes - yurikazuba@gmail.com
    
    --------------------------------------------------------------------
    
    This library is in development.
*/


void convertRGB565ToRGB888(unsigned char rgb565[], unsigned char rgb888[]);
void getRGB565at( char* buffer, unsigned char rgb585[], int x, int y );
void getRGB888at( char* buffer, unsigned char rgb888[], int x, int y );
char nearestNeighborRGB(unsigned char* rbg888, char flag);

char brushedColorFromRGB565(unsigned char rgb565[], char flag);
char getBrushedColorAt( char* buffer, char flag, int x, int y, int w );
