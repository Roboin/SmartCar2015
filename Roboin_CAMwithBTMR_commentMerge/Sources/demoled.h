/*
 * demoled.h
 *
 *  Created on: Dec 29, 2014
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.      
 */

#ifndef DEMOLED_H_
#define DEMOLED_H_

#include "gpio.h"

#define DLED_LED1  GPIO_PE4
#define DLED_LED2  GPIO_PE5
#define DLED_LED3  GPIO_PE6
#define DLED_LED4  GPIO_PE7

#define DLED_ON 	1
#define DLED_OFF 	0



void DLED_Init(void);

//- LED 한 개를 ON/OFF
//	  led: DLED_LED1~DLED_LED4 중 하나
// 	  onoff: DLED_ON, DLED_OFF 중 선택
#define DLED_Set(led, onoff)	GPIO_Set((led), ((onoff)==DLED_ON ? 0 : 1))

//- LED 한 개를 Toggle (ON이면 OFF, OFF면 ON)
//	  led: DLED_LED1~DLED_LED4 중 하나
#define DLED_Toggle(led)		GPIO_Set((led), GPIO_GetDO(led) ? 0 : 1)


void DLED_SetAll(uint16_t val);


#endif /* DEMOLED_H_ */
