/*
 * File:   HDI_DSPIC30F6014A.h
 * Author: Stefan M. Trenkwalder
 *
 * Created on 07 July 2014, 14:47
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
 */

#ifndef HDI_DSPIC30F6014A_H
#define	HDI_DSPIC30F6014A_H

#include "library/motor_led/e_epuck_ports.h"

#ifdef	__cplusplus
extern "C" {
#endif

// Interrupt Vector Table
#define ADDRESS_IVT                 0x000004
#define ADDRESS_ITV_OSC_FAIL        ADDRESS_IVT+2
#define ADDRESS_ITV_ADDRESS_ERROR   ADDRESS_IVT+4
#define ADDRESS_ITV_STACK_ERROR     ADDRESS_IVT+6
#define ADDRESS_ITV_MATH_ERROR      ADDRESS_IVT+8
#define ADDRESS_IVT_T1              0x00001A


#define ADDRESS_AIVT                 0x000084
#define ADDRESS_AITV_OSC_FAIL        ADDRESS_AIVT+2
#define ADDRESS_AITV_ADDRESS_ERROR   ADDRESS_AIVT+4
#define ADDRESS_AITV_STACK_ERROR     ADDRESS_AIVT+6
#define ADDRESS_AITV_MATH_ERROR      ADDRESS_AIVT+8
#define ADDRESS_AIVT_T1              0x00009A



#ifdef	__cplusplus
}
#endif

#endif	/* HDI_DSPIC30F6014A_H */

