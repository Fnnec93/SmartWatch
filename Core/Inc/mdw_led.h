

#ifndef __LED_H
#define __LED_H


#include <stdio.h>
#include <stdbool.h>

#include "drv_led.h"

#include <stm32f4xx_hal.h>



//TODO  : si je rajoute cette include en desosus, ça ne compilera pas
//#include "stm32f4xx_hal_gpio.h"


//TODO  je les garde pour une futur utilisation possible ...
//typedef enum {
//	LED_CONF_OPEN_DRAIN = 1,
//	LED_CONF_PUSH_PULL  = 2,
//}LED_MODE;
//
//typedef struct {
//
//LED_MODE ledMode;
//
//}Led_configuration;


int 	mdw_led_load  		(void);
int 	mdw_led_unload  	(void);

int 	mdw_led_open		(unsigned int idAll);
int 	mdw_led_close		(unsigned int idAll);
int 	mdw_led_set 		(const led_idLed id ,const led_state state,  int notUse);
//int 	led_togleValue	(led_IsLed id , unsigned int frequence);

#endif





