#include "system_Events.h"
#include "system_Process_Management_HDI.h"
#include "system_Timer_HDI.h"
#include "system_Memory.h"
#include <string.h>
#include <stdlib.h>

typedef struct sys_subscribed_process_s{
    uint16 pid;
    struct sys_subscribed_process_s *next;
}sys_subscribed_process;

typedef struct sys_registered_event_s{
    uint16 id;
    sys_subscribed_process *subscribers;
    struct sys_registered_event_s *next;
}sys_registered_event;
/*
typedef struct sys_event_s {
    uint16 id;
    void *value;
    uint16 value_length;
} sys_event;
*/
sys_registered_event *registered_events = 0;

sys_registered_event *Sys_Find_Event(uint16 eventID);

bool Sys_Send_Event(uint16 eventID, void *data, uint16 data_size){
    Sys_Stop_SystemTimer_HDI();//doesn't consume execution time

   sys_registered_event *event = Sys_Find_Event(eventID);
   if(event == 0){
        Sys_Continue_SystemTimer_HDI();
       return false;
   }

   sys_subscribed_process *process = event->subscribers;
   while(process != 0){
       Sys_Add_Event_to_Process(process->pid, eventID, data, data_size);
       process = process->next;
   }

    Sys_Continue_SystemTimer_HDI();
    return true;
}
inline bool Sys_Send_IntEvent(uint16 eventID, uint16 data){
    uint16 value = data;
    return Sys_Send_Event(eventID, &value, 2);
}

bool Sys_Register_Event(uint16 eventID){
    sys_registered_event* events = registered_events;
    sys_registered_event* next_event = registered_events;
    sys_registered_event* new_event = 0;
    

    while(next_event != 0){
        if(events->id == eventID){ //is Event (EID) already registered?
            return false;
        }

        events = next_event;
        next_event = events->next;
    }

    new_event = (sys_registered_event*) Sys_Malloc(sizeof(struct sys_registered_event_s));
    if(new_event == 0){
        return false;
    }
    new_event->id = eventID;
    new_event->subscribers = 0;
    new_event->next = 0;

    if(registered_events == 0){
        registered_events = new_event;
        return true;
    }

    events->next = new_event;
    return true;
}

bool Sys_Subscribe_to_Event(uint16 eventID, uint16 pid, pEventHandlerFunction handler, pConditionFunction condition){
    sys_registered_event* events = registered_events;
    
    while(events != 0){
        if(events->id == eventID){
            if(!Sys_Add_Event_Subscription(pid, eventID, handler, condition)){
                return false;
            }

            sys_subscribed_process *subscribed_process = events->subscribers;
            while(subscribed_process != 0 && subscribed_process->next != 0){
                if(subscribed_process->pid == pid){
                    return true;
                }

                subscribed_process = subscribed_process->next;
            }

            sys_subscribed_process *new_process = (sys_subscribed_process*) Sys_Malloc(sizeof(struct sys_subscribed_process_s));
            if(new_process == 0){
                return false;
            }
            new_process->pid = pid;
            new_process->next = 0;

            if(events->subscribers == 0){
                events->subscribers = new_process;
                return true;
            }
            
            subscribed_process->next = new_process;
            return true;
        }

        events = events->next;
    }
    return false;
}


void Sys_Unregister_Event(uint16 eventID){
    sys_registered_event* event = registered_events;
    sys_registered_event* next_event = registered_events;

    if(registered_events == 0){
        return;
    }else{
        if(event->id == eventID){
            registered_events = event->next;

            sys_subscribed_process* subscriber = event->subscribers;
            while(subscriber != 0){
               sys_subscribed_process* temp = subscriber;
               subscriber = subscriber->next;

               Sys_Free(temp);
            }

            Sys_Remove_All_Event_Subscriptions(eventID);//
            return;
        }
    }

    while(next_event != 0){
        if(next_event->id == eventID){
            sys_subscribed_process* subscriber = event->subscribers;
            while(subscriber != 0){//REMOVE all subscribed processes
               sys_subscribed_process* temp = subscriber;
               subscriber = subscriber->next;

               Sys_Free(temp);
            }
            Sys_Remove_All_Event_Subscriptions(eventID);//removes event elements from all processes
            event = next_event->next;
            return;
        }

        event = next_event;
        next_event = next_event->next;
    }

    return;
}

void Sys_Unsubscribe_from_Event(uint16 eventID, uint16 pid){
    sys_registered_event* event = Sys_Find_Event(eventID);

    sys_subscribed_process* subscriber = event->subscribers;
    if(event->subscribers->pid == pid){
        event->subscribers = event->subscribers->next;
        Sys_Free(subscriber);

        Sys_Remove_Event_Subscription(pid, eventID, 0);
        return;
    }

    sys_subscribed_process* next_subscriber = event->subscribers->next;
    while(next_subscriber != 0){
        if(next_subscriber->pid == pid){
            subscriber->next = next_subscriber->next;
            Sys_Free(next_subscriber);

            Sys_Remove_Event_Subscription(pid, eventID, 0);
            return;
        }

        subscriber = next_subscriber;
        next_subscriber = next_subscriber->next;
    }
}

void Sys_Unsubscribe_Handler_from_Event(uint16 eventID, pEventHandlerFunction func,  uint16 pid){
    sys_registered_event* event = Sys_Find_Event(eventID);

    sys_subscribed_process* subscriber = event->subscribers;
    if(event->subscribers->pid == pid){
        event->subscribers = event->subscribers->next;
        Sys_Free(subscriber);

        Sys_Remove_Event_Subscription(pid, eventID, func);
        return;
    }

    sys_subscribed_process* next_subscriber = event->subscribers->next;
    while(next_subscriber != 0){
        if(next_subscriber->pid == pid){
            subscriber->next = next_subscriber->next;
            Sys_Free(next_subscriber);

            Sys_Remove_Event_Subscription(pid, eventID, func);
            return;
        }

        subscriber = next_subscriber;
        next_subscriber = next_subscriber->next;
    }
}

sys_registered_event *Sys_Find_Event(uint16 eventID){
    sys_registered_event* event = registered_events;

    while(event != 0){
        if(event->id == eventID){
            return event;
        }

        event = event->next;
    }

    return 0;
}

bool Sys_IsEventRegistered(uint16 eventID){
    sys_registered_event* event = registered_events;

    while(event != 0){
        if(event->id == eventID){
            return true;
        }

        event = event->next;
    }

    return false;
}

void Sys_Unsubscribe_Process(uint16 pid){
    sys_registered_event* event = registered_events;

    while(event != 0){//look into every event
        sys_subscribed_process* subscriber = event->subscribers;
        if(event->subscribers->pid == pid){
            event->subscribers = event->subscribers->next;
            Sys_Free(subscriber);

            Sys_Remove_Event_Subscription(pid, event->id, 0);
            continue;
        }

        sys_subscribed_process* next_subscriber = subscriber->next;
        while(next_subscriber != 0){
            if(next_subscriber->pid == pid){
                subscriber->next = next_subscriber->next;
                Sys_Free(next_subscriber);

                Sys_Remove_Event_Subscription(pid, event->id, 0);
                break;
            }

            subscriber = next_subscriber;
            next_subscriber = next_subscriber->next;
        }
    }
}
