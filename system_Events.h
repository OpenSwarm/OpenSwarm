/* 
 * File:   system_Events.h
 * Author: strenkwalder
 *
 * Created on 23 March 2015, 19:15
 */

#ifndef SYSTEM_EVENTS_H
#define	SYSTEM_EVENTS_H

#include <stdbool.h>
#include "definitions.h"

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

