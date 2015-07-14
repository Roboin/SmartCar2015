/*
 * onBoardLedSwitch.c
 *
 *  Created on: Jul 10, 2015
 *      Author: JaeHakJeong
 */

#include "onBoardLedSwitch.h"

//LED
void Led_Set(uint16_t led_num, uint8_t onoff){
	uint16_t led_pin = LED_1 + led_num - 1;
	GPIO_Set(led_pin, !onoff);
}
void Led_Set_All(uint8_t onoff){
	GPIO_Set(LED_1, !onoff);
	GPIO_Set(LED_2, !onoff);
	GPIO_Set(LED_3, !onoff);
	GPIO_Set(LED_4, !onoff);
}

//SW
uint16_t BUTTON_Read(void){
	uint16_t rval = 0;
	rval = GPIO_Get(SW_1);
	rval+= GPIO_Get(SW_2)<<1;
	rval+= GPIO_Get(SW_3)<<2;
	rval+= GPIO_Get(SW_4)<<3;
	return rval;
}

//DIP
uint16_t DIP_Read(void){
	uint16_t rval = 0;
	rval = GPIO_Get(DIP_1);
	rval+= GPIO_Get(DIP_2)<<1;
	rval+= GPIO_Get(DIP_3)<<2;
	rval+= GPIO_Get(DIP_4)<<3;
	return rval;
}


