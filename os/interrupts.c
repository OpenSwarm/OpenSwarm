/*! \file
 * \ingroup base
 *  \brief  It defines the functions to create atomic sections.
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

#include "processes/data.h"

/*! \var static uint irq_counter
    \brief counts the amount of occurred interrupts.
*/
static uint irq_counter = 0;
/**
 *
 * This Function starts an atomic section. This means the code afterwards cannot be interrupted by any interrupt.
 * @note    This function can be called within an atomic section. However, it doesn't change the behaviour when called within an atomic section. To end an atomic section, Sys_End_AtomicSection() must be called as often as Sys_Start_AtomicSection() was called.
 * @post	Sys_End_AtomicSection() must be called to execute any interrupt that happened or will happen.
 */
inline void Sys_Start_AtomicSection(){
    /*if(Sys_GetCurrentIRQPNesting() == 0){
        Sys_SetCurrentIRQPriority((uint)((SR & 0x00E0) >> 5));
        SRbits.IPL = SYS_IRQP_MAX;
    }
    Sys_IncCurrentIRQPNesting();
     */
}


/**
 *
 * This Function ends an atomic section. This means the code afterwards can be interrupted by a interrupt.
 * @pre	Sys_Start_AtomicSection() must have been called.
 */
inline void Sys_End_AtomicSection(){
    /*Sys_DecCurrentIRQPNesting();
    sint irqn = Sys_GetCurrentIRQPNesting();
    
    if(irqn <= 0){
        if(irqn == 0){
            SR = SR & ( ((Sys_GetCurrentIRQPriority() << 5) & 0x00E0) | 0xFF1F);
        }
        Sys_SetCurrentIRQPNesting(0);
    }
     */
}

/**
 *
 * This Function ends an atomic section. This means the code afterwards can be interrupted by a interrupt.
 *
 */
sint Sys_Get_IRQNestingLevel(){
    return 0;//Sys_GetCurrentIRQPNesting();
}

/**
 *
 * This Function increases the interrupt counter, which counts the amount of occurred interrupts.
 *
 */
inline void Sys_Inc_InterruptCounter(void){
    irq_counter++;
}

/**
 *
 * This Function sets the interrupt counter, which counts the amount of occurred interrupts, to 0. 
 *
 */
inline void Sys_Reset_InterruptCounter(void){
    irq_counter = 0;
}

/**
 *
 * This Function gives you the number of called interrupt since it was last reset.
 *
 * @return uint returns the number of called interrupts
 */
uint Sys_Get_InterruptCounter(void){
    return irq_counter;
}