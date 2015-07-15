/*
 * ultra_sonic.h
 *  Created on: Dec 29, 2014
 *  Author: 동양미래대학교 정준
 *  Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.
 *  
 *  Edited on: Jul 10, 2015
 *  Editor: JaeHakJeong
 *  Note: 본 헤더파일은 동양미래대학교 정준 교수님의 허가 하에 편집되었습니다. 
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
