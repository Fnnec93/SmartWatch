/*
 * drv_led.h
 *
 *  Created on: Apr 3, 2024
 *      Author: ASUS
 */

#ifndef INC_DRV_LED_H_
#define INC_DRV_LED_H_


#include "stm32f4xx_hal.h"
#include "pcb.h"


typedef enum
{
	LED_GREEN_1 		=1,
	LED_NUMBER_MAX  	=1
}led_idLed;

typedef enum
{
	LED_OFF 	= GPIO_PIN_RESET,
	LED_ON  	= GPIO_PIN_SET
}led_state;


int  drv_led_open (const led_idLed id ,const int unused );
int  drv_led_close (const led_idLed id ,const int unused );

int  drv_led_setStateLed (const led_idLed id ,const led_state state );


#endif /* INC_DRV_LED_H_ */
