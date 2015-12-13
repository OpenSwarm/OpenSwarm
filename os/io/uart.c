#include "uart.h"
#include "../platform/e-puck/system_IO_uart_HDI.h"
#include <stdlib.h>
#include <string.h>

#include "../definitions.h"

#include "../interrupts.h"
#include "../memory.h"

#define SYS_UART1_BAUDRATE 115000
#define SYS_UART2_BAUDRATE 115000

inline void Sys_Init_UART1(void){
    Sys_Init_UART1_HDI();
}

inline void Sys_Init_UART2(void){
    Sys_Init_UART2_HDI();
}

inline void Sys_Start_UART1(void){
    Sys_Start_UART1_HDI();
}

inline void Sys_Start_UART2(void){
    Sys_Start_UART2_HDI();
}

void Sys_SetReadingFunction_UART1(pUART_reader func){
    read_uart_1 = func;
}

void Sys_SetReadingFunction_UART2(pUART_reader func){
    read_uart_2 = func;
}

void Sys_Writeto_UART1(void *data, uint16 length){

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

void Sys_Writeto_UART2(void *data, uint16 length){

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
