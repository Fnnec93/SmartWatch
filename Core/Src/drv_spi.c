/*
 * drv_spi.c
 *
 *  Created on: May 25, 2024
 *      Author: Lenovo
 */


#include "drv_spi.h"
#include "pcb.h"
#include "stm32f4xx_hal.h"



static SPI_HandleTypeDef sHSpi;

static void drv_spi_local_enableHdw(SPI_HandleTypeDef* psHSpi);
static void drv_spi_local_disableHdw(SPI_HandleTypeDef* psHSpi);
static void drv_spi_local_getInstance(SPI_TypeDef** ppsInstance, SPI_Number number);
static uint32_t drv_spi_local_getMode(SPI_Mode mode);
static uint32_t drv_spi_local_getDirection(SPI_Direction direction);
static uint32_t drv_spi_local_getDataSize(SPI_DataSize dataSize);
static uint32_t drv_spi_local_getCLKPolarity(SPI_CLKPolarity clkPolarity);
static uint32_t drv_spi_local_getCLKPhase(SPI_CLKPhase clkPhase);
static uint32_t drv_spi_local_getNSS(SPI_NSS nss);
static uint32_t drv_spi_local_getFirstBit(SPI_FirstBit firstBit);
static uint32_t drv_spi_local_getTIMode(SPI_TIMode tiMode);
static uint32_t drv_spi_local_getCRCCalculation(SPI_CRCCalculation crcCalculation);


// SPI Init
void drv_spi_load(SPI_Config* psSpiConfig)
{
    SPI_TypeDef* psInstance = NULL;
    drv_spi_local_getInstance(&psInstance, psSpiConfig->SPIx);
    sHSpi.Instance = psInstance;
    sHSpi.Init.Mode = drv_spi_local_getMode(psSpiConfig->Mode);
    sHSpi.Init.Direction = drv_spi_local_getDirection(psSpiConfig->Direction);
    sHSpi.Init.DataSize = drv_spi_local_getDataSize(psSpiConfig->DataSize);
    sHSpi.Init.CLKPolarity = drv_spi_local_getCLKPolarity(psSpiConfig->CLKPolarity);
    sHSpi.Init.CLKPhase = drv_spi_local_getCLKPhase(psSpiConfig->CLKPhase);
    sHSpi.Init.NSS = drv_spi_local_getNSS(psSpiConfig->NSS);
    sHSpi.Init.BaudRatePrescaler = psSpiConfig->BaudRatePrescaler;
    sHSpi.Init.FirstBit = drv_spi_local_getFirstBit(psSpiConfig->FirstBit);
    sHSpi.Init.TIMode = drv_spi_local_getTIMode(psSpiConfig->TIMode);
    sHSpi.Init.CRCCalculation = drv_spi_local_getCRCCalculation(psSpiConfig->CRCCalculation);
    sHSpi.Init.CRCPolynomial = psSpiConfig->CRCPolynomial;
}

// Open SPI communication
void drv_spi_open(void)
{
    drv_spi_local_enableHdw(&sHSpi);
    if (HAL_SPI_Init(&sHSpi) != HAL_OK)
    {
        // error management
    }
}

void drv_spi_close(void)
{
    if (HAL_SPI_DeInit(&sHSpi) != HAL_OK)
    {
        // error management
    }
    drv_spi_local_disableHdw(&sHSpi);
}

int drv_spi_transmit(uint8_t *pData, uint16_t Size)
{
	int status = 1;

    if (HAL_OK == HAL_SPI_Transmit(&sHSpi, pData, Size, HAL_MAX_DELAY))
    {
    	// transmission succeed
    	status = 0;
    }

    return status;
}

int drv_spi_receive(uint8_t *pData, uint16_t Size)
{
	int status = 1;

    if (HAL_OK == HAL_SPI_Receive(&sHSpi, pData, Size, HAL_MAX_DELAY))
    {
    	// transmission succeed
    	status = 0;
    }

    return status;
}

// Fonction de transmission-réception combinée
int drv_spi_transmit_receive(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
    int status = 1;  // Échec par défaut

    // Utilisation de la fonction HAL_SPI_TransmitReceive pour un transfert full-duplex
    if (HAL_OK == HAL_SPI_TransmitReceive(&sHSpi, pTxData, pRxData, Size, HAL_MAX_DELAY))
    {
        status = 0;  // Succès
    }

    return status;
}


void drv_spi_unselect(void)
{
    HAL_GPIO_WritePin(SPI2_PORT, SPI2_CS, GPIO_PIN_SET); // CS doit etre en niveau haut
}

void drv_spi_select(void)
{
    HAL_GPIO_WritePin(SPI2_PORT, SPI2_CS, GPIO_PIN_RESET); // CS doit etre en niveau bas
}

static void drv_spi_local_enableHdw(SPI_HandleTypeDef* psHSpi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(psHSpi->Instance == SPI2)
    {
        __HAL_RCC_SPI2_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_13;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}

static void drv_spi_local_disableHdw(SPI_HandleTypeDef* psHSpi)
{
    if(psHSpi->Instance == SPI2)
    {
        __HAL_RCC_SPI2_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_13);
    }
}

