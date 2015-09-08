/*
 * File:   system_IO_camera.c
 * Author: Stefan trenkwalder
 *
 * Created on 27 August 2015, 16:33
 *
 * LICENCE: adapted FreeBSD Licence
 * Copyright (c) 2015, Stefan Trenkwalder
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. If this or parts of this source code or binary code in any form is used for an commercial product or service in any form, this product or service must provide a clear notice/message readable for any user or customer that this product or service uses OpenSwarm.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "system_IO_camera.h"


#include <p30F6014A.h>
#include <stdlib.h>
#include <string.h>

#include "HDI_epuck_ports.h"

#include "system_Events.h"
#include "system_IO.h"
//#include "system_IO_i2c.h"
#include "system_Interrupts.h"
#include "system_Memory.h"

#include "e_poxxxx.h"
#include "e_po6030k.h"

#define FRAME_WIDTH     10
#define FRAME_HEIGHT    10
#define CAMERA_I2C_ADDRESS 0xDC

#define RED_THRESHOLD   0b00010011
#define GREEN_THRESHOLD 0b00011111
#define BLUE_THRESHOLD  0b00001011

static pCameraPreProcessor pre_processor = 0;

static sys_rgb_pixel *frame_a = 0;
static sys_rgb_pixel *frame_b = 0;
static uint16 max_frame_size = 0;

static sys_rgb_pixel *current_frame = 0;
static uint16 current_row = 0;
static uint16 current_col = 0;
static uint16 current_pos = 0;

static bool is_newframe_available = false;

inline void Sys_Process_newPixel(void);
inline void Sys_Process_newLine(void);
inline void Sys_Process_newFrame(void);

void Sys_Camera_PreProcessor(void);

inline void Sys_Write_to_Camera(uint8 address, uint8* data, uint16 length){
    uint8 *i2c_data = (uint8 *) Sys_Malloc(length+1);

    i2c_data[0] = address;

    Sys_Memcpy(data, i2c_data+1,length);

//    Sys_I2C_SentBytes(CAMERA_I2C_ADDRESS, i2c_data, length+1);
}


static unsigned char buffer[10*10*2] = {0};
void Sys_Init_Camera(){


#define CAM_WIDTH 160
#define CAM_HEIGHT 160
#define CAM_ZOOM_X 4
#define CAM_ZOOM_Y 4

e_poxxxx_init_cam ();
e_poxxxx_config_cam(300, 220, 40, 40, 4, 4, RGB_565_MODE);
e_po6030k_write_register(BANK_C, 0x04, 0b10011110);
e_po6030k_write_register(BANK_C, 0x55, 0x00);
e_po6030k_write_register(BANK_C, 0x56, 0x00);
e_po6030k_write_register(BANK_C, 0x28, 0x10);
e_po6030k_write_register(BANK_C, 0x29, 0x00);

e_poxxxx_launch_capture(&buffer[0]); //take first image
    if(!Sys_Register_IOHandler(Sys_Camera_PreProcessor)){
        return;
    }
    
    /*

    Sys_Init_IOManagement();

    if(!Sys_Register_Event(SYS_EVENT_IO_CAMERA)){
        return;
    }
    

    //CAM_VSYNC_DIR = INPUT_PIN;
    //CAM_HREF_DIR = INPUT_PIN;
    //CAM_PCLK_DIR = INPUT_PIN;

    //Sys_Init_I2C();
    //Sys_Start_I2C();
	e_i2cp_init();

    CAM_RESET=0;

    if(frame_a != 0){
        Sys_Free(frame_a);
    }

    max_frame_size = sizeof(sys_rgb_pixel) * FRAME_HEIGHT * FRAME_WIDTH;
    frame_a = (sys_rgb_pixel *) Sys_Malloc(max_frame_size);
    if(frame_a == 0){ //no memory
        return;
    }


    uint16 c = 0;
    uint16 r = 0;
    for(r = 0; r < FRAME_HEIGHT; r++){
        for(c = 0; c < FRAME_WIDTH; c++){
            frame_a[r*FRAME_WIDTH+c].red = 0;
            frame_a[r*FRAME_WIDTH+c].green = 0;
            frame_a[r*FRAME_WIDTH+c].blue = 0;
        }
    }
    //frame_a is initialised

    current_frame = frame_a;

    if(frame_b != 0){
        Sys_Free(frame_b);
    }

    CAM_RESET=1;

    frame_b = (sys_rgb_pixel *) Sys_Malloc(sizeof(sys_rgb_pixel) * FRAME_HEIGHT * FRAME_WIDTH);
    if(frame_b == 0){ //no memory
        return;
    }

    for(r = 0; r < FRAME_HEIGHT; r++){
        for(c = 0; c < FRAME_WIDTH; c++){
            frame_b[r*FRAME_WIDTH+c].red = 0;
            frame_b[r*FRAME_WIDTH+c].green = 0;
            frame_b[r*FRAME_WIDTH+c].blue = 0;
        }
    }
    //frame_b is initialised
    current_pos = 0;

#define BANK_A 0x0
#define BANK_B 0x1
#define BANK_C 0x2
#define BANK_D 0x3
    
	e_i2cp_enable();
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x03, BANK_B);
	
    e_i2cp_write(CAMERA_I2C_ADDRESS, 0x50, 0x00);//Set start position of the frame
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x51, 0x4E);
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x52, 0x00);
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x53, 0x39);
    
    e_i2cp_write(CAMERA_I2C_ADDRESS, 0x54, 0x00);//Set stop position of the frame
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x55, 0x58);
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x56, 0x00);
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x57, 0x43);
    
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x60, 0x00);//set Vsync
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x61, 0xE4);
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x62, 0x01);
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x63, 0x0C);
    
    e_i2cp_write(CAMERA_I2C_ADDRESS, 0x03, BANK_A);
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x91, 0x50);//set speed to QQVGA
    
    
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x03, BANK_B);
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x38, 0x08);//colour RGB 565
    
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x80, 0x80);//SCALE X & Y to QQVGA
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x81, 0x80);
	e_i2cp_write(CAMERA_I2C_ADDRESS, 0x82, 0x01);
    
    ////*
    uint8 byte = 0x01;
    Sys_Write_to_Camera(0x03,&byte,1);//set regesters to group B

    byte = 0x08;
    Sys_Write_to_Camera(0x38,&byte,1);//set data to R5G6B5
    byte = 0x07;
    Sys_Write_to_Camera(0x68,&byte,1);//set to QQVGA (160x120)

    //set frame to central 10x10 -> frame is from (75+3,55+2) -> (85+2,65+1) = (0x4E,0x39) -> (0x57, 0x42)
    uint8 values1[] = {0x00, 0x4E, 0x00, 0x39, 0x00, 0x57, 0x00, 0x42};
    Sys_Write_to_Camera(0x50,values1,8);

    byte = 0x01;
    Sys_Write_to_Camera(0x82,&byte,1);//in example only states as reserved

    uint8 values2[] = {0x80, 0x80};
    Sys_Write_to_Camera(0x80,values2,2);//scaling is 128

    //set framelimits (width)
    uint8 values3[] = {0x00, 0x6B};
    Sys_Write_to_Camera(0x48,values3,2);
    //set framelimits (height)
    uint8 values4[] = {0x00, 0x46};
    Sys_Write_to_Camera(0x29,values4,2);
    //Frame Rate = freq (PCLK) / ( ( FrameHeight +1 ) X ( FrameWidth+1 ) )

    //set the values back to Group A
    byte = 0x00;
    Sys_Write_to_Camera(0x03,&byte,1);//set data to R5G6B5


    IEC0bits.T1IE = 0;// enable pixel interrupt
    IEC1bits.T4IE = 0;// enable line interrupt
    IEC1bits.T5IE = 0;// enable frame interrupt

    IFS0bits.T1IF = 0;
    IFS1bits.T4IF = 0;
    IFS1bits.T5IF = 0;

    IPC0bits.T1IP = SYS_IRQP_CAMERA_PIXEL;//Interrupt Priority
    IPC5bits.T4IP = SYS_IRQP_CAMERA_LINE;//Interrupt Priority
    IPC5bits.T5IP = SYS_IRQP_CAMERA_FRAME;//Interrupt Priority

    TMR1 = 0;
    PR1 = 1;

    T1CONbits.TCKPS = 0b00; //prescaler 1
    T1CONbits.TGATE = 0;
    T1CONbits.TCS = 1; //enable external clock from T1CK pin
    T1CONbits.TSYNC = 1;//use syncronous counter with rising edge of T1CK
    T1CONbits.TON = 0;

    TMR4 = 0;
    PR4 = 1;
    T4CONbits.TCKPS = 0b00; //prescaler 1
    T4CONbits.TGATE = 0;
    T4CONbits.TCS = 1; //enable external clock from T1CK pin
    T4CONbits.TON = 0;

    TMR5 = 0;
    PR5 = 1;
    T5CONbits.TCKPS = 0b00; //prescaler 1
    T5CONbits.TGATE = 0;
    T5CONbits.TCS = 1; //enable external clock from T1CK pin
    T5CONbits.TON = 1;
*/
}

