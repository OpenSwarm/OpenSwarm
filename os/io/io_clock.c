#include "io_clock.h"
#include "../platform/e-puck/system_IO_HDI.h"

#include "io.h"
#include "../events/events.h"

static uint32 sys_clock = 0;
    
void Sys_SystemClock_Counter(void);

inline void Sys_Init_Clock(){
    Sys_Register_Event(SYS_EVENT_1ms_CLOCK);
    Sys_Register_IOHandler(Sys_SystemClock_Counter);
}

inline void Sys_Init_SystemTime(){
    Sys_Init_Clock();
}

void Sys_SystemClock_Counter(){
    
    static uint16 counter = 0;
    
    if( counter < STEPS_PER_MILISECOND){
        counter++;
        return;
    }
    Sys_Send_Event(SYS_EVENT_1ms_CLOCK, &sys_clock, sizeof(sys_clock));
    counter = 0;
    sys_clock++;
}

inline uint32 Sys_Get_SystemTime(){
    return Sys_Get_SystemClock();
}

inline uint32 Sys_Get_SystemClock(){
    return sys_clock;
}