/*! \file
 * \ingroup base
 *  \brief  includes basic system calls to create atomic sections. (Sections that cannot be interrupted)
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 *  \version   	1.0
 *  \date      	2015
 *  \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 * \n To protect sections of code from any interruptions one has to use the following code:
 * \code
 * // do something
 *
 * Sys_Start_AtomicSection();
 *      
 *      //do something which should not be interrupted     
 * 
 * Sys_End_AtomicSection();
 * 
 * // do something else
 * \endcode
 */

#include "interrupts.h"
#include "definitions.h"

/*! \var static uint8 sys_IRQ_Priority
    \brief stores the previous interrupt level.
*/
static uint8 sys_IRQ_Priority = SYS_IRQP_SYSTEM_TIMER;

/*! \var static sint16 nesting
    \brief stores how often Sys_Start_AtomicSection() was executed.
*/
static sint16 nesting = 0;

/**
 * Starts an atomic section
 *
 * This Function starts an atomic section. This means the code afterwards cannot be interrupted by any interrupt.
 * @post	Sys_End_AtomicSection() must be called to execute any interrupt that happened or will happen.
 */
inline void Sys_Start_AtomicSection(){
    if(nesting == 0){
        sys_IRQ_Priority = SRbits.IPL;
        SRbits.IPL = SYS_IRQP_MAX;
    }
    nesting++;
}


/**
 * Starts an atomic section
 *
 * This Function starts an atomic section. This means the code afterwards cannot be interrupted by any interrupt.
 * @pre	Sys_Start_AtomicSection() must have been called.
 * @warning Do not execute Sys_End_AtomicSection() without having called Sys_Start_AtomicSection() once. Otherwise, the interrupt priority will be set to SYS_IRQP_SYSTEM_TIMER. This might cause errors.
 */
inline void Sys_End_AtomicSection(){
    nesting--;
    if(nesting <= 0){
        SRbits.IPL = sys_IRQ_Priority;
        nesting = 0;
    }
}

