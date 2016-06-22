/*!
 * \file
 * \ingroup camera
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \author  Yuri Kaszubowski Lopes <yurikazuba@gmail.com>
 * 
 * \version 1.0
 *
 * \date 27 August 2015
 * 
 * \brief  It defines functions to process data retrieved by a camera.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 * \todo The used functions from the e-puck library are very time and computational intensive. These function can be rewritten to decrease the processing load.
 */

#include "camera.h"
#include "../../io/io.h"

#include "uart.h"

#include "../../definitions.h"

#include "../../events/events.h"

#include "../../interrupts.h"
#include "../../memory.h"

#ifdef EPUCK_USED 
#include "camera_processing.h"
#include "library/camera/fast_2_timer/e_poxxxx.h"
#include "library/camera/fast_2_timer/e_po6030k.h"
#endif

#define FRAME_WIDTH     10      /*!< Width of the subframe of the image */
#define FRAME_HEIGHT    10      /*!< Height of the subframe of the image */
#define CAMERA_I2C_ADDRESS 0xDC /*!< I2C address of the camera*/

#define RED_MAX         0x0C1C  /*!< maximum value for received red */
#define GREEN_MAX       0x189C  /*!< maximum value for received green */
#define BLUE_MAX        0x0C1C  /*!< maximum value for received blue */
#define RED_THRESHOLD   0x060E  /*!< threshold value for received red */
#define GREEN_THRESHOLD 0x0E4E  /*!< threshold value for received green */
#define BLUE_THRESHOLD  0x060E  /*!< threshold value for received blue */

static pCameraPreProcessor pre_processor = 0; /*!< callback function to processes the raw camera image*/
int flashDetected = false;
int lightsOn = true;

//static sys_rgb_pixel *frame_a = 0;/*!< pointer to a received frame */
//static sys_rgb_pixel *frame_b = 0;/*!< pointer to a second received frame */
//static sys_rgb_pixel *current_frame = 0; /*!< pointer to the current frame that needs to be finished */
//static bool is_newframe_available = false; /*!< flag to indicate that a new frame was received*/


inline void Sys_Process_newPixel(void);
inline void Sys_Process_newLine(void);
inline void Sys_Process_newFrame(void);

void Sys_Camera_PreProcessor(void);

/*
inline void Sys_Write_to_Camera(uint8 address, uint8* data, uint16 length){
    uint8 *i2c_data = (uint8 *) Sys_Malloc(length+1);

    i2c_data[0] = address;

    Sys_Memcpy(data, i2c_data+1,length);

//    Sys_I2C_SentBytes(CAMERA_I2C_ADDRESS, i2c_data, length+1);
}
 */

#define CAM_WIDTH 160   /*!< width of the camera input frame */
#define CAM_HEIGHT 160  /*!< height of the camera input frame */
#define CAM_ZOOM_X 8    /*!< zoom factor to scale the frame */
#define CAM_ZOOM_Y 8    /*!< zoom factor to scale the frame */
#define CAM_W_SIZE 20   /*!< post scale width frame */
#define CAM_H_SIZE 20   /*!< post scale height frame */
static char buffer[CAM_W_SIZE*CAM_H_SIZE*2];  /*!< received frame */

/**
 *
 * This function initialises the camera using e-puck library from Subversion at svn://svn.gna.org/svn/e-puck/trunk
 *
 * @todo rewrite the camera to computational less intensive functions
 * 
 */
void Sys_Init_Camera(){

    if(!Sys_Register_IOHandler(Sys_Camera_PreProcessor)){
        return;
    }
    if(!Sys_Register_Event(SYS_EVENT_IO_CAMERA)){
        return;
    }

    e_poxxxx_init_cam();
    e_poxxxx_config_cam((ARRAY_WIDTH -CAM_WIDTH)/2,(ARRAY_HEIGHT-CAM_HEIGHT)/2,CAM_WIDTH,CAM_HEIGHT,CAM_ZOOM_X,CAM_ZOOM_Y,RGB_565_MODE);
    e_po6030k_write_register(BANK_C, 0x04, 0b10011110);
    e_po6030k_set_mirror(1,1);
    e_poxxxx_write_cam_registers();
}

/**
 *
 * This function starts the capturing using e-puck library from Subversion at svn://svn.gna.org/svn/e-puck/trunk
 *
 * @todo rewrite the camera to computational less intensive functions
 * 
 */
void Sys_Start_Camera(){
/*
    IFS0bits.T1IF = 0;
    IFS1bits.T4IF = 0;
    IFS1bits.T5IF = 0;

    IEC0bits.T1IE = 1;// enable pixel interrupt
    IEC1bits.T4IE = 1;// enable line interrupt
    IEC1bits.T5IE = 1;// enable frame interrupt
*/
    e_poxxxx_launch_capture( &buffer[0]); //take first image
}

/**
 *
 * Defines a preprocessor callback functions to process the frame.
 *
 * 
 * @param[in] func camera preprocessor which computes events out of the raw image
 */
void Sys_Set_Preprocessing(pCameraPreProcessor func){
    pre_processor = func;
}

#define CP_WI 120               /*!< whitness factor to process and calibrate the camera */
#define CP_RI 80                /*!< red factor to process and calibrate the camera */
#define CP_GI 80                /*!< green factor to process and calibrate the camera  */
#define CP_BI 100               /*!< blue factor to process and calibrate the camera  */
#define COLOUR_THRESHOLD 766    /*!< threshold to decide if a colour pixel has been measured */


/**
 *
 * This function processes an incoming camera frame and emits events according to used algorithm. 
 *
 * @todo rewrite the camera to computational less intensive functions
 * 
 */
void Sys_Camera_PreProcessor(void){
    if(!e_poxxxx_is_img_ready()){//is a new frame ready
        return;
    }
 
    unsigned char rgb565[2], rgb888[3];
    int r,c;
    int WeightOfRed = 0, countDark = 0, countLight = 0;
    
    for(r=9;r<10;r++){
        for(c=0;c<CAM_W_SIZE;c++){
            Sys_Start_AtomicSection();
            getRGB565at(buffer, rgb565, r, c);
            convertRGB565ToRGB888( rgb565, rgb888  );
            Sys_End_AtomicSection();
    
            if (lightsOn == false) {
                char colour = nearestNeighborRGB( rgb888, 0b11110001 ); //flag: WRGbymcD

                switch(colour){
                    case 'r':
                        WeightOfRed++; 
                        break;
                    case 'w':
                    case 'g':
                    case 'b':
                    case 'y':
                    case 'c':
                    case 'm':
                    case 'd':
                    default:
                        break;
                }
            }
            if (rgb888[0] < 30 && rgb888[1] < 30 && rgb888[2] < 30 ) countDark++;
        }
    }
    
    if (WeightOfRed >= 2) flashDetected = true;
    else flashDetected = false;
    
    if (countDark > CAM_W_SIZE/2) lightsOn = false;
    else lightsOn = true;
    
    e_poxxxx_launch_capture((char *) buffer);
}

int seeFlash() {
    int out = flashDetected;
    flashDetected = false;
    return out;
}

int lightSwitch() {
    return !lightsOn;
}