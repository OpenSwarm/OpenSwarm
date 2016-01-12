/*!
 * \file
 * \ingroup events
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 23 March 2015
 * 
 * \brief functions to create handle and configure events.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

/*! \defgroup events Event System
 * \brief Functions to emit, create, (un)subscribe, (un)register, and manage events.
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * Events are a main component of OpenSwarm. It can be used to synchronise and communicate with processes, to implement asynchronous programming model, and process incoming data/signals. 
 * 
 * \section events_usage Usage
 * The event system doesn't need to be initialised. Any event is identified by an integer \b eventID. To use an event the following steps have to be taken:
 *  -#  An event (\b eventID ) can be (un)registered by Sys_Register_Event(uint16 eventID) and Sys_Unregister_Event(uint16 eventID). When an event is registered, it means that an event (\b eventID ) can occur and handled by OpenSwarm.
 *  -#  After the event was registered, processes can be subscribed to it with Sys_Subscribe_to_Event(uint16 eventID, uint16 pid, pEventHandlerFunction handler, pConditionFunction condition) and Sys_Unsubscribe_from_Event(uint16 eventID, uint16 pid).
 *      During the subscription, an event handler (i.e. a function to process data that was sent by events) is subscribed to a specific event (\b eventID ) and a process. 
 *      Each event handler of a process for an specific event is unique. As a result the same handler function can be assigned to the same event if they are assigned to other processes.
 *  -#  After an event is registered, events can be sent with Sys_Send_Event(uint16 eventID, void *data, uint16 data_size) and Sys_Send_IntEvent(uint16 eventID, uint16 data).
 * 
 * \section events_example Example 
 * \code
 * #include "os/system.h"
 * #include "os/events/events.h"
 * 
 * #define USER_EVENT_ID 0xCC
 * 
 * bool pConditionFunction(void *data){//only execute the the eventHandler every 5th time.
 *      static int counter = 0;
 * 
 *      if(++counter >= 4){//if event occurred 5 times
 *          counter = 0;
 *          return true;//execute eventHandler
 *      }
 * 
 *     return false;//don't execute eventHandler
 * }
 * 
 * bool eventHandler(uint16 pid, uint16 eventID, sys_event_data *data){
 *      //do something with the data
 * }
 * 
 * int main(void){
 *  //initialise some global or local variables
 *
 *  int variable;
 * 
 * 	Sys_Init_Kernel();
 *
 *  Sys_Register_Event(USER_EVENT_ID);
 *      
 *  Sys_Start_Kernel();      
 * 	while(1){
 * 
 *      if( condition ){
 *           Sys_Send_Event(USER_EVENT_ID, &variable, sizeof(int));
 *      }
 *      //do something
 * 	}
 * }
 * \endcode
 * 
 * \section events_license License
 * LICENSE: adapted FreeBSD License (see http://openswarm.org/license)\n
 * Copyright (c) 2015, Stefan M. Trenkwalder\n
 * All rights reserved. 
 */

#ifndef SYSTEM_EVENTS_H
#define	SYSTEM_EVENTS_H

#include <stdbool.h>
#include "../definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
    /**
     * @brief This struct contains data of the size \b size at the memory of \b value. It is a struct for a linked list.
     */
    typedef struct sys_event_data_s{
        void *value;/*!< pointer to the data transfered by an event */
        uint16 size;/*!< size of the dransfered data (bytes) */

        struct sys_event_data_s *next;/*!< pointer to the next element in the List */
    }sys_event_data;
    
     
    /**
     * @brief Event handler function pinter type (process id, event id, received data) 
     */
    typedef bool (*pEventHandlerFunction)(uint16, uint16, sys_event_data *);//PID, EventID, EventData
     
    /**
     * @brief Condition function pinter type (received data)
     */
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