void Sys_Start_Camera(){
/*
    IFS0bits.T1IF = 0;
    IFS1bits.T4IF = 0;
    IFS1bits.T5IF = 0;

    IEC0bits.T1IE = 1;// enable pixel interrupt
    IEC1bits.T4IE = 1;// enable line interrupt
    IEC1bits.T5IE = 1;// enable frame interrupt
*/
}

void Sys_Set_Preprocessing(pCameraPreProcessor func){
    pre_processor = func;
}

/*

void __attribute__((interrupt,auto_psv)) _T1Interrupt(void){

    //PLL  = 8  instructions per MCLK
    //QQVGA used -> 4 MCKL/Pixel
    // < 32 instructions
    __asm__ volatile ("bclr     _IFS0bits, #3");//copy pixel values into w2
    __asm__ volatile ("clr      _TMR4       ");
    __asm__ volatile ("push.s               ");//protect w0-4

    __asm__ volatile ("mov    _PORTC, w0  ");
    __asm__ volatile ("btst     w0,#3");// check CAM_HREF
    __asm__ volatile ("bra      Z,FRAME_END");
    __asm__ volatile ("btst     w0,#4");// check CAM_VSYNC
    __asm__ volatile ("bra      Z,FRAME_END");                          //1 MCLK

    __asm__ volatile ("mov    _PORTD, w4  ");//save pixel values in W4
    __asm__ volatile ("mov  _current_frame,w1");
    __asm__ volatile ("mov  #_current_pos, w2");
    __asm__ volatile ("add  w1, w2, w0      ");//W0 = W1 + W2 -> points to the next pixel part
    __asm__ volatile ("mov  w4, [w0]        ");
    __asm__ volatile ("inc  _current_pos     ");//current_pos++

    __asm__ volatile ("mov  #_current_pos,    w2");//
    __asm__ volatile ("mov  #_max_frame_size, w1");//                   //2 MCLK
    __asm__ volatile ("cp   w1, w2");//
    __asm__ volatile ("bra  N, T1_END");//

__asm__ volatile ("FRAME_END:");//retfie = 3 cycles
    __asm__ volatile ("call  _Sys_Finished_Frame");//

__asm__ volatile ("T1_END:");
    __asm__ volatile ("pop.s                ");//retfie = 3 cycles

    if(IFS1bits.T4IF == 0){
        LED7 =1;
    }
}
void __attribute__((interrupt,auto_psv)) _AltT1Interrupt(void){
    Sys_Process_newPixel();
}


void __attribute__((interrupt,auto_psv)) _T4Interrupt(void){
    Sys_Process_newLine();
    
    TMR4 = 0;
    IFS1bits.T4IF = 0;
}
void __attribute__((interrupt,auto_psv)) _AltT4Interrupt(void){
    Sys_Process_newLine();
    
    TMR4 = 0;
    IFS1bits.T4IF = 0;
}

void __attribute__((interrupt,auto_psv)) _T5Interrupt(void){
    Sys_Process_newFrame();
    
    TMR5 = 0;
    IFS1bits.T5IF = 0;
}
void __attribute__((interrupt,auto_psv)) _AltT5Interrupt(void){
    Sys_Process_newFrame();
    
    TMR5 = 0;
    IFS1bits.T5IF = 0;
}

*/

