#include "system_IO_remoteControl.h"
#include "HDI_epuck_ports.h"

#include <stdbool.h>
#include "system_IO.h"
#include "system_Interrupts.h"

static bool message_arriving = false;
static sint8 waiting_cycles = 20;

typedef struct sys_RemoteControl_Data_s{
    uint8           : 4;
    uint8   check   : 1;
    uint8   address : 5;
    uint8   data    : 6;
} sys_RemoteControl_Data;


static sys_RemoteControl_Data rx_buffer_1 = {0};
static sys_RemoteControl_Data rx_buffer_2 = {0};

static sys_RemoteControl_Data *current_rx_buffer = 0;
static bool isNewDataAvailable = false;

#define NOT_STARTED -1
static sint8 receiving_bit = NOT_STARTED;

void Sys_Receive_RemoteControl_Msg(void);

inline void Sys_Init_RemoteControl(void){
    REMOTE_DIR = INPUT_PIN;	
    
    IPC0bits.INT0IP = SYS_IRQP_REMOTECONTROL;
    INTCON2bits.INT0EP = 1;   	//use falling edge
    IFS0bits.INT0IF = 0;		//clear to IRQ flag
    
    current_rx_buffer = &rx_buffer_1;
    
    if(!Sys_Register_IOHandler(Sys_Receive_RemoteControl_Msg)){// no error handling used
        return;
    }
}

inline void Sys_Start_RemoteControl(void){
	IFS0bits.INT0IF = 0;		///clear to IRQ flag
	IEC0bits.INT0IE = 1;		//enable external INT0	
}

#define WAIT_FOR_QUARTERBIT 4
#define WAIT_FOR_HALFBIT    8
#define WAIT_FOR_BIT        17
#define WAIT_INITIALLY      WAIT_FOR_BIT+WAIT_FOR_QUARTERBIT
void __attribute__((__interrupt__, auto_psv))  _INT0Interrupt(void){
    //When a message arrives deactivate interrupt -> is activated at the end of the message
	IEC0bits.INT0IE = 0;
        IFS0bits.INT0IF = 0;
	message_arriving = true;
        waiting_cycles = WAIT_INITIALLY;
	return;
}
//https://en.wikipedia.org/wiki/RC-5
void Sys_Receive_RemoteControl_Msg(){
    if(!message_arriving){
        return;
    }
    
    if(waiting_cycles > 0){
        waiting_cycles--;
        return;
    }

    uint8 value = REMOTE;
    BODY_LED = value;  
    if (receiving_bit == NOT_STARTED){
        if(value == 1){//this is only noise
            Sys_Start_AtomicSection();
            IEC0bits.INT0IE = 1;   	//enable interrupt from falling edge
            IFS0bits.INT0IF = 0;    //clear interrupt flag from first receive !
            message_arriving = false;
            waiting_cycles = WAIT_INITIALLY;
            Sys_End_AtomicSection();
            return;
        } else {
            waiting_cycles = WAIT_FOR_HALFBIT; //because active low -> wait at the first half of the bit !!
            receiving_bit=0;
            return;
        }
    }

    uint16 *pbuffer = (uint16 *) current_rx_buffer;

    if (receiving_bit < 13){
        Sys_Start_AtomicSection();
        
        *pbuffer <<= 1;
        *pbuffer += value;
        if(value != 0){

            LED5 = 1;
        }
        waiting_cycles = WAIT_FOR_BIT;

        receiving_bit++;
        Sys_End_AtomicSection();
        return;
    }

    Sys_Start_AtomicSection();
    LED7 = ~REMOTE;
    if(current_rx_buffer->data != 0){
        LED1 = 1;
    }else{
        LED1 = 0;
    }
    if(current_rx_buffer == &rx_buffer_1){//swap buffers
        current_rx_buffer = &rx_buffer_2;
    }else{
        current_rx_buffer = &rx_buffer_1;
    }

    IEC0bits.INT0IE = 1;
    IFS0bits.INT0IF = 0;

    message_arriving = false;
    receiving_bit = NOT_STARTED;
    waiting_cycles = 20;

    isNewDataAvailable = true;
    Sys_End_AtomicSection();
    LED0 = ~LED0;
}

bool Sys_HasRemoteC_Sent_New_Data() {
    return isNewDataAvailable;
}
uint8 Sys_RemoteC_Get_CheckBit() {
    if(current_rx_buffer == &rx_buffer_1){
        return rx_buffer_2.check;
    }else{
        return rx_buffer_1.check;
    }
}

uint8 Sys_RemoteC_Get_Address() {
    if(current_rx_buffer == &rx_buffer_1){
        return rx_buffer_2.address;
    }else{
        return rx_buffer_1.address;
    }
}

uint8 Sys_RemoteC_Get_Data() {
    
    isNewDataAvailable = false;
    if(current_rx_buffer == &rx_buffer_1){
        return rx_buffer_2.data;
    }else{
        return rx_buffer_1.data;
    }
}