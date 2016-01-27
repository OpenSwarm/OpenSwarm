/*!
 * \file
 * \ingroup base
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date{05 September 2015}
 * 
 * \brief It declares functions to allocate, free, and copy memory
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#ifndef SYSTEM_MEMORY_H
#define	SYSTEM_MEMORY_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "definitions.h"

void *Sys_Malloc(uint length);
void Sys_Free(void *);
void Sys_Memcpy(void *source, void *destination, uint length);


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_MEMORY_H */

