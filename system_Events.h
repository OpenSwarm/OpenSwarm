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

    typedef void (*pEventHandler)(void *);

    bool Sys_Send_Event(uint16 eventID, void *data, uint16 data_size);

    bool Sys_Register_Event(uint16 eventID);
    void Sys_Unregister_Event(uint16 eventID);
    
    bool Sys_Subscribe_to_Event(uint16 eventID, uint16 pid, pEventHandler handler);
    void Sys_Unsubscribe_from_Event(uint16 eventID, uint16 pid);


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_EVENTS_H */

