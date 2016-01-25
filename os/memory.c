/*!
 * \file
 * \ingroup base
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date{05 September 2015}
 * 
 * \brief defines functions to allocate, free, and copy memory
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#include "memory.h"

#include "interrupts.h"
#include <stdlib.h>

/**
 * Function to allocate \b length bytes of memory
 *
 * This Function allocates memory of the size \b length. This allocation is performed as atomic action.
 *
 * @param 	length  value how many bytes should be allocated
 * @return 	pointer to the allocated memory
 */
void *Sys_Malloc(uint16 length){
    void *out = 0;

    Sys_Start_AtomicSection();

    out = malloc(length);

    Sys_End_AtomicSection();

    return out;
}

/**
 * Function to free memory
 *
 * This Function frees dynamic allocated memory. This freeing is performed as atomic action.
 *
 * @param 	data   pointer to memory that should be freed.
 */
void Sys_Free(void *data){

    Sys_Start_AtomicSection();

    free(data);

    Sys_End_AtomicSection();

}

/**
 * Function to copies memory of the size \b length from \b source_i to \b destination_o.
 *
 * Function to copies memory of the size \b length from \b source_i to \b destination_o. This copying is performed as atomic action.
 *
 * @param 	source_i        pointer to the source
 * @param 	destination_o   pointer to the destination
 * @param 	length          size of the memory that has to be copied
 */
void Sys_Memcpy(void *source_i, void *destination_o, uint16 length){

    uint8 *source = source_i;
    uint8 *destination = destination_o;
    
    Sys_Start_AtomicSection();

    uint16 i = 0;
    for(i = 0; i < length; i++){
        destination[i] = source[i];
    }

    Sys_End_AtomicSection();

}
