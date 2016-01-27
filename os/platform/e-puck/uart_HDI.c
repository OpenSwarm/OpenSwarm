/*!
 * \file
 * \ingroup uart
 * \ingroup hdi
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * 
 * \version 1.0
 *
 * \date 27 August 2015
 * 
 * \brief  Hardware dependent implementations to transmit bytes via UART.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 * 
 */


#include "uart_HDI.h"

#include <p30F6014A.h>
#include <stdlib.h>
#include <string.h>

#include "library/motor_led/e_epuck_ports.h"
#include "../../interrupts.h"
#include "../../memory.h"

pUART_reader read_uart_1 = 0; /*!< pointer to the functions that processes incoming bytes from UART1*/
pUART_reader read_uart_2 = 0; /*!< pointer to the functions that processes incoming bytes from UART2*/

sys_uart_txdata *sys_UART1_TX_data = 0;  /*!< Linked list of messages that need to be sent via UART1*/
sys_uart_txdata *sys_UART2_TX_data = 0;  /*!< Linked list of messages that need to be sent via UART2*/

uint byte_counter_uart1 = 0;  /*!< Bytes that were written */
uint byte_counter_uart2 = 0;  /*!< Bytes that were written */

/**
 *
 * This function initialises UART1.
 * 
 */
void Sys_Init_UART1_HDI(void){
    UART1_RX_DIR = INPUT_PIN;
    UART1_TX_DIR = OUTPUT_PIN;

    IPC2bits.U1RXIP = SYS_IRQP_UART1;
    IPC2bits.U1TXIP = SYS_IRQP_UART1;


    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;//Transmission is enabled
    IFS0bits.U1TXIF = 0;
    IEC0bits.U1TXIE = 1;//Transmission is enabled

    
    U1MODE = 0;
    //U1MODEbits.PDSEL = 0b00;//8-bit; no parity
    //U1MODEbits.STSEL = 0;//1 stop-bit

    U1STA = 0;
    //U1STAbits.URXISEL = 0b00;//U1RXIF occurs after 1 or more bit were transfered into the buffer
    //U1STAbits.ADDEN = 0;//No ninth address bit
    //U1STAbits.UTXISEL = 1;//U1TXIF is set if byte was successfully transmitted
        
    U1BRG = (((FCY/SYS_UART1_BAUDRATE)/16)-1);
    
    U1MODEbits.UARTEN = 1; //enable UART
    U1STAbits.UTXEN = 1;



}

/**
 *
 * This function initialises UART2.
 * 
 */
