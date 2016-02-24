/*!
 * \file
 * \ingroup selector
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 19 February 2016
 * 
 * \brief  It declares functions to obtain the selected value of the selector.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 */
/*! \defgroup selector Selector Module
 * \ingroup io
 * \ingroup epuck
 * \brief The selector is a simple input device that can be set by a human to a value between 0 and 15. This value is then sent as an \link events event\endlink.
 * 
 * 
 * This module reads the values of the selector and creates an SYS_EVENT_IO_SELECTOR event if the value has changed.
 * 
 * \section selector_usage Usage
 * The selector is initialised and started by Sys_Init_Selector() and Sys_Start_Selector() respectively.
 * 
 * The selector uses a preprocessor to process identify a change of value and generate the required events. This preprocessor is defined by Sys_Selector_PreProcessor.
 * 
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * \section camera_license License
 * LICENSE: adapted FreeBSD License (see http://openswarm.org/license)\n
 * Copyright (c) 2015, Stefan M. Trenkwalder\n
 * All rights reserved. 
 */

#ifndef SELECTOR_H
#define	SELECTOR_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    void Sys_Init_Selector(void);
    void Sys_Start_Selector(void);
    
    uint8 Sys_Get_Selector(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SELECTOR_H */

