



#include <string.h>
#include <stdbool.h>

#include "drv_led.h"
#include "pcb.h"

#include "mdw_led.h"


typedef struct  {
bool isLoad;
bool isOpen;
}Led_StatusGlobal;

Led_StatusGlobal sLedStatus;

int mdw_led_load  	(void)
{
	int status = 0;
	memset(&sLedStatus,0,sizeof(sLedStatus));
	sLedStatus.isLoad = true;
	return status;
}
int mdw_led_unload  	(void)
{
	int status = 1;
	if (false==sLedStatus.isOpen )
	{
		memset(&sLedStatus,0,sizeof(sLedStatus));
		sLedStatus.isLoad = false;
		status = 0 ;
	}
	return status;
}



int mdw_led_open	(unsigned int idAll)
{
	int status = 1;
	int valueInc = 0;

	if (true == sLedStatus.isLoad)
	{
		for (valueInc= 1 ;valueInc<=LED_NUMBER_MAX; valueInc = valueInc <<1 )
			if (LED_GREEN_1 == ( idAll&LED_NUMBER_MAX))
			{
				drv_led_open(LED_GREEN_1 , 0);
				status= 0;
				sLedStatus.isOpen = true;
			}
	}
	return status;
}


//[msi] : une fois que j'aurais trouvé le mode qui consomme le moins ( soit entré , sois sorti en Z ), ameliorer cette partie

int mdw_led_close	(unsigned int idAll)
{
	int status = 1;
	int valueInc = 0;

	if (true == sLedStatus.isOpen)
	{
		for (valueInc= 1 ;valueInc<=LED_NUMBER_MAX; valueInc = valueInc <<1 )
			if (LED_GREEN_1 == ( idAll&LED_NUMBER_MAX))
			{
				drv_led_close(LED_GREEN_1 , 0);
				status= 0;
				sLedStatus.isOpen = false;
			}
	}
	return status;
}

int mdw_led_set (const led_idLed id ,const led_state state , int notUse)
{
	int status = 1;
	if ( true == sLedStatus.isOpen)
	{
		status = drv_led_setStateLed (id,state);
	}

	return status;
}





