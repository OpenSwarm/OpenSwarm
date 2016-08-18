/*!
 * \file
 * \ingroup events
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 23 March 2015
 * 
 * \brief defines functions to create, (un)subscribe, (un)register, and delete events and related handler.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#include "events.h"

#include "../definitions.h"
#include "../interrupts.h"
#include "../memory.h"

#include "../processes/system_Timer.h"

#include <stdlib.h>

/**
 * @brief A single linked list element containing all information needed to handle an occurred event;
 */
typedef struct sys_event_handler_s{
    pEventHandlerFunction   handler;    /*!< subscribed handler function */
    pConditionFunction      condition;  /*!< subscribed condition function */
    void*                   userData;   /*!< pointer to userdata which is a pointer that was stored when Sys_Subscribe_to_Event was called. It can be used */
    sys_event_data*         bufferd_data;/*!< pointer to a list of event data that has been sent and buffered*/
    struct sys_event_handler_s *next;   /*!< pointer to the next element in the list */
}sys_event_handler;

/**
 * @brief A single linked list for registered events
 */
typedef struct sys_event_s{
    uint                id;         /*!< event identifier */
    sys_event_handler * handlers;   /*!< list of subscribed handlers */
    struct sys_event_s *next;       /*!< pointer to the next element in the list */
}sys_event;

/**
 * @brief A single linked list for occurred events
 */
typedef struct sys_occurred_event_s{
    uint                            id;         /*!< event identifier */
    struct sys_occurred_event_s *   next;       /*!< pointer to the next element in the list */
}sys_occurred_event;


/*******************************************************************************
 *  Global Event Variables
 ******************************************************************************/
sys_event *             registered_events   = 0;/*!< pointer to the List of registered events*/

/*******************************************************************************
 *  Functions Prototypes
 ******************************************************************************/
void Sys_Clear_Event(sys_event *e);
void Sys_Clear_EventHandler(sys_event_handler *h);
void Sys_Clear_BufferedList(sys_event_data** d);
void Sys_Clear_BufferedData(sys_event_data* d);
sys_event *Sys_FindEvent(uint eventID);

/*******************************************************************************
 *  Functions Definitions
 ******************************************************************************/

/**
 *
 * This function registers an new event. The registration tells the operating system that this event can occur.
 *
 * @param[in] 	eventID    ID of the event
 * @return 	was it successful.
 */
bool Sys_Register_Event(uint eventID){
    sys_event** empty_element;
    
    Sys_Start_AtomicSection();
    
    empty_element = &registered_events;
    
    while( (*empty_element) != 0){//go through all elements in the list until you reached the end
        if((*empty_element)->id == eventID){ //if the event has already been registered
            Sys_End_AtomicSection();
            return true;
        }
        
        empty_element = &((*empty_element)->next);
    }
    
    sys_event* new_event = (sys_event*) Sys_Malloc(sizeof(struct sys_event_s));
    if(new_event == 0){
        Sys_End_AtomicSection();
        return false;
    }
    
    new_event->id       = eventID;
    new_event->handlers = 0;
    new_event->next     = 0;
    
    (*empty_element) = new_event;
    Sys_End_AtomicSection();
    return true;
}

/**
 *
 * This function unregisters an event
 *
 * @param[in] 	eventID     ID of the event
 */
void Sys_Unregister_Event(uint eventID){
    sys_event**             event_element;
    
    Sys_Start_AtomicSection();
    event_element = &registered_events;
    
    while( (*event_element) != 0){//go through all elements in the list until you reached the end
        if((*event_element)->id == eventID){ //if the event has already been registered
            sys_event* event = (*event_element);
            
            (*event_element) = event->next;
            event->next = 0;
            
            Sys_Clear_Event(event);
            Sys_Free(event);
            break;
        }
        
        event_element = &((*event_element)->next);
    }
        
    Sys_End_AtomicSection();
    return;
}

