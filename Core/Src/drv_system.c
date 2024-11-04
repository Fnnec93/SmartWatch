/*
 * drv_system.c
 *
 *  Created on: Apr 19, 2024
 *      Author: ASUS
 */

#include "stm32f4xx_hal.h"

#include "drv_system.h"




typedef struct
{
	unsigned int CpuSpeed;

}Syteme_info;
Syteme_info SystemInfo;






//TODO a comprendre ...
void drv_system_initMemoryCache( void)
{
	//TODO deplacer les défine depuis le fichier de TM32F4xx_HAL_CONF_H a ici
	/* Configure Flash prefetch, Instruction cache, Data cache */
	#if (INSTRUCTION_CACHE_ENABLE != 0U)
	  __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
	#endif /* INSTRUCTION_CACHE_ENABLE */

	#if (DATA_CACHE_ENABLE != 0U)
	  __HAL_FLASH_DATA_CACHE_ENABLE();
	#endif /* DATA_CACHE_ENABLE */

	#if (PREFETCH_ENABLE != 0U)
	  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	#endif /* PREFETCH_E  */
}

//TODO a comprendre ... je les commencé grace au timer
void drv_system_initInterruptVector( void)
{
	/* Set Interrupt Group Priority */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
}

//TODO a comprendre ... a priori c'est un timer qui trigger chaque 1 ms
void drv_system_initSysTick( void)
{
	/* Use systick as time base source and configure 1ms tick (default clock after Reset is HSI) */
	HAL_InitTick(TICK_INT_PRIORITY);
}

//TODO a comprendre ... a priori il permet de set la configuration du RCC
void drv_system_initLowLvlHw( void)
{
	  /* Init the low level hardware */
	  HAL_MspInit();
}




void drv_system_openClock (void)
{
	  /** Configure the main internal regulator output voltage
	  */
	  __HAL_RCC_PWR_CLK_ENABLE();
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
}
void drv_system_initMainOscilator (void)
{
	  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	  /** Initializes the RCC Oscillators according to the specified parameters
	  * in the RCC_OscInitTypeDef structure.
	  */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	  RCC_OscInitStruct.PLL.PLLM = 16;
	  RCC_OscInitStruct.PLL.PLLN = 336;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	  RCC_OscInitStruct.PLL.PLLQ = 4;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  else
	  {
		  SystemInfo.CpuSpeed = 84; //TODO pour l'instant c'est une valeur ecrite en dur ... une fois qu'on aura vue le mode veille, faudra penser a faire evolv cette partie
	  }
}
void drv_system_initClockAllPeripheral (void)
{
	  /** Initializes the CPU, AHB and APB buses clocks
	  */
	  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

int drv_system_getSpeedCpuClock (unsigned int *pValue)
{
	*pValue = SystemInfo.CpuSpeed;
}