inline void Sys_Finished_Frame(){
        T4CONbits.TON = 0;
        T1CONbits.TON = 0;
        if(current_frame == frame_a){
            current_frame = frame_b;
        }else{
            current_frame = frame_a;
        }

        LED1 = ~LED1;

        is_newframe_available = true;
}

inline void Sys_Process_newPixel(){
//#define CAM_PWDN _RC2
//#define CAM_VSYNC _RC4
//#define CAM_HREF _RC3
//#define CAM_PCLK _RC14

    //PLL  = 8  instructions per MCLK
    //QQVGA used -> 4 MCKL/Pixel
    // < 32 instructions
 /* __asm__ volatile ("bclr     _IFS0bits, #3");//copy pixel values into w2
    __asm__ volatile ("clr      _TMR4       ");
    __asm__ volatile ("push.s               ");//protect w0-4

    __asm__ volatile ("mov    _PORTC, w0  ");
    __asm__ volatile ("btst     w0,#3");// check CAM_HREF
    __asm__ volatile ("bra      Z,FRAME_END");
    __asm__ volatile ("btst     w0,#4");// check CAM_VSYNC
    __asm__ volatile ("bra      Z,FRAME_END");                          //1 MCLK

    __asm__ volatile ("mov    _PORTD, w4  ");//save pixel values in W4
    __asm__ volatile ("mov  _current_frame,w1");
    __asm__ volatile ("mov  #_current_pos, w2");
    __asm__ volatile ("add  w1, w2, w0      ");//W0 = W1 + W2 -> points to the next pixel part
    __asm__ volatile ("mov  w4, [w0]        ");
    __asm__ volatile ("inc  _current_pos     ");//current_pos++

    __asm__ volatile ("mov  #_current_pos,    w2");//
    __asm__ volatile ("mov  #_max_frame_size, w1");//                   //2 MCLK
    __asm__ volatile ("cp   w2, w1");//
    __asm__ volatile ("bra  Z, T1_END");//

__asm__ volatile ("FRAME_END:");//retfie = 3 cycles
    __asm__ volatile ("call  _Sys_Finished_Frame");//

__asm__ volatile ("T1_END:");
    __asm__ volatile ("pop.s                ");//retfie = 3 cycles
*/  
    /*
    if( CAM_VSYNC != 1){
        Sys_Finished_Frame();
        return;
    }

    if(CAM_HREF != 1){
        T1CONbits.TON = 0;
        return;
    }
    
    uint8 buffer = CAM_DATA;
    current_frame[current_pos].red = buffer >> 3;
    current_frame[current_pos].green = (buffer << 3) & 0b00111000;


    buffer = CAM_DATA;
    current_frame[current_pos].green |= (buffer >> 5);
    current_frame[current_pos].blue = (buffer & 0b00011111);
    //LED5 = CAM_PCLK_DIR;

    
    if( current_pos >= FRAME_WIDTH*FRAME_HEIGHT){
        Sys_Finished_Frame();
        return;
    }
    
    if(current_col >= FRAME_WIDTH){
        if(current_row >= FRAME_HEIGHT){
            Sys_Finished_Frame();
            return;            
        }
        
        T1CONbits.TON = 0;
        current_row++;
        current_col = 0;
        return;
    }

    if(buffer == 0){
        buffer = CAM_DATA;
        current_frame[current_row][current_col].red = buffer >> 3;
        current_frame[current_row][current_col].green = (buffer << 3) & 0b00111000;
        return;
    }else{
        buffer = CAM_DATA;
        current_frame[current_row][current_col].green |= (buffer >> 5);
        current_frame[current_row][current_col].blue = (buffer & 0b00011111);

        buffer = 0;
        current_col++;
    }*/

}

