/*
 * rotaryencodr.c
 *
 *  Created on: Jan 11, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다. 
 */

#include "rotaryencoder.h"
#include "gpio.h"

vint32_t renc_val;


void RENC_IRQ16_23_ISR(void)
{
	SIU.ISR.R = 0x00FF0000;
	
	if (GPIO_Get(GPIO_PA12) == 1) {
		if (GPIO_Get(GPIO_PA13) == 1) {
			renc_val++;
		}
		else {
			renc_val--;
		}
	}
	else {
		if (GPIO_Get(GPIO_PA13) == 1) {
			renc_val--;
		}
		else {
			renc_val++;
		}
	}
}


void RENC_Init(void)
{
	renc_val = 0;
}


// val 값으로 encoder 회전각(단위: pulse)를 설정
void RENC_Set(int32_t val)
{
	renc_val = val;
}


// Pulse 단위의 Encoder 회전각을 리턴
int32_t RENC_Get(void)
{
	return renc_val;
}


