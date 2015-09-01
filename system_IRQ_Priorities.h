/* 
 * File:   system_irq_priorities.h
 * Author: Stefan Trenkwalder
 *
 * Created on 28 August 2015, 13:46
 *
 * LICENCE: adapted FreeBSD Licence
 * Copyright (c)2015, Stefan trenkwalder
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. If this or parts of this source code or binary code in any form is used for a commercial product or service in any form, this product or service must provide a clear notice/message readable for any user or customer that this product or service uses OpenSwarm.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SYSTEM_IRQ_PRIORITIES_H
#define	SYSTEM_IRQ_PRIORITIES_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SYS_IRQP_SYSTEM_TIMER 2

#define SYS_IRQP_IO_TIMER 3

#define SYS_IRQP_UART1 4
#define SYS_IRQP_UART2 4

#define SYS_IRQP_CAMERA_PIXEL 5
#define SYS_IRQP_CAMERA_LINE  6
#define SYS_IRQP_CAMERA_FRAME 7

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IRQ_PRIORITIES_H */
