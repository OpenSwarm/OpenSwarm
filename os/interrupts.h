/*!
 * \file
 * \ingroup base
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date{03 September 2015}
 * 
 * \brief It declares interrupt priority levels and functions to create atomic sections.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#ifndef SYSTEM_INTERRUPTS_H
#define	SYSTEM_INTERRUPTS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SYS_IRQP_MAX 7              /*!< maximum interrupt priority*/

#define SYS_IRQP_SYSTEM_TIMER 2	 	/*!< interrupt priority for the system timer interrupt*/

#define SYS_IRQP_IO_TIMER 3         /*!< interrupt priority for the I/O timer interrupt*/

#define SYS_IRQP_UART1 4            /*!< interrupt priority for the UART1 interrupt*/
#define SYS_IRQP_UART2 4            /*!< interrupt priority for the UART2 interrupt*/

#define SYS_IRQP_I2C 5              /*!< interrupt priority for the I2C interrupt*/

#define SYS_IRQP_REMOTECONTROL 4    /*!< interrupt priority for the remote control interrupt*/

#define SYS_IRQP_CAMERA_PIXEL 5     /*!< interrupt priority for the camera pixel interrupt*/
#define SYS_IRQP_CAMERA_LINE  6     /*!< interrupt priority for the camera line  interrupt*/
#define SYS_IRQP_CAMERA_FRAME 7     /*!< interrupt priority for the camera frame interrupt*/


inline void Sys_Start_AtomicSection(void);
inline void Sys_End_AtomicSection(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_INTERRUPTS_H */

