/*!
 * \file
 * \ingroup uart
 * 
 * \author  Stefan M. Trenkwalder <s.trenkwalder@openswarm.org>
 * \version 1.0
 *
 * \date 27 August 2015
 * 
 * \brief It declares functions to transmit bytes via UART.
 * \copyright 	adapted FreeBSD License (see http://openswarm.org/license)
 */

#include "uart.h"
#include "uart_HDI.h"

#include "../../definitions.h"

#include "../../interrupts.h"
#include "../../memory.h"

#define SYS_UART1_BAUDRATE 115000 	/*!< Baudrate for UART 1 (bits/s)*/
#define SYS_UART2_BAUDRATE 115000 	/*!< Baudrate for UART 2 (bits/s)*/


/**
 *
 * This function initialises UART1.
 * 
 */
inline void Sys_Init_UART1(void){
    Sys_Init_UART1_HDI();
}

/**
 *
 * This function initialises UART2.
 * 
 */
inline void Sys_Init_UART2(void){
    Sys_Init_UART2_HDI();
}

/**
 *
 * This function starts UART1.
 * 
 * @note When executed this function, bytes can be received or transmitted at any time.
 * 
 */
inline void Sys_Start_UART1(void){
    Sys_Start_UART1_HDI();
}

/**
 *
 * This function starts UART2.
 * 
 * @note When executed this function, bytes can be received or transmitted at any time.
 * 
 */
inline void Sys_Start_UART2(void){
    Sys_Start_UART2_HDI();
}

/**
 *
 * defines a function that processes received bytes (UART1). This defined callback function is only executed once by arrival of one byte.
 * 
 * @param[in] func  pointer to the function that should process the received byte(s).
 */
void Sys_SetReadingFunction_UART1(pUART_reader func){
    read_uart_1 = func;
}

/**
 *
 * defines a function that processes received bytes (UART2). This defined callback function is only executed once by arrival of one byte.
 * 
 * @param[in] func  pointer to the function that should process the received byte(s).
 */
void Sys_SetReadingFunction_UART2(pUART_reader func){
    read_uart_2 = func;
}

/**
 *
 * This function writes sequentially the bytes on the UART1.
 * 
 * @note The data will be put into a queue, where it will be sent as soon as the UART is idle
 * 
 * @param[in] data  pointer to the bytes that should be transmitted.
 * @param[in] length  number of bytes to send.
 */
void Sys_Writeto_UART1(void *data, uint length){

    sys_uart_txdata *element = Sys_Malloc(sizeof(sys_uart_txdata));
    if(element == 0){//not enough memory
        return;
    }

    element->data = Sys_Malloc(length);
    if(element->data == 0){//not enough memory
        Sys_Free(element);
        return;
    }
    Sys_Memcpy(data,element->data,length);
    
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

/**
 *
 * This function writes sequentially the bytes on the UART2. 
 * 
 * @note The data will be put into a queue, where it will be sent as soon as the UART is idle
 * 
 * @param[in] data  pointer to the bytes that should be transmitted.
 * @param[in] length  number of bytes to send.
 */
void Sys_Writeto_UART2(void *data, uint length){

    sys_uart_txdata *element = Sys_Malloc(sizeof(sys_uart_txdata));
    if(element == 0){//not enough memory
        return;
    }

    element->data = Sys_Malloc(length);
    if(element->data == 0){//not enough memory
        Sys_Free(element);
        return;
    }
    Sys_Memcpy((uint8 *) data,element->data,length);

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
