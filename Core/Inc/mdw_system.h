/*
 * system.h
 *
 *  Created on: Apr 19, 2024
 *      Author: ASUS
 */

#ifndef INC_MDW_SYSTEM_H_
#define INC_MDW_SYSTEM_H_

typedef enum
{
	SYSTEM_LOAD_NORMAL 	= 0, // démarrage normal
	SYSTEM_AFTER_WAKEUP = 1,
	SYSTEM_MAX_STATE	= 2,

}SYSTEM_STATE;

int mdw_system_initAll 				(SYSTEM_STATE state);
int mdw_system_initClock 			(SYSTEM_STATE state);
void mdw_system_getClockCpuSpeed 	(unsigned int *pClockCpuSpeed_MHZ );




#endif /* INC_MDW_SYSTEM_H_ */
