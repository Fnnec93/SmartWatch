/*
 * drv_uart.c
 *
 *  Created on: 20 avr. 2024
 *
 */

#include "drv_uart.h"
#include "pcb.h"
#include "stm32f4xx_hal.h"
#include <string.h>



static void drv_uart_local_enableHdw(UART_HandleTypeDef* psHuart);
static void drv_uart_local_disableHdw(UART_HandleTypeDef* psHuart);
static void drv_uart_local_getInstance(USART_TypeDef** ppsInstance, UART_Number number);
static uint32_t drv_uart_local_getWordLength(Word_Length WordLength);
static uint32_t drv_uart_local_getStopBits(Stop_Bits StopBits);
static uint32_t drv_uart_local_getParity(Parity parity);
static uint32_t drv_uart_local_getMode(Mode mode);
static uint32_t drv_uart_local_getHwFlowCtl(Hw_Flow_Ctl HwFlowCtl);
static uint32_t drv_uart_local_getOverSampling(Over_Sampling overSampling);

static UART_HandleTypeDef sHuart;

// UART Init
void drv_uart_load(UART_Config* psUartConfig)
{
	USART_TypeDef* psInstance = NULL;
	// get usart instance
	drv_uart_local_getInstance(&psInstance, psUartConfig->USARTx);
	sHuart.Instance = psInstance;
	sHuart.Init.BaudRate = psUartConfig->BaudRate;
	sHuart.Init.WordLength = drv_uart_local_getWordLength(psUartConfig->WordLength);
	sHuart.Init.StopBits = drv_uart_local_getStopBits(psUartConfig->StopBits);
	sHuart.Init.Parity = drv_uart_local_getParity(psUartConfig->Parity);
	sHuart.Init.Mode = drv_uart_local_getMode(psUartConfig->Mode);
	sHuart.Init.HwFlowCtl = drv_uart_local_getHwFlowCtl(psUartConfig->HwFlowCtl);
	sHuart.Init.OverSampling = drv_uart_local_getOverSampling(psUartConfig->OverSampling);
}

// Ouvrir la communication UART
void drv_uart_open(void)
{ // retourne int
	drv_uart_local_enableHdw(&sHuart);
	if (HAL_UART_Init(&sHuart) != HAL_OK)
	{
		// error management
	}
}

// Fermer la communication UART
void drv_uart_close(void)
{
    if (HAL_UART_DeInit(&sHuart) != HAL_OK)
    {
        // error management
    }
    drv_uart_local_disableHdw(&sHuart);
}

// Uart Log
void drv_uart_send(const char* pMessage)
{
	HAL_UART_Transmit(&sHuart, (uint8_t*)pMessage, strlen(pMessage), HAL_MAX_DELAY);
}

// Uart receive
void drv_uart_receive(uint8_t* pMessage)
{
	HAL_UART_Receive(&sHuart, pMessage, sizeof(pMessage), HAL_MAX_DELAY);
}

static void drv_uart_local_enableHdw(UART_HandleTypeDef* psHuart)
{
	GPIO_InitTypeDef sGPIO_InitStruct = {0};
	if (psHuart->Instance == USART2)
	{
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		sGPIO_InitStruct.Pin = USART2_TX_PIN | USART2_RX_PIN;
		sGPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		sGPIO_InitStruct.Pull = GPIO_NOPULL;  // à revoir avec micipssa
		sGPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		sGPIO_InitStruct.Alternate = USART2_AF;  // CREUSER
		HAL_GPIO_Init(USART2_TX_PORT, &sGPIO_InitStruct);
	}
}

static void drv_uart_local_disableHdw(UART_HandleTypeDef* psHuart)
{
	if (psHuart->Instance == USART2)
	{
		__HAL_RCC_USART2_CLK_DISABLE();
		HAL_GPIO_DeInit(USART2_TX_PORT, USART2_TX_PIN|USART2_RX_PIN);
	}
}

static void drv_uart_local_getInstance(USART_TypeDef** ppsInstance, UART_Number number)
{
	switch (number)
	{
       case UART_1:
    	    *ppsInstance = USART1;
	        break;
	   case UART_2:
		    *ppsInstance = USART2;
	        break;
	   default:
		    *ppsInstance = NULL;
	        break;
	}
}


static uint32_t drv_uart_local_getWordLength(Word_Length WordLength)
{
	uint32_t ValueReturn = 0;
	switch(WordLength)
	{
	  case WORD_LENGTH_8B:
		 ValueReturn = UART_WORDLENGTH_8B;
		 break;
	  case WORD_LENGTH_9B:
		 ValueReturn = UART_WORDLENGTH_9B;
		 break;
	  default:
		  ValueReturn = 0;
	}
	return ValueReturn;
}

static uint32_t drv_uart_local_getStopBits(Stop_Bits StopBits)
{
	uint32_t ValueReturn = 0;

	switch(StopBits)
	{
	  case  STOP_BITS1:
		 ValueReturn = UART_STOPBITS_1;
		 break;
	  case  STOP_BITS2:
		 ValueReturn = UART_STOPBITS_2;
		 break;
	  default:
		  ValueReturn = 0;
	}
	return ValueReturn;
}

static uint32_t drv_uart_local_getParity(Parity parity)
{
	uint32_t ValueReturn = 0;

	switch(parity)
	{
	  case  PARITY_NONE:
		 ValueReturn = UART_PARITY_NONE;
		 break;
	  case  PARITY_EVEN:
		 ValueReturn = UART_PARITY_EVEN;
		 break;
	  case PARITY_ODD:
		  ValueReturn = UART_PARITY_ODD;
	  default:
		  ValueReturn = 0;
	}
	return ValueReturn;

}

static uint32_t drv_uart_local_getMode(Mode mode)
{
	uint32_t ValueReturn = 0;

    switch (mode)
    {
        case MODE_RX:
        	ValueReturn = UART_MODE_RX;
        	break;
        case MODE_TX:
        	ValueReturn = UART_MODE_TX;
        	break;
        case MODE_TX_RX:
        	ValueReturn = UART_MODE_TX_RX;
        	break;
        default:
        	ValueReturn = 0;
        	break;
    }
    return ValueReturn;
}

static uint32_t drv_uart_local_getHwFlowCtl(Hw_Flow_Ctl HwFlowCtl)
{
	uint32_t ValueReturn = 0;

    switch (HwFlowCtl)
    {
        case HWCONTROL_NONE:
        	ValueReturn = UART_HWCONTROL_NONE;
        	break;
        case HWCONTROL_RTS:
        	ValueReturn = UART_HWCONTROL_RTS;
        	break;
        case HWCONTROL_CTS:
        	ValueReturn = UART_HWCONTROL_CTS;
        	break;
        case HWCONTROL_RTS_CTS:
        	ValueReturn = UART_HWCONTROL_RTS_CTS;
        	break;
        default:
        	ValueReturn = 0;
        	break;
    }
    return ValueReturn;

}

static uint32_t drv_uart_local_getOverSampling(Over_Sampling overSampling)
{
	uint32_t ValueReturn = 0;

	switch(overSampling)
	{
	  case  OVERSAMPLING_16:
		 ValueReturn = UART_OVERSAMPLING_16;
		 break;
	  case  OVERSAMPLING_8:
		 ValueReturn = OVERSAMPLING_8;
		 break;
	  default:
		  ValueReturn = 0;
	}
	return ValueReturn;
}
