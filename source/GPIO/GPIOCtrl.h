/*
 * GPIOCtrl.h
 *
 *  Created on: Jan 4, 2018
 *      Author: Administrator
 */

#ifndef SOURCE_GPIO_GPIOCTRL_H_
#define SOURCE_GPIO_GPIOCTRL_H_

#include"gpio/GPIO.h"
using namespace exploringBB;


int GPIOCtrl_Init();
void GPIOCtrl_SetOut(GPIO_VALUE value);
void GPIOCtrl_swingPort(void);
//void GPIOCtrl_SetOut(GPIO_VALUE value);

#endif /* SOURCE_GPIO_GPIOCTRL_H_ */
