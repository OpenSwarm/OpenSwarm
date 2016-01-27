/*!
 * \file
 * \ingroup camera
 * \ingroup shefpuck
 * \author  Yuri Kaszubowski Lopes <yurikazuba@gmail.com>
 * \version 1.0
 *
 * \date 2014
 */

#include "camera_processing.h"
#include <math.h>
#include <stdio.h>

void convertRGB565ToRGB888(unsigned char rgb565[], unsigned char rgb888[]){
    rgb888[0] = (rgb565[0] & 0xF8);
    rgb888[1] = (((rgb565[0] & 0x07) << 5) | ((rgb565[1] & 0xE0) >> 3));
    rgb888[2] = ((rgb565[1] & 0x1F) << 3);
}

void getRGB565at( char* buffer, unsigned char rgb585[], int x, int y ){
    rgb585[0] = buffer[40*y + 2*x];
    rgb585[1] = buffer[40*y + 2*x + 1];
}

void getRGB888at( char* buffer, unsigned char rgb888[], int x, int y ){
    unsigned char rgb565[2];
    getRGB565at( buffer, rgb565, x, y );
    convertRGB565ToRGB888( rgb565, rgb888 );
}

#define CP_WI 100
#define CP_WGB_I 80
#define CP_RI 80
#define CP_GI 40
#define CP_BI 100
const unsigned char colorPositions[8][4] = {
    { 0    , 0    , 0    , 'd' },
    { 0    , CP_GI, CP_BI, 'c' },
    { CP_RI, 0    , CP_BI, 'm' },
    { CP_RI, CP_GI, 0    , 'y' },
    { 0    , 0    , CP_BI, 'b' },
    { 0    , CP_GI, 0    , 'g' },
    { CP_RI, 0    , 0    , 'r' },
    { CP_WI, CP_WGB_I, CP_WGB_I, 'w' }
    
};

//FLAG 0wrgbymcd
char nearestNeighborRGB(unsigned char* rbg888, char flag){
    int current, valueNear = 766;
    char color = 'd';
    int i;
    for(i=0;i<8;i++){
        if( flag & (0x01 << i) ){
            current = fabs( (int) rbg888[0] - colorPositions[i][0] ) + fabs( (int) rbg888[1] - colorPositions[i][1] ) + fabs( (int) rbg888[2] - colorPositions[i][2] );
            if( current < valueNear ){
                valueNear = current;
                color     = colorPositions[i][3];
            }
        }
    }
    return color;
}

/******************************** BRUSHED BITS ********************************/

const int powerTbl[33] = {0,1,4,9,16,25,36,49,64,81,100,121,144,169,196,225,256,289,324,361,400,441,484,529,576,625,676,729,784,841,900,961,1024};
#define CBP_WI 16
#define CBP_RI 11
#define CBP_GI 11
#define CBP_BI 13
#define CBP_DI 2
const unsigned char colorBrushedPositions[8][4] = {
    { CBP_DI, CBP_DI, CBP_DI, 'd' },
    { CBP_DI, CBP_GI, CBP_BI, 'c' },
    { CBP_RI, CBP_DI, CBP_BI, 'm' },
    { CBP_RI, CBP_GI, CBP_DI, 'y' },
    { CBP_DI, CBP_DI, CBP_BI, 'b' },
    { CBP_DI, CBP_GI, CBP_DI, 'g' },
    { CBP_RI, CBP_DI, CBP_DI, 'r' },
    { CBP_WI, CBP_WI, CBP_WI, 'w' }
    
};

char brushedColorFromRGB565(unsigned char rgb565[], char flag){
    int i, current, valueNear = 3072;
    char r,g,b,color = 'd';

    r = (rgb565[0] & 0xF8)>>3; //0b11111000
    g = (((rgb565[0] & 0x07) << 2) | ((rgb565[1] & 0xC0) >> 6)); //0b00000111 | 0b11000000
    b = (rgb565[1] & 0x1F); //0b00011111
    
    for(i=0;i<8;i++){
        if( flag & (0x01 << i) ){
            current =
                powerTbl[ (int) fabs( (int) (r - colorBrushedPositions[i][0]) ) ] +
                powerTbl[ (int) fabs( (int) (g - colorBrushedPositions[i][1]) ) ] +
                powerTbl[ (int) fabs( (int) (b - colorBrushedPositions[i][2]) ) ];
            if( current < valueNear ){
                valueNear = current;
                color     = colorBrushedPositions[i][3];
            }
        }
    }
    return color;
}

char getBrushedColorAt( char* buffer, char flag, int x, int y, int w ){
    unsigned char rgb565[2];
    rgb565[0] = buffer[2*w*y + 2*x];
    rgb565[1] = buffer[2*w*y + 2*x + 1];
    return brushedColorFromRGB565( rgb565, flag );
}

/*
//FLAG 0wrgbymcd
char nearestNeighborRGB(char* rbg888, char flag){
    long int current, valueNear = 195075;
    char color = 'd';
    int i;
    for(i=0;i<8;i++){
        if( flag & (0x01 << i) ){
            current = pow( (unsigned long int) fabs( rbg888[0] - colorPositions[i][0] ), 2 ) +
                      pow( (unsigned long int) fabs( rbg888[1] - colorPositions[i][1] ), 2 ) +
                      pow( (unsigned long int) fabs( rbg888[2] - colorPositions[i][2] ), 2 );
            if( current < valueNear ){
                valueNear = current;
                color     = colorPositions[i][3];
            }
        }
    }
    return color;
}
*/

/*
int _color_detection_light;

void initCameraProcessing(){
    _color_detection_light = 40;
}

int isColorRed(char* rgb888){
    int DR = rgb888[0]*13;
    int DG = rgb888[1]*19;
    int DB = rgb888[2]*17;
    return (rgb888[0]>=_color_detection_light) && (DR>=DG) && (DR>=DB);
}

int isColorYellow(char* rgb888){
    int DR  = rgb888[0]*13;
    int DG  = rgb888[1]*12;
    int DBr = rgb888[2]*17;
    int DBg = rgb888[2]*14;
    int DRG = fabs(rgb888[0] - rgb888[1]);
    return (rgb888[0]>=_color_detection_light) && (rgb888[1]>=_color_detection_light) && (DR>=DBr) && (DG>=DBg) && DRG <= 20;
}

int isColorGreen(char* rgb888){
    int DR = rgb888[0]*16;
    int DG = rgb888[1]*12;
    int DB = rgb888[2]*14;
    return (rgb888[1]>=_color_detection_light) && (DG>=DR) && (DG>=DB);
}

int isColorBlue(char* rgb888){
    int DR = (int)rgb888[2] - rgb888[0];
    int DG = (int)rgb888[2] - rgb888[1];
    return (RGB[2]>=_color_detection_light) && (DR>=24) && (DG>=8);
}

int isColorDark(char* rgb888){
    return (rgb888[0]<_color_detection_light) && (rgb888[1]<_color_detection_light) && (rgb888[2]<_color_detection_light);
}
*/