static void drv_spi_local_getInstance(SPI_TypeDef** ppsInstance, SPI_Number number)
{
	switch (number)
	{
       case SPI_2:
    	    *ppsInstance = SPI2;
	        break;
	   case SPI_3:
		    *ppsInstance = SPI3;
	        break;
	   default:
		    *ppsInstance = NULL;
	        break;
	}
}

static uint32_t drv_spi_local_getMode(SPI_Mode mode)
{
	uint32_t ValueReturn = 0;

    switch (mode)
    {
        case MODE_MASTER:
        	ValueReturn = SPI_MODE_MASTER;
        	break;
        case MODE_SLAVE:
        	ValueReturn = SPI_MODE_SLAVE;
        	break;
        default:
            ValueReturn = 0;
            break;
    }

    return ValueReturn;
}

static uint32_t drv_spi_local_getDirection(SPI_Direction direction)
{
	uint32_t ValueReturn = 0;

    switch (direction)
    {
        case DIRECTION_2LINES:
            ValueReturn = SPI_DIRECTION_2LINES;
            break;
        case DIRECTION_2LINES_RXONLY:
            ValueReturn = SPI_DIRECTION_2LINES_RXONLY;
            break;
        case DIRECTION_1LINE:
        	ValueReturn = SPI_DIRECTION_1LINE;
        	break;
        default:
        	ValueReturn = 0;
        	break;
    }

    return ValueReturn;
}

static uint32_t drv_spi_local_getDataSize(SPI_DataSize dataSize)
{
	uint32_t ValueReturn = 0;

    switch (dataSize)
    {
        case DATASIZE_8BIT:
        	ValueReturn = SPI_DATASIZE_8BIT;
        	break;
        case DATASIZE_16BIT:
        	ValueReturn = SPI_DATASIZE_16BIT;
        	break;
        default:
        	ValueReturn = 0;
        	break;
    }

    return ValueReturn;
}

static uint32_t drv_spi_local_getCLKPolarity(SPI_CLKPolarity clkPolarity)
{
	uint32_t ValueReturn = 0;

    switch (clkPolarity)
    {
        case POLARITY_LOW:
        	ValueReturn = SPI_POLARITY_LOW;
        	break;
        case POLARITY_HIGH:
        	ValueReturn = SPI_POLARITY_HIGH;
        	break;
        default:
        	ValueReturn = 0;
        	break;
    }

    return ValueReturn;
}

static uint32_t drv_spi_local_getCLKPhase(SPI_CLKPhase clkPhase)
{
	uint32_t ValueReturn = 0;

    switch (clkPhase)
    {
        case PHASE_1EDGE:
        	ValueReturn = SPI_PHASE_1EDGE;
        	break;
        case PHASE_2EDGE:
        	ValueReturn = SPI_PHASE_2EDGE;
        	break;
        default:
        	ValueReturn = 0;
        	break;
    }

    return ValueReturn;
}

static uint32_t drv_spi_local_getNSS(SPI_NSS nss)
{
	uint32_t ValueReturn = 0;

    switch (nss)
    {
        case NSS_SOFT:
        	ValueReturn = SPI_NSS_SOFT;
        	break;
        case NSS_HARD_INPUT:
        	ValueReturn = SPI_NSS_HARD_INPUT;
        	break;
        case NSS_HARD_OUTPUT:
        	ValueReturn = SPI_NSS_HARD_OUTPUT;
        	break;
        default:
        	ValueReturn = 0;
        	break;
    }
    return ValueReturn;
}

static uint32_t drv_spi_local_getFirstBit(SPI_FirstBit firstBit)
{
	uint32_t ValueReturn = 0;

    switch (firstBit)
    {
        case FIRSTBIT_MSB:
        	ValueReturn = SPI_FIRSTBIT_MSB;
        	break;
        case FIRSTBIT_LSB:
        	ValueReturn = SPI_FIRSTBIT_LSB;
        	break;
        default:
        	ValueReturn = 0;
        	break;
    }
    return ValueReturn;
}

static uint32_t drv_spi_local_getTIMode(SPI_TIMode tiMode)
{
	uint32_t ValueReturn = 0;

    switch (tiMode)
    {
        case TIMODE_DISABLE:
        	ValueReturn = SPI_TIMODE_DISABLE;
        	break;
        case TIMODE_ENABLE:
        	ValueReturn = SPI_TIMODE_ENABLE;
        	break;
        default:
        	ValueReturn = 0;
        	break;
    }
    return ValueReturn;
}

static uint32_t drv_spi_local_getCRCCalculation(SPI_CRCCalculation crcCalculation)
{
	uint32_t ValueReturn = 0;

    switch (crcCalculation)
    {
        case CRCCALCULATION_DISABLE:
        	ValueReturn = SPI_CRCCALCULATION_DISABLE;
        	break;
        case CRCCALCULATION_ENABLE:
        	ValueReturn = SPI_CRCCALCULATION_ENABLE;
        	break;
        default:
        	ValueReturn = 0;
        	break;
    }
    return ValueReturn;
}


