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

#ifndef SYSTEM_IO_UART_HDI_H
#define	SYSTEM_IO_UART_HDI_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../../definitions.h"

#define UART1_RX _RF2  /*!< Used port on the microcontroller to read from the UART1 */
#define UART1_TX _RF3 /*!< Used port on the microcontroller to  write on the UART1 */
#define UART2_RX _RF4  /*!< Used port on the microcontroller to read from the UART2 */
#define UART2_TX _RF5  /*!< Used port on the microcontroller to write on the UART2 */

#define UART1_RX_DIR _TRISF2  /*!< direction of the used port on the microcontroller (reading from the UART1) */
#define UART1_TX_DIR _TRISF3 /*!< direction of the used port on the microcontroller (writing from the UART1) */
#define UART2_RX_DIR _TRISF4 /*!< direction of the used port on the microcontroller (reading from the UART2) */
#define UART2_TX_DIR _TRISF5 /*!< direction of the used port on the microcontroller (writing from the UART2) */
    
#define SYS_UART1_BAUDRATE 115000 /*!< Baud rate for UART1 */
#define SYS_UART2_BAUDRATE 115000 /*!< Baud rate for UART2 */
    
/**
 * @brief Linked list element to store transmission data
 *
 * It is a single linked list containing a set of bytes that should be sent via UART.
 *
 */
typedef struct sys_uart_tx_data_s{
    uint8 *data;/*!< pointer to bytes that should be sent */
    uint length;/*!< number of bytes that need to be sent */

    struct sys_uart_tx_data_s *next;/*!< pointer to the next element in the list */
}sys_uart_txdata;

extern sys_uart_txdata *sys_UART1_TX_data;
extern sys_uart_txdata *sys_UART2_TX_data;

extern uint byte_counter_uart1;
extern uint byte_counter_uart2;

extern pUART_reader read_uart_1;
extern pUART_reader read_uart_2;

void Sys_Init_UART1_HDI(void);
void Sys_Init_UART2_HDI(void);

void Sys_Start_UART1_HDI(void);
void Sys_Start_UART2_HDI(void);

inline void Sys_Read_UART1_ISR();
inline void Sys_Write_UART1_ISR();
inline void Sys_Read_UART2_ISR();
inline void Sys_Write_UART2_ISR();

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_IO_UART_H */

