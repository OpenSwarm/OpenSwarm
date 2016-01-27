/*!
 * \file
 * \ingroup events
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 23 March 2015
 * 
 * \brief declares functions to create, (un)subscribe, (un)register, and delete events and related handler. 
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

/*! \defgroup events Event Management
 * \brief Events are the main information exchange method in OpenSwarm. Events can be emitted, created, and (un)registered. Functions to handle events can also (un)subscribed to certain events. It can be used to synchronise and communicate with processes, to implement asynchronous programming model, and process incoming data/signals. 
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 *  
 * In general, events are data structures passed from one process/part of OpenSwarm to another. 
 * 
 * For example, an event might be triggered by detecting another robot within range. If a function is subscribed to this event, the robot can react on the detection of another robot.
 * 
 * Events can also carry additional information---such as the distance and angle to the detected robot.
 * 
 * \section events_usage Usage
 * The event system is ready to be used after the start of OpenSwarm. Each event is identified by an integer \b eventID. To use an event the following steps have to be taken:
 *  -#  An event (\b eventID ) can be (un)registered by Sys_Register_Event(uint16 eventID) and Sys_Unregister_Event(uint16 eventID). When an event is registered, it means that an event (\b eventID) can occur and be handled by OpenSwarm.
 *  -#  After the event was registered, processes can be subscribed to it with Sys_Subscribe_to_Event(uint16 eventID, uint16 pid, pEventHandlerFunction handler, pConditionFunction condition) and Sys_Unsubscribe_from_Event(uint16 eventID, uint16 pid).
 *      During the subscription, an event handler (i.e. a function to process the event) is subscribed to a specific event (\b eventID ) and a process. 
 *      Each event handler of a process for an specific event is unique. As a result, the same handler function can be used by multiple processes.
 *  -#  After an event is registered, events can be sent with Sys_Send_Event(uint16 eventID, void *data, uint16 data_size) and Sys_Send_IntEvent(uint16 eventID, uint16 data).
 * 
 * \section events_example Example 
 * \code
 * #include "os/system.h"
 * #include "os/events/events.h"
 * 
 * #define USER_EVENT_ID 0xCC
 * 
 * bool pConditionFunction(void *data){//only executes the eventHandler every 5th time.
 *      static int counter = 0;
 * 
 *      if(++counter >= 4){//if the event occurred the 5th time
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
 *  Sys_Start_Kernel();//OpenSwarm is running now.
 * 	
 * while(true){
 *      if( ... ){//under a certain condition
 *           Sys_Send_Event(USER_EVENT_ID, &variable, sizeof(int));
 *           //alternatively, you could use Sys_Send_IntEvent(USER_EVENT_ID, variable);
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
     * @brief It is a single linked list element and contains data of an occurred event.
     */
    typedef struct sys_event_data_s{
        void *value;/*!< pointer to data transferred by the event */
        uint size;/*!< size of the transferred data (bytes) */

        struct sys_event_data_s *next;/*!< pointer to the next element in the List */
    }sys_event_data;
    
     
    /**
     * @brief Event handler function pointer type (process id, event id, received data) 
     * 
     * This function points to an event handler function, which processes incoming events and its data. 
     */
    typedef bool (*pEventHandlerFunction)(uint /*PID*/, uint /*EventID*/, sys_event_data */*EventData*/);
    
    /**
     * @brief Condition function pointer type 
     * 
     * This function points to a condition function, which defines if an event handler should be executed or not. 
     */
    typedef bool (*pConditionFunction)(void *);


    bool Sys_Send_Event(uint eventID, void *data, uint data_size);
    inline bool Sys_Send_IntEvent(uint eventID, uint data);

    bool Sys_Register_Event(uint eventID);
    void Sys_Unregister_Event(uint eventID);
    
    bool Sys_Subscribe_to_Event(uint eventID, uint pid, pEventHandlerFunction handler, pConditionFunction condition);
    void Sys_Unsubscribe_from_Event(uint eventID, uint pid);
    void Sys_Unsubscribe_Process(uint pid);

    bool Sys_IsEventRegistered(uint eventID);


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_EVENTS_H */

