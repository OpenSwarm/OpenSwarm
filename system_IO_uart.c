#include "system_IO_uart.h"
#include <p30F6014A.h>
#include <stdlib.h>
#include <string.h>

#include "HDI_epuck_ports.h"
#include "system_Interrupts.h"

#define SYS_UART1_BAUDRATE 115000
#define SYS_UART2_BAUDRATE 115000

static pUART_reader read_uart_1 = 0;
static pUART_reader read_uart_2 = 0;

inline void Sys_Read_UART1_ISR();
inline void Sys_Write_UART1_ISR();
inline void Sys_Read_UART2_ISR();
inline void Sys_Write_UART2_ISR();

typedef struct uart_tx_data_s{
    uint8 *data;
    uint16 length;

    struct uart_tx_data_s *next;
}sys_uart_txdata;

sys_uart_txdata *sys_UART1_TX_data = 0;
sys_uart_txdata *sys_UART2_TX_data = 0;


void Sys_Init_UART1(void){
    UART1_RX_DIR = INPUT_PIN;
    UART1_TX_DIR = OUTPUT_PIN;

    U1BRG = (((FCY/SYS_UART1_BAUDRATE)/16)-1);

    U1MODEbits.PDSEL = 0b00;//8-bit; no parity
    U1MODEbits.STSEL = 0;//1 stop-bit

    IPC2bits.U1RXIP = SYS_IRQP_UART1;
    IPC2bits.U1TXIP = SYS_IRQP_UART1;

    U1STAbits.UTXISEL = 1;//U1TXIF is set if byte was successfully transmitted
    U1STAbits.UTXEN = 1;//Transmission is enabled

    U1STAbits.URXISEL = 0b00;//U1RXIF occurs after 1 or more bit were transfered into the buffer

    U1STAbits.ADDEN = 0;//No ninth address bit
}

void Sys_Init_UART2(void){
    UART2_RX_DIR = INPUT_PIN;
    UART2_TX_DIR = OUTPUT_PIN;

    U2BRG = (((FCY/SYS_UART2_BAUDRATE)/16)-1);

    U2MODEbits.PDSEL = 0b00;//8-bit; no parity
    U2MODEbits.STSEL = 0;//1 stop-bit
    U2STAbits.ADDEN = 0;//No ninth address bit

    IPC6bits.U2RXIP = SYS_IRQP_UART2;
    IPC6bits.U2TXIP = SYS_IRQP_UART2;

    U2STAbits.UTXISEL = 1;//U1TXIF is set if byte was successfully transfered to the internal transmit buffer
    U2STAbits.UTXEN = 1;//Transmission is enabled

    U2STAbits.URXISEL = 0b00;//U1RXIF occurs after 1 or more bit were transfered into the buffer

}

void Sys_Start_UART1(void){
    U1STAbits.UTXISEL = 0;//U1TXIF is set if byte was successfully transfered to the internal transmit buffer
    U1STAbits.UTXEN = 1;//Transmission is enabled

    U1STAbits.URXISEL = 0b00;//U1RXIF occurs after 1 or more bit were transfered into the buffer

    U1MODEbits.UARTEN = 1;
}

void Sys_Start_UART2(void){
    U2STAbits.UTXISEL = 0;//U1TXIF is set if byte was successfully transfered to the internal transmit buffer
    U2STAbits.UTXEN = 1;//Transmission is enabled

    U2STAbits.URXISEL = 0b00;//U1RXIF occurs after 1 or more bit were transfered into the buffer

    U2MODEbits.UARTEN = 1;
}

void Sys_SetReadingFunction_UART1(pUART_reader func){
    read_uart_1 = func;
}

void Sys_SetReadingFunction_UART2(pUART_reader func){
    read_uart_2 = func;
}

void Sys_Writeto_UART1(uint8 *data, uint16 length){

    sys_uart_txdata *element = malloc(sizeof(sys_uart_txdata));
    if(element == 0){//not enough memory
        return;
    }

    element->data = malloc(length);
    if(element->data == 0){//not enough memory
        free(element);
        return;
    }
    memcpy(data,element->data,length);
    
    element->length = length;
    element->next = 0;

    if(sys_UART1_TX_data == 0){//root
        sys_UART1_TX_data = element;
        Sys_Write_UART1_ISR();
        return;
    }

    sys_uart_txdata *list = sys_UART1_TX_data;
    while(list->next != 0){
        list = list->next;
    }
    list->next = element;
}

