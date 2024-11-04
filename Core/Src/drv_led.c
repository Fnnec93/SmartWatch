/*
 * drv_led.c
 *
 *  Created on: Apr 3, 2024
 *      Author: ASUS
 */


#include "drv_led.h"

typedef struct
{
	GPIO_TypeDef 	*psPortSelected  ;
	unsigned int 	pinSelected ;
	unsigned int	pinTypeOutput ;
}conf_led;
conf_led ConfLed;



static int drv_led_local_selectLed (const unsigned int idLed  );

int  drv_led_open (const led_idLed id ,const int unused )
{
	int status = 1;
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	status = drv_led_local_selectLed(id);
	if (status == 0)
	{
		GPIO_InitStruct.Pin = ConfLed.pinSelected;
		GPIO_InitStruct.Mode = LED_TYPE_OUTPUT_GREEN_1;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(ConfLed.psPortSelected, &GPIO_InitStruct);
	}
	return status;
}
int  drv_led_close (const led_idLed id ,const int unused )
{
	int status = 1;
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	status = drv_led_local_selectLed(id);
	if (0 == status)
	{
		GPIO_InitStruct.Pin = ConfLed.pinSelected;
		GPIO_InitStruct.Mode = ConfLed.pinTypeOutput;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(ConfLed.psPortSelected, &GPIO_InitStruct);
	}
	return status;
}


static int drv_led_local_selectLed (const unsigned int idLed )
{
	int status = 1;
	switch (idLed)
	{
	case LED_GREEN_1 :
		ConfLed.psPortSelected = LED_PORT_GREEN_1;
		ConfLed.pinSelected = (unsigned int)LED_GPIO_GREEN_1;
		ConfLed.pinTypeOutput = GPIO_MODE_OUTPUT_PP;

		status = 0;
		break;
	default:
		status = 1;
		break;
	}

	return status;
}

int drv_led_setStateLed (const led_idLed id ,const led_state state )
{
	int status = 1;
	status = drv_led_local_selectLed(id);
	//TODO : encapsuler la prochaine fonction dans drv_gpio_write_pin
	if ( 0 == status)
	{
		HAL_GPIO_WritePin(ConfLed.psPortSelected, ConfLed.pinSelected, state);
	}
	return status;
}
