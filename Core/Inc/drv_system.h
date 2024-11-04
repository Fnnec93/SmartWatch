/*
 * drv_system.h
 *
 *  Created on: Apr 19, 2024
 *      Author: ASUS
 */

#ifndef INC_DRV_SYSTEM_H_
#define INC_DRV_SYSTEM_H_

void drv_system_initMemoryCache( void);
void drv_system_initInterruptVector( void);
void drv_system_initSysTick( void);
void drv_system_initLowLvlHw( void);

void drv_system_openClock (void);
void drv_system_initMainOscilator (void);
void drv_system_initClockAllPeripheral (void);

int drv_system_getSpeedCpuClock (unsigned int *pValue);


#endif /* INC_DRV_SYSTEM_H_ */
