#include "system_IO_clock.h"

#include "system_IO.h"

static uint32 sys_clock = 0;
    
void Sys_SystemClock_Counter(void);

inline void Sys_Init_Clock(){
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
    
    counter = 0;
    sys_clock++;
}

inline uint32 Sys_Get_SystemTime(){
    return Sys_Get_SystemClock();
}

inline uint32 Sys_Get_SystemClock(){
    return sys_clock;
}