void Sys_Writeto_UART2(uint8 *data, uint16 length){

    sys_uart_txdata *element = malloc(sizeof(sys_uart_txdata));
    if(element == 0){//not enough memory
        free(data);
        return;
    }

    element->data = data;
    element->length = length;
    element->next = 0;

    if(sys_UART2_TX_data == 0){//root
        sys_UART2_TX_data = element;
        Sys_Write_UART2_ISR();
        return;
    }

    sys_uart_txdata *list = sys_UART2_TX_data;
    while(list->next != 0){
        list = list->next;
    }
    list->next = element;

}

void __attribute__((interrupt,auto_psv)) _U1RXInterrupt(void){
    Sys_Read_UART1_ISR();
}
void __attribute__((interrupt,auto_psv)) _AltU1RXInterrupt(void){
    Sys_Read_UART1_ISR();
}

void __attribute__((interrupt,auto_psv)) _U1TXInterrupt(void){
    Sys_Write_UART1_ISR();
}
void __attribute__((interrupt,auto_psv)) _AltU1TXInterrupt(void){
    Sys_Write_UART1_ISR();
}

void __attribute__((interrupt,auto_psv)) _U2RXInterrupt(void){
    Sys_Read_UART2_ISR();
}
void __attribute__((interrupt,auto_psv)) _AltU2RXInterrupt(void){
    Sys_Read_UART2_ISR();
}

void __attribute__((interrupt,auto_psv)) _U2TXInterrupt(void){
    Sys_Write_UART2_ISR();
}
void __attribute__((interrupt,auto_psv)) _AltU2TXInterrupt(void){
    Sys_Write_UART2_ISR();
}

inline void Sys_Read_UART1_ISR(){
    uint8 data;
    
    if(U1STAbits.OERR == 1){//Buffer full?
        U1STAbits.OERR = 0;//I will empty it now
    }
    
    while(U1STAbits.URXDA == 1){//available data
        data = U1RXREG & 0x00FF;
        if(read_uart_1 != 0){
            read_uart_1(data);
        }
    }
}
inline void Sys_Write_UART1_ISR(){

    static uint16 byte_counter = 0;

    if(sys_UART1_TX_data == 0){//nothing to send
        return;
    }

    while(U1STAbits.UTXBF == 0){//as long as the transmission buffer isn't full?
        if(byte_counter == sys_UART1_TX_data->length){//transmitted the last byte
            byte_counter = 0;
            sys_uart_txdata *element = sys_UART1_TX_data;

            sys_UART1_TX_data = sys_UART1_TX_data->next;
            element->next = 0;

            free(element->data);
            free(element);

            if(sys_UART1_TX_data == 0){//it was the last msg
                return;
            }
        }

        U1TXREG = sys_UART1_TX_data->data[byte_counter++];//add new byte

    }

}
inline void Sys_Read_UART2_ISR(){
    uint8 data;

    if(U2STAbits.OERR == 1){//Buffer full?
        U2STAbits.OERR = 0;//I will empty it now
    }

    while(U2STAbits.URXDA == 1){//available data
        data = U2RXREG & 0x00FF;
        if(read_uart_2 != 0){
            read_uart_2(data);
        }
    }

}
inline void Sys_Write_UART2_ISR(){

    static uint16 byte_counter = 0;

    if(sys_UART2_TX_data == 0){//nothing to send
        return;
    }

    while(U2STAbits.UTXBF == 0){//as long as the transmission buffer isn't full?
        if(byte_counter == sys_UART2_TX_data->length){//transmitted the last byte
            byte_counter = 0;
            sys_uart_txdata *element = sys_UART2_TX_data;

            sys_UART2_TX_data = sys_UART2_TX_data->next;
            element->next = 0;

            free(element->data);
            free(element);

            if(sys_UART2_TX_data == 0){//it was the last msg
                return;
            }
        }

        U2TXREG = sys_UART2_TX_data->data[byte_counter++];//add new byte

    }
}
