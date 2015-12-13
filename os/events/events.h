/*
 * File:   events.h
 * Author: Stefan M. Trenkwalder
 *
 * Created on 23 March 2015, 19:15
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


#ifndef SYSTEM_EVENTS_H
#define	SYSTEM_EVENTS_H

#include <stdbool.h>
#include "../definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct sys_event_data_s{
        void *value;
        uint16 size;

        struct sys_event_data_s *next;
    }sys_event_data;
    
    typedef bool (*pEventHandlerFunction)(uint16, uint16, sys_event_data *);//PID, EventID, EventData
    typedef bool (*pConditionFunction)(void *);


    bool Sys_Send_Event(uint16 eventID, void *data, uint16 data_size);
    inline bool Sys_Send_IntEvent(uint16 eventID, uint16 data);

    bool Sys_Register_Event(uint16 eventID);
    void Sys_Unregister_Event(uint16 eventID);
    
    bool Sys_Subscribe_to_Event(uint16 eventID, uint16 pid, pEventHandlerFunction handler, pConditionFunction condition);
    void Sys_Unsubscribe_from_Event(uint16 eventID, uint16 pid);
    void Sys_Unsubscribe_Process(uint16 pid);

    bool Sys_IsEventRegistered(uint16 eventID);


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_EVENTS_H */

