/*!
 * \file
 * \ingroup base
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date{05 September 2015}
 * 
 * \brief It defines functions to allocate, free, and copy memory
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#include "memory.h"

#include "interrupts.h"
#include <stdlib.h>

#undef EXTENDED_MEMORY

static uint sys_bytes_used = 0;

typedef struct sys_memory_metadata_s{
    size_t size;
} sys_memory_metadata;

/**
 *
 * This Function allocates memory of the size \b length. This allocation is performed as atomic action.
 *
 * @param 	length  value how many bytes should be allocated
 * @return 	pointer to the allocated memory
 */
void *Sys_Malloc(uint length){
    uint8 *out = 0;
    
    Sys_Start_AtomicSection();

#ifdef EXTENDED_MEMORY 
    size_t total_length;
    
    total_length = length + sizeof(sys_memory_metadata);
    out = malloc(total_length);
    sys_memory_metadata *header = (sys_memory_metadata *) out;
    
    header->size = length;
    
    sys_bytes_used += total_length;
    Sys_End_AtomicSection();

    return (void*) (out + sizeof(sys_memory_metadata));
#else
    out = malloc(length);
    
    Sys_End_AtomicSection();
    
    return out;
#endif   
}

/**
 *
 * This Function frees dynamic allocated memory. This freeing is performed as atomic action.
 *
 * @param 	data   pointer to memory that should be freed.
 */
void Sys_Free(void *data){

    Sys_Start_AtomicSection();
#ifdef EXTENDED_MEMORY
    sys_memory_metadata *header = (sys_memory_metadata *) (((uint8*) data) - sizeof(sys_memory_metadata));
    sys_bytes_used -= (header->size + sizeof(sys_memory_metadata));//theoretically his should be the length
        
    free(header);
#else
    free(data);
#endif

    Sys_End_AtomicSection();

}

/**
 *
 * Function to copies memory of the size \b length from \b source_i to \b destination_o. This copying is performed as atomic action.
 *
 * @param[in] 	source_i        pointer to the source
 * @param[out] 	destination_o   pointer to the destination
 * @param[in] 	length          size of the memory that has to be copied
 */
void Sys_Memcpy(void *source_i, void *destination_o, uint length){
    uint8 *source;
    uint8 *destination;
    
    Sys_Start_AtomicSection();
    source = source_i;
    destination = destination_o;
    

    uint i = 0;
    for(i = 0; i < length; i++){
        destination[i] = source[i];
    }

    Sys_End_AtomicSection();

}

/**
 *
 * Function to returns the used memory
 *
 * @return 	uint Amount of bytes used
 */
uint Sys_MemoryUsed(){
    return sys_bytes_used;
}