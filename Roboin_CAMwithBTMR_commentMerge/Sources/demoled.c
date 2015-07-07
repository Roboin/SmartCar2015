/*
 * demoled.c
 *
 *  Created on: Dec 29, 2014
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.            
 */

#include "ngv_dmu.h"
#include "gpio.h"
#include "demoled.h"

//- LED 초기화 (전부 OFF)
void DLED_Init(void)
{
	DLED_SetAll(0);
}


//- LED 4개 전체를 On/Off
//	  B0 자리가 LED1에 해당, 1이면 ON, 0이면 OFF
//	  B1 자리가 LED2
//	  B2 자리가 LED3
//	  B3 자리가 LED4
void DLED_SetAll(uint16_t val)
{
	DLED_Set(DLED_LED1, val&0x1 ? DLED_ON : DLED_OFF);
	DLED_Set(DLED_LED2, val&0x2 ? DLED_ON : DLED_OFF);
	DLED_Set(DLED_LED3, val&0x4 ? DLED_ON : DLED_OFF);
	DLED_Set(DLED_LED4, val&0x8 ? DLED_ON : DLED_OFF);
}

