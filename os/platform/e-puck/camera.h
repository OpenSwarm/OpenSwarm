/*!
 * \file
 * \ingroup camera
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 27 August 2015
 * 
 * \brief  It declares functions to process data retrieved by a camera.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 */
/*! \defgroup camera Camera Module
 * \ingroup io
 * \ingroup epuck
 * \brief The camera module is used to retrieve raw camera data, process the incoming frames, and emits the result as \link events events\endlink.
 * 
 * 
 * This module currently is under development and is using functions of the e-puck library provided using Subversion at  svn://svn.gna.org/svn/e-puck/trunk (01/01/2016).
 * 
 * \todo The used functions from the e-puck library are very time and computational intensive. These function need to be rewritten to decrease the processing load.
 * 
 * \section camera_usage Usage
 * The camera is initialised and started by Sys_Init_Camera() and Sys_Start_Camera() respectively.
 * 
 * The camera uses a preprocessor to process a frame and generate the required events. This preprocessor can be defined by Sys_Set_Preprocessing(pCameraPreProcessor).
 * 
 * A received frame, if available (isNewFrameAvailable()) can be obtained with getFinishedFrame().
 * 
 * \note An e-puck cannot store a entire frame with full resolution (640x480). Consequently, only sub-frames can be taken.
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * \section camera_license License
 * LICENSE: adapted FreeBSD License (see http://openswarm.org/license)\n
 * Copyright (c) 2015, Stefan M. Trenkwalder\n
 * All rights reserved. 
 */


#ifndef SYSTEM_IO_CAMERA_H
#define	SYSTEM_IO_CAMERA_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../../definitions.h"

#define SYS_MAX_RED     0b00011111;/*!< red bits received */
#define SYS_MAX_GREEN   0b00111111;/*!< green bits received */
#define SYS_MAX_BLUE    0b00011111;/*!< blue bits received */

    /**
     * @brief This bitfield contains the structure of a received camera pixel.
     */
    typedef struct sys_rgb_pixel_s{
        uint8 red   : 5;
        uint8 green : 6;
        uint8 blue  : 5;
    }sys_rgb, sys_rgb_pixel;

    typedef void (*pCameraPreProcessor)(sys_rgb_pixel **frame, uint16 width, uint16 height);/*!< pointer to a camera preprocessor */

    void Sys_Init_Camera(void);
    void Sys_Start_Camera(void);

    void Sys_Set_Preprocessing(pCameraPreProcessor func);

    sys_rgb_pixel *getFinishedFrame();
    bool isNewFrameAvailable();

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_CAMERA_H */

