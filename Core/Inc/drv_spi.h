/*
 * drv_spi.h
 *
 *  Created on: May 25, 2024
 *      Author: Lenovo
 */

#include <stdint.h>
#include "stm32f4xx_hal.h"

#ifndef INC_DRV_SPI_H_
#define INC_DRV_SPI_H_

typedef enum
{
    SPI_2,
    SPI_3,
} SPI_Number;

typedef enum
{
    MODE_MASTER,
    MODE_SLAVE,
} SPI_Mode;

typedef enum
{
    DIRECTION_2LINES,
    DIRECTION_2LINES_RXONLY,
    DIRECTION_1LINE,
} SPI_Direction;

typedef enum
{
    DATASIZE_8BIT,
    DATASIZE_16BIT,
} SPI_DataSize;

typedef enum
{
    POLARITY_LOW,
    POLARITY_HIGH,
} SPI_CLKPolarity;

typedef enum
{
    PHASE_1EDGE,
    PHASE_2EDGE,
} SPI_CLKPhase;

typedef enum
{
    NSS_SOFT,
    NSS_HARD_INPUT,
    NSS_HARD_OUTPUT,
} SPI_NSS;

typedef enum
{
    FIRSTBIT_MSB,
    FIRSTBIT_LSB,
} SPI_FirstBit;

typedef enum
{
    TIMODE_DISABLE,
    TIMODE_ENABLE,
} SPI_TIMode;

typedef enum
{
    CRCCALCULATION_DISABLE,
    CRCCALCULATION_ENABLE,
} SPI_CRCCalculation;

typedef struct
{
    SPI_Number SPIx;
    SPI_Mode Mode;
    SPI_Direction Direction;
    SPI_DataSize DataSize;
    SPI_CLKPolarity CLKPolarity;
    SPI_CLKPhase CLKPhase;
    SPI_NSS NSS;
    uint32_t BaudRatePrescaler;
    SPI_FirstBit FirstBit;
    SPI_TIMode TIMode;
    SPI_CRCCalculation CRCCalculation;
    uint32_t CRCPolynomial;
} SPI_Config;

// SPI Init
void drv_spi_load(SPI_Config* psSpiConfig);
// Open SPI communication
void drv_spi_open(void);
// Close SPI communication
void drv_spi_close(void);
// SPI Transmit
int drv_spi_transmit(uint8_t *pData, uint16_t Size);
// SPI Receive
int drv_spi_receive(uint8_t *pData, uint16_t Size);
// Fonction de transmission-réception combinée
int drv_spi_transmit_receive(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
// Select CS
void drv_spi_select(void);
// Unselect CS
void drv_spi_unselect(void);


#endif /* INC_DRV_SPI_H_ */
