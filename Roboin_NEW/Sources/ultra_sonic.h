/*
 * ultra_sonic.h
 *
 *  Created on: Jul 10, 2015
 *      Author: JaeHakJeong
 */

#ifndef ULTRA_SONIC_H_
#define ULTRA_SONIC_H_

#include "gpio.h"

#define US_Echo		GPIO_PG2
#define US_Trig		GPIO_PG10

void US_Init(void);
uint32_t US_Get(void);

void US_PIT3_ISR(void);
void US_EMIOS1_F10_11_ISR(void);

#endif /* ULTRA_SONIC_H_ */
