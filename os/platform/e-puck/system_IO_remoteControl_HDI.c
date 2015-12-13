#include "system_IO_remoteControl_HDI.h"

#include "../../../os/definitions.h"
#include "../../../os/interrupts.h"
#include "../../../os/io/io.h"
#include "../../../os/io/remoteControl.h"
#include "../../../os/events/events.h"

bool message_arriving = false;
sint8 waiting_cycles = 20;

uint16 rx_buffer = 0;

bool isNewDataAvailable = false;

sint8 receiving_bit = RC_NOT_STARTED;

inline void Sys_Init_RemoteControl_HDI(void){
    REMOTE_DIR = INPUT_PIN; 
    
    IPC0bits.INT0IP = SYS_IRQP_REMOTECONTROL;
    INTCON2bits.INT0EP = 1;     //use falling edge
    IFS0bits.INT0IF = 0;        //clear to IRQ flag
    
    if(!Sys_Register_IOHandler(Sys_Receive_RemoteControl_Msg)){// no error handling used
        return;
    }
    
    Sys_Register_Event(SYS_EVENT_IO_REMOECONTROL);
}

inline void Sys_Start_RemoteControl_HDI(void){
    IFS0bits.INT0IF = 0;        ///clear to IRQ flag
    IEC0bits.INT0IE = 1;        //enable external INT0  
}

void __attribute__((__interrupt__, auto_psv))  _INT0Interrupt(void){
    //When a message arrives deactivate interrupt -> is activated at the end of the message
    IEC0bits.INT0IE = 0;
        IFS0bits.INT0IF = 0;
    message_arriving = true;
        waiting_cycles = RC_WAIT_INITIALLY;
    return;
}