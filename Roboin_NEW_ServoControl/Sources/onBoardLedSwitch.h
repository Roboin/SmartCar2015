/*
 * onBoardLedSwitch.h
 *
 *  Created on: Jul 10, 2015
 *      Author: JaeHakJeong
 */

#ifndef ONBOARDLEDSWITCH_H_
#define ONBOARDLEDSWITCH_H_

#include "gpio.h"

#define SW_1	GPIO_PE0
#define SW_2	GPIO_PE1
#define SW_3	GPIO_PE2
#define SW_4	GPIO_PE3

#define LED_1	GPIO_PE4
#define LED_2	GPIO_PE5
#define LED_3	GPIO_PE6
#define LED_4	GPIO_PE7

#define DIP_1	GPIO_PG6
#define DIP_2	GPIO_PG7
#define DIP_3	GPIO_PG8
#define DIP_4	GPIO_PG9

void Led_Set(uint16_t led_num, uint8_t onoff);
void Led_Set_All(uint8_t onoff);
uint16_t BUTTON_Read(void);
uint16_t DIP_Read(void);


#endif /* ONBOARDLEDSWITCH_H_ */
