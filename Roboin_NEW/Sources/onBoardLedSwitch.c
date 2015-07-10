/*
 * onBoardLedSwitch.c
 *
 *  Created on: Jul 10, 2015
 *      Author: JaeHakJeong
 */

#include "onBoardLedSwitch.h"

//LED
void LedSet(uint16_t led_num, uint8_t onoff){
	uint16_t led_pin = LED_1 + led_num - 1;
	GPIO_Set(led_pin, !onoff);
}
void LedSetAll(uint8_t onoff){
	GPIO_Set(1, !onoff);
	GPIO_Set(2, !onoff);
	GPIO_Set(3, !onoff);
	GPIO_Set(4, !onoff);
}

//SW
uint16_t BUTTON_Read(void){
	uint16_t rval = 0;
	rval = GPIO_Get(SW_1);
	rval+= GPIO_Get(SW_2)<<1;
	rval+= GPIO_Get(SW_3)<<2;
	rval+= GPIO_Get(SW_3)<<4;
	return rval;
}

//DIP
uint16_t DIP_Read(void){
	uint16_t rval = 0;
	rval = GPIO_Get(DIP_1);
	rval+= GPIO_Get(DIP_2)<<1;
	rval+= GPIO_Get(DIP_3)<<2;
	rval+= GPIO_Get(DIP_4)<<4;
	return rval;
}