/**
 *
 * This function subscribes a specific handler function to an process and a specific event
 *
 * @param[in] 	eventID     ID of the event
 * @param[in] 	handler     pointer to the function that should handle the event data
 * @param[in] 	condition   pointer to the function that decides if the handler should be executed or not
 * @param[in] 	user_data   pointer to an arbitrary element  which get passed to the handler every time it is executed.
 * @return 	was it successful.
 */
bool Sys_Subscribe_to_Event(uint eventID, pEventHandlerFunction handler, pConditionFunction condition, void *user_data){
    sys_event* event;
    sys_event_handler **handlers;
    
    if(handler == 0){
        return true;
    }
    
    Sys_Start_AtomicSection();
    event = Sys_FindEvent(eventID);
    
    if(event == 0){
        Sys_End_AtomicSection();
        return false;
    }
    
    handlers = &(event->handlers);
    
    while( (*handlers) != 0){//go through all elements in the list until you reached the end
        if((*handlers)->handler == handler){ //if the handler has already been subscribed
            (*handlers)->condition = condition;
            (*handlers)->userData = user_data;
            
            Sys_End_AtomicSection();
            return true;
        }
        handlers = &((*handlers)->next);
    }
        
    sys_event_handler* new_handler = (sys_event_handler*) Sys_Malloc(sizeof(struct sys_event_handler_s));
    if(new_handler == 0){
        Sys_End_AtomicSection();
        return false;
    }
    
    new_handler->handler    = handler;
    new_handler->condition  = condition;
    new_handler->userData   = user_data;
    new_handler->bufferd_data = 0;
    new_handler->next = 0;
    
    (*handlers) = new_handler;
    Sys_End_AtomicSection();
    return true;
}

/**
 *
 * This function only unsubscribes a specific handler function from a specific Event
 *
 * @param[in] 	eventID     ID of the event
 * @param[in] 	handler     pointer to the handler function
 */
void Sys_Unsubscribe_Handler(uint eventID, pEventHandlerFunction handler, void *user_data){
    sys_event* event;
    sys_event_handler **handlers;
    
    Sys_Start_AtomicSection();
    event = Sys_FindEvent(eventID);
   
    if(event == 0){
        Sys_End_AtomicSection();
        return;
    }
    
    handlers = &(event->handlers);
    
    while( (*handlers) != 0){//go through all elements in the list until you reached the end
        if(((*handlers)->handler == handler || handler == 0) && ((*handlers)->userData == user_data || user_data == 0)){ //if the handler has already been subscribed            
            sys_event_handler* hdl = (*handlers);
            
            (*handlers) = hdl->next;
            hdl->next = 0;
            
            Sys_Clear_BufferedData(hdl->bufferd_data);
            Sys_Free(hdl);
            continue;
        }
        handlers = &((*handlers)->next);
    }
    
    Sys_End_AtomicSection();
    return;
}

/**
 *
 * This function sends an event to all subscribers.
 *
 * @param[in] 	eventID    ID of the event
 * @param[in] 	data       pointer to the data that want to be sent as an event
 * @param[in] 	data_size  size of the data in bytes
 * @return true if it was successful.
 */
bool Sys_Send_Event(uint eventID, void *data, uint data_size){
    Sys_Start_AtomicSection();//doesn't consume execution time
    Sys_Stop_SystemTimer();
    
        Sys_Inc_EventCounter();

        sys_event *event = Sys_FindEvent(eventID);
        
        if(event == 0){
            Sys_Continue_SystemTimer();
            Sys_End_AtomicSection();
            return false;
        }
        
        sys_event_handler* hdl = event->handlers;
        while(hdl != 0){
            sys_event_data* evData = (sys_event_data*) Sys_Malloc(sizeof(struct sys_event_data_s));
            if(evData == 0){
                Sys_Continue_SystemTimer();
                Sys_End_AtomicSection();
                return false;
            }
                
            evData->size = data_size;
            evData->next = 0;
            evData->value = Sys_Malloc(data_size);
            if(evData->value == 0){
                Sys_Free(evData);
                
                Sys_Continue_SystemTimer();
                Sys_End_AtomicSection();
                return false;
            }
            
            Sys_Memcpy(data, evData->value, data_size);
            
            bool condition_met = true;
            if(hdl->condition != 0){
                condition_met = hdl->condition(eventID, evData, hdl->userData);
            }
                
            if(condition_met){
                sys_event_data** data_element = &(hdl->bufferd_data);
                while( (*data_element) != 0){
                    data_element = &((*data_element)->next);
                }
                (*data_element) = evData;
            }else{
                Sys_Free(evData->value);
                Sys_Free(evData);
            }
            
            hdl = hdl->next;
        }

    Sys_Continue_SystemTimer();
    Sys_End_AtomicSection();
    return true;
}

