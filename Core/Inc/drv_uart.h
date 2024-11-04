/*
 * drv_uart.h
 *
 *  Created on: 20 avr. 2024
 *  Author: Lenovo
 */

#include <stdint.h>
#include "stm32f4xx_hal.h"

#ifndef INC_DRV_UART_H_
#define INC_DRV_UART_H_

typedef enum
{
    UART_1,
    UART_2,
} UART_Number;

typedef enum
{
	WORD_LENGTH_8B,
	WORD_LENGTH_9B,
}Word_Length;

typedef enum
{
	STOP_BITS1,
	STOP_BITS2,
}Stop_Bits;

typedef enum
{
	PARITY_NONE,
	PARITY_EVEN,
	PARITY_ODD,
}Parity;

typedef enum
{
	MODE_RX,
	MODE_TX,
	MODE_TX_RX,
}Mode;

typedef enum
{
	HWCONTROL_NONE,
	HWCONTROL_RTS,
	HWCONTROL_CTS,
	HWCONTROL_RTS_CTS,
}Hw_Flow_Ctl;

typedef enum
{
	OVERSAMPLING_16,
	OVERSAMPLING_8,
}Over_Sampling;

typedef struct
{
    UART_Number USARTx;
    uint32_t BaudRate;
    Word_Length WordLength;
    Stop_Bits StopBits;
    Parity Parity;
    Mode Mode;
    Hw_Flow_Ctl HwFlowCtl;
    Over_Sampling OverSampling;
} UART_Config;

void drv_uart_load(UART_Config* psUartConfig);
void drv_uart_open(void);
void drv_uart_close(void);
void drv_uart_send(const char* pMessage);
void drv_uart_receive(uint8_t* pMessage);

#endif /* INC_DRV_UART_H_ */