inline void Sys_Process_newLine(){

    if( CAM_VSYNC != 1){
        Sys_Finished_Frame();
        return;
    }
    T1CONbits.TON = 1;//enable new Pixel interrupt -> minimise IRQ overhead
    TMR4 = 0;

}

inline void Sys_Process_newFrame(){
    T4CONbits.TON = 1;//enable new Line interrupt -> minimise IRQ overhead
    TMR5 = 0;
}


sys_rgb_pixel *getFinishedFrame(){
    is_newframe_available = false;

    if(current_frame != frame_a){
        return frame_b;
    }

    return frame_a;
}
bool isNewFrameAvailable(){
    return is_newframe_available;
}


void Sys_Camera_PreProcessor(void){
    if(!e_poxxxx_is_img_ready()){
        return;
    }

    
    uint16 i = 0;
    sys_rgb_pixel *frame = (sys_rgb_pixel *) buffer;

    uint16 red_counter = 0;
    uint16 green_counter = 0;
    uint16 blue_counter = 0;

    for(i = 0; i < 100; i++){
            if(frame[i].red > RED_THRESHOLD){
                red_counter++;
            }
            if(frame[i].green > GREEN_THRESHOLD){
                green_counter++;
            }
            if(frame[i].blue > BLUE_THRESHOLD){
                blue_counter++;
            }
    }

    uint8 colour = 0;

    if(red_counter > 35){
        colour |= RED;
        LED1 = 1;
    }else{
        LED1 = 0;
    }
    if(green_counter > 35){
        colour |= GREEN;
        LED2 = 1;
    }else{
        LED2 = 0;
    }
    if(blue_counter > 35){
        colour |= BLUE;
        LED3 = 1;
    }else{
        LED3 = 0;
    }

    e_poxxxx_launch_capture(buffer);
    //Sys_Send_Event(SYS_EVENT_IO_CAMERA, &colour, 1);
}