/**
 *
 * This function sends an integer (16-bit) to all subscribers.
 *
 * @param[in] 	eventID    ID of the event
 * @param[in] 	data       integer value that should be sent as an event
 * @return 	was it successful.
 */
inline bool Sys_Send_IntEvent(uint eventID, uint data){
    uint value = data;
    return Sys_Send_Event(eventID, &value, 2);
}

/**
 *
 * This function sends an event to all subscribers and executed the handler immediately.
 *
 * @param[in] 	eventID    ID of the event
 * @param[in] 	data       pointer to the data that want to be sent as an event
 * @param[in] 	data_size  size of the data in bytes
 * @return true if it was successful.
 */
bool Sys_Send_CriticalEvent(uint eventID, void *data_in, uint data_size){
    Sys_Start_AtomicSection();//doesn't consume execution time
    Sys_Stop_SystemTimer();
    
        Sys_Inc_EventCounter();

        sys_event *event = Sys_FindEvent(eventID);
        if(event == 0){
            Sys_Continue_SystemTimer();
            Sys_End_AtomicSection();
            return false;
        }
        
        sys_event_data data;
        data.value = Sys_Malloc(data_size);
        if(data.value == 0){
            Sys_Continue_SystemTimer();
            Sys_End_AtomicSection();
            return false;
        }
        
        Sys_Memcpy(data_in, data.value, data_size);
        data.size  = data_size;
        data.next  = 0;
        
        
        sys_event_handler* hdl = event->handlers;
        while(hdl != 0){
            bool condition_met = true;
            if(hdl->condition != 0){
                condition_met = hdl->condition(eventID, &data, hdl->userData);
            }
                
            if(condition_met){
                hdl->handler(eventID, &data, hdl->userData);
            }
            
            hdl = hdl->next;
        }
        
        Sys_Free(data.value);

    Sys_Continue_SystemTimer();
    Sys_End_AtomicSection();
    return true;
}

/**
 *
 * This function executes all buffered events.
 *
 */
void Sys_Execute_BufferedEvents(void){
    Sys_Start_AtomicSection();
       
        sys_event *event = registered_events;
        while(event != 0){
            
            sys_event_handler *handler = event->handlers;
            while(handler != 0){
                
                sys_event_data** data = &(handler->bufferd_data);
                sys_event_data* temp = 0;
                while( (*data) != 0){
                    handler->handler(event->id, (*data), handler->userData);
                    
                    temp = (*data);
                    (*data) = (*data)->next;
                    
                    Sys_Clear_BufferedData(temp);
                }                
                handler = handler->next;
            }            
            event = event->next;
        }        
    Sys_End_AtomicSection();
}

/**
 *
 * This function replicates a sys_event_data struct.
 *
 * @param[in] 	data       pointer to the data that needs to be replicated
 * @return a pointer to the new data
 */
sys_event_data* Sys_Copy_EventData(sys_event_data* data){
    Sys_Start_AtomicSection();
    
    //create new
    sys_event_data* evData = (sys_event_data*) Sys_Malloc(sizeof(struct sys_event_data_s));
    if(evData == 0){
        Sys_End_AtomicSection();
        return 0;
    }
                
    evData->size = data->size;
    evData->next = data->next;
    
    //create new value
    evData->value = Sys_Malloc(data->size);
    if(evData->value == 0){
        Sys_Free(evData);
                
        Sys_End_AtomicSection();
        return 0;
    }
            
    Sys_Memcpy(data->value, evData->value, data->size);
    
    return evData;
}

