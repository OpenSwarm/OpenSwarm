#include "system_IO_remoteControl.h"
#include "HDI_epuck_ports.h"

#include <stdbool.h>
#include "system_IO.h"

static bool message_arriving = 0;
static uint8 waiting_cycles = 20;

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

void Sys_Receive_RemoteControl_Msg(void);

inline void Sys_Init_RemoteControl(void){
    REMOTE_DIR = INPUT_PIN;	
	INTCON2bits.INT0EP = 1;   	//use falling edge
	IFS0bits.INT0IF = 0;		//clear to IRQ flag
    
    current_rx_buffer = &rx_buffer_1;
    
    if(!Sys_Register_IOHandler(Sys_Receive_RemoteControl_Msg)){// no error handling used
        return;
    }
	return;
}

inline void Sys_Start_RemoteControl(void){
	IFS0bits.INT0IF = 0;		///clear to IRQ flag
	IEC0bits.INT0IE = 1;		//enable external INT0	
}


void __attribute__((__interrupt__, auto_psv))  _INT0Interrupt(void){
    //When a message arrives deactivate interrupt -> is activated at the end of the message
	IEC0bits.INT0IE = 0;   			
	message_arriving = true;
    waiting_cycles = 20;
	return;
}

#define NOT_STARTED -1
void Sys_Receive_RemoteControl_Msg(){
    
    if(!message_arriving){
        return;
    }
    
    static uint8 bit_receiving = NOT_STARTED;
    
    if(waiting_cycles != 0){
        waiting_cycles--;
        return;
    }
    
    if (bit_receiving == NOT_STARTED){
		if(REMOTE){//this is only noise
				IEC0bits.INT0IE = 1;   	//enable interrupt from falling edge
				IFS0bits.INT0IF = 0;    //clear interrupt flag from first receive !	
                message_arriving = false;
                return;
		} else {
				waiting_cycles = 10; //wait in total 3ms
				bit_receiving=0;
                return;
		}
	} 	
	
    uint16 *pbuffer = (uint16 *) current_rx_buffer;
	if (bit_receiving < 13){
		*pbuffer <<= 1;
        *pbuffer += REMOTE;
        waiting_cycles = 18;
	} else{
        
        if(current_rx_buffer == &rx_buffer_1){
            current_rx_buffer = &rx_buffer_2;
        }else{
            current_rx_buffer = &rx_buffer_1;
        }
        
        IEC0bits.INT0IE = 1;   	//enable interrupt from falling edge
		IFS0bits.INT0IF = 0;    //clear interrupt flag from first receive !
        message_arriving = false;
        waiting_cycles = 20;
        
        isNewDataAvailable = true;
        bit_receiving = NOT_STARTED;
        
        LED6 = ~LED6;
    }
	
	bit_receiving++;
    
    
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