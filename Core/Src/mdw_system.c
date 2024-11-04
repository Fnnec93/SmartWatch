/*
 * system.c
 *
 *  Created on: Apr 19, 2024
 *      Author: ASUS
 */



#include "drv_system.h"

#include "mdw_system.h"

static void mdw_local_system_configClock	(void);
static void mdw_local_system_initSystem		(void);



int mdw_system_initAll (SYSTEM_STATE state)
{
	int status= 1;

	switch (state)
	{
	case SYSTEM_LOAD_NORMAL :

		mdw_local_system_initSystem	();


		status =0;

		break;
	case SYSTEM_AFTER_WAKEUP:
	default:
		status =1;
		break;
	}
	return status;
}
int mdw_system_initClock (SYSTEM_STATE state)
{
	int status= 1;

	switch (state)
	{
	case SYSTEM_LOAD_NORMAL :

		mdw_local_system_configClock();
		status =0;

		break;
	case SYSTEM_AFTER_WAKEUP:
	default:
		status =1;
		break;
	}
	return status;
}

void mdw_system_getClockCpuSpeed (unsigned int *pClockCpuSpeed_MHZ )
{
	drv_system_getSpeedCpuClock(pClockCpuSpeed_MHZ);
}

static void mdw_local_system_configClock	(void)
{
	// la sequence suivantes doit étre respecter
	drv_system_openClock();
	drv_system_initMainOscilator();
	drv_system_initClockAllPeripheral();
}


static void mdw_local_system_initSystem	(void)
{
	// c'est obligatoire de suivres examenement la procédure suivantes !! example pris depuis "HAL_Init"
	drv_system_initMemoryCache( );
	drv_system_initInterruptVector( );
	drv_system_initSysTick( );
	drv_system_initLowLvlHw( );
}


