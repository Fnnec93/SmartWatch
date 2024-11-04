#include "mdw_log.h"
#include "drv_uart.h"
#include <stdio.h>
#include <stdbool.h>

#define BUFFER_MESSAGE_SIZE                    256

typedef struct {
    bool isLoaded;
    bool isOpen;
} Log_StatusGlobal;


static Log_StatusGlobal sLogStatus;
static UART_Config sUartConfig =
{
    .USARTx = UART_2,
    .BaudRate = 115200,
    .WordLength = WORD_LENGTH_8B,
    .StopBits = STOP_BITS1,
    .Parity = PARITY_NONE,
    .Mode = MODE_TX_RX,
    .HwFlowCtl = HWCONTROL_NONE,
    .OverSampling = OVERSAMPLING_16
};

// Functions definition

void  mdw_log_load(void)
{
	if (false == sLogStatus.isLoaded)
	{
		drv_uart_load(&sUartConfig);
		sLogStatus.isLoaded = true;
	}
}

void mdw_log_open(void)
{
	if((true== sLogStatus.isLoaded) && (false == sLogStatus.isOpen))
	{
		drv_uart_open();
		sLogStatus.isOpen = true;
	}
}

void mdw_log_close(void)
{
	if(true == sLogStatus.isOpen)
	{
		drv_uart_close();
		sLogStatus.isOpen = false;
	}
}

// Sys log function
void sys_log(SysLogLevel level, const char* pMessage)
{
    if(true == sLogStatus.isOpen)
	{
        // buffer that contain level and message
		char buffer[BUFFER_MESSAGE_SIZE];
		// String
		const char* pLevel = "";
		switch (level)
		{
            case SYS_LOG_ERROR:
               pLevel = "ERROR";
               break;
            case SYS_LOG_WARNING: 
               pLevel = "WARNING";
               break;
            case SYS_LOG_INFO: 
               pLevel = "INFO";
               break;
            case SYS_LOG_DEBUG: 
               pLevel = "DEBUG";
               break;
        }
        if(snprintf(buffer, sizeof(buffer), "%s: %s\r\n", pLevel, pMessage)<0)
        {
            buffer[0]= '\0';
        }
        drv_uart_send(buffer);
    }
}
