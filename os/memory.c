/**
 * @file memory.c
 * @author  Stefan M. Trenkwalder <stefan.markus.trenkwalder@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Created on 05 September 2014
 *
 * LICENSE: adapted FreeBSD License
 * Copyright (c) 2015, Stefan M. Trenkwalder
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. If this or parts of this source code (as code or binary) is, in any form, used for an commercial product or service (in any form), this product or service must provide a clear notice/message to any user/customer that OpenSwarm was used in this product.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * @section DESCRIPTION
 *
 * This file includes all basic system calls to allocate, free and copy memory
 */

#include "memory.h"

#include "interrupts.h"
#include <stdlib.h>

void *Sys_Malloc(uint16 length){
    void *out = 0;

    Sys_Start_AtomicSection();

    out = malloc(length);

    Sys_End_AtomicSection();

    return out;
}

void Sys_Free(void *data){

    Sys_Start_AtomicSection();

    free(data);

    Sys_End_AtomicSection();

}

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