void Sys_Init_UART2_HDI(void){
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

/**
 *
 * This function starts UART2.
 * 
 * @note When executed this function, bytes can be received or transmitted at any time.
 * 
 */
void Sys_Start_UART1_HDI(void){
    U1STAbits.UTXISEL = 0;//U1TXIF is set if byte was successfully transfered to the internal transmit buffer
    U1STAbits.UTXEN = 1;//Transmission is enabled

    U1STAbits.URXISEL = 0b00;//U1RXIF occurs after 1 or more bit were transfered into the buffer

    U1MODEbits.UARTEN = 1;
}

/**
 *
 * This function starts UART2.
 * 
 * @note When executed this function, bytes can be received or transmitted at any time.
 * 
 */
void Sys_Start_UART2_HDI(void){
    U2STAbits.UTXISEL = 0;//U1TXIF is set if byte was successfully transfered to the internal transmit buffer
    U2STAbits.UTXEN = 1;//Transmission is enabled

    U2STAbits.URXISEL = 0b00;//U1RXIF occurs after 1 or more bit were transfered into the buffer

    U2MODEbits.UARTEN = 1;
}

/**
 *
 * UART1 reading interrupt.
 * 
 */
void __attribute__((interrupt,auto_psv)) _U1RXInterrupt(void){
    Sys_Read_UART1_ISR();
    IFS0bits.U1RXIF = 0;
}

/**
 *
 * Alternative UART1 reading interrupt.
 * 
 */
void __attribute__((interrupt,auto_psv)) _AltU1RXInterrupt(void){
    Sys_Read_UART1_ISR();
    IFS0bits.U1RXIF = 0;
}

/**
 *
 * UART1 writing interrupt.
 * 
 */
void __attribute__((interrupt,auto_psv)) _U1TXInterrupt(void){
    Sys_Write_UART1_ISR();
    IFS0bits.U1TXIF = 0;
}
/**
 *
 * Alternative UART1 writing interrupt.
 * 
 */
void __attribute__((interrupt,auto_psv)) _AltU1TXInterrupt(void){
    Sys_Write_UART1_ISR();
    IFS0bits.U1TXIF = 0;
}

/**
 *
 * UART2 reading interrupt.
 * 
 */
void __attribute__((interrupt,auto_psv)) _U2RXInterrupt(void){
    Sys_Read_UART2_ISR();
    IFS1bits.U2RXIF = 0;
}
/**
 *
 * Alternative UART2 reading interrupt.
 * 
 */
void __attribute__((interrupt,auto_psv)) _AltU2RXInterrupt(void){
    Sys_Read_UART2_ISR();
    IFS1bits.U2RXIF = 0;
}

/**
 *
 * UART2 writing interrupt.
 * 
 */
void __attribute__((interrupt,auto_psv)) _U2TXInterrupt(void){
    Sys_Write_UART2_ISR();
    IFS1bits.U2TXIF = 0;
}
/**
 *
 * Alternative UART2 writing interrupt.
 * 
 */
void __attribute__((interrupt,auto_psv)) _AltU2TXInterrupt(void){
    Sys_Write_UART2_ISR();
    IFS1bits.U2TXIF = 0;
}

/**
 *
 * This function is executed at occurrence of the UART1 reading interrupt.
 * 
 */
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

/**
 *
 * This function is executed at occurrence of the UART1 writing interrupt.
 * 
 */
inline void Sys_Write_UART1_ISR(){
    if(sys_UART1_TX_data == 0){//nothing to send
        byte_counter_uart1 = 0;
        return;
    }

    while(U1STAbits.UTXBF == 0){//as long as the transmission buffer isn't full?

        if(byte_counter_uart1 < sys_UART1_TX_data->length){
            U1TXREG = sys_UART1_TX_data->data[byte_counter_uart1];//add new byte
            byte_counter_uart1++;
            continue;
        }

        Sys_Start_AtomicSection();

        byte_counter_uart1 = 0;
        sys_uart_txdata *element = sys_UART1_TX_data;

        sys_UART1_TX_data = sys_UART1_TX_data->next;
        element->next = 0;

        Sys_Free(element->data);
        Sys_Free(element);

        Sys_End_AtomicSection();

        if(sys_UART1_TX_data == 0){//it was the last msg
            return;
        }
    }
}

/**
 *
 * This function is executed at occurrence of the UART2 reading interrupt.
 * 
 */
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
/**
 *
 * This function is executed at occurrence of the UART1 writing interrupt.
 * 
 */
inline void Sys_Write_UART2_ISR(){
    while(U2STAbits.UTXBF == 0){//as long as the transmission buffer isn't full?

        if(sys_UART2_TX_data == 0){//it was the last msg
            byte_counter_uart2 = 0;
            return;
        }

        if(byte_counter_uart2 < sys_UART2_TX_data->length){
            U1TXREG = sys_UART2_TX_data->data[byte_counter_uart2];//add new byte
            byte_counter_uart2++;
            continue;
        }

        Sys_Start_AtomicSection();

        byte_counter_uart2 = 0;
        sys_uart_txdata *element = sys_UART2_TX_data;

        sys_UART2_TX_data = sys_UART2_TX_data->next;
        element->next = 0;

        Sys_Free(element->data);
        Sys_Free(element);

        Sys_End_AtomicSection();
    }
}