/**
 *
 * This function frees the memory of an sys_event struct.
 *
 * @param[in] 	event      pointer to the event that needs to be deleted
 */
void Sys_Clear_Event(sys_event *event){
    sys_event_handler * handler;
    sys_event_handler * temp;
    
    Sys_Start_AtomicSection();
    if(event != 0){
        handler = event->handlers;
    
        event->next = 0;
        event->handlers = 0;
    
        while(handler != 0){
            temp = handler;
            handler = handler->next;
        
            Sys_Clear_EventHandler(temp);
        }
    
        Sys_Free(event);
    }
    Sys_End_AtomicSection();
}

/**
 *
 * This function frees the memory of an sys_event_handler struct.
 *
 * @param[in]   hdlr      pointer to the event handler struct that needs to be deleted
 */
void Sys_Clear_EventHandler(sys_event_handler *hdlr){
    
    Sys_Start_AtomicSection();
    if(hdlr != 0){
        hdlr->next = 0;
        hdlr->bufferd_data = 0;
        Sys_Clear_BufferedList( &(hdlr->bufferd_data) );
    
        Sys_Free(hdlr);
    }
    
    Sys_End_AtomicSection();
}

/**
 *
 * This function frees the memory of a list of sys_event_data structs.
 *
 * @param[in]   data      pointer to the event data struct that needs to be deleted
 */
void Sys_Clear_BufferedList(sys_event_data** data){    
    sys_event_data* d;
    sys_event_data* temp;
    
    Sys_Start_AtomicSection();
    if(data != 0){
        d = (*data);
        (*data) = 0;
    
        while( d != 0){
            temp = d;
            d = d->next;
        
            Sys_Clear_BufferedData(temp);
        }
    }
    
    Sys_End_AtomicSection();
}

/**
 *
 * This function frees the memory of an sys_event_data struct.
 *
 * @param[in]   data      pointer to the event data struct that needs to be deleted
 */
void Sys_Clear_BufferedData(sys_event_data* data){
    Sys_Start_AtomicSection();
    
    if(data != 0 ){   
        data->next = 0;
        Sys_Free(data->value);
        Sys_Free(data);
    }
    
    Sys_End_AtomicSection();
}

/**
 *
 * This function frees the memory of an sys_event_data struct.
 *
 * @param[in]   data      pointer to the event data struct that needs to be deleted
 */
inline void Sys_Clear_EventData(sys_event_data* data){
    Sys_Clear_BufferedData(data);
}

/**
 *
 * This function returns the sys_event struct of the event with id eventID.
 *
 * @param[in] 	eventID    ID of the event
 * @return sys_event struct with id eventID.
 */
sys_event *Sys_FindEvent(uint eventID){
    sys_event* event = 0;
    
    Sys_Start_AtomicSection();
    
    event = registered_events;
    
    while(event != 0){
        if(event->id == eventID){
            break;
        }
        
        event = event->next;
    }
    
    Sys_End_AtomicSection();
    return event;
}

static uint event_counter = 0; /*!< amount of occurred events since the last reset */

/**
 *
 * This function increases the event counter, which counts the number of sent events since the last reset.
 *
 */
inline void Sys_Inc_EventCounter(void){
    event_counter++;    
}

/**
 *
 * This function resets the event counter, which counts the number of sent events since the last reset.
 *
 */
inline void Sys_Reset_EventCounter(void){
    event_counter = 0;
}

/**
 *
 * This function returns the event counter, which counts the number of sent events since the last reset.
 *
 * @return uint the number of sent events since the last reset
 */
uint Sys_Get_EventCounter(void){
    return event_counter;
}