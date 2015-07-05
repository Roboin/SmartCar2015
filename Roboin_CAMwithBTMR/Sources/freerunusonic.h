/*
 * freerunusonic.c
 *
 *  Created on: Jan 10, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.        
 */

#ifndef FREERUNUSONIC_H_
#define FREERUNUSONIC_H_

#include "jdp.h"

#define FUSS_CLK		(0.25)	// MHz

#define FUSS_CH0 		0x1
#define FUSS_CH1 		0x2
//#define FUSS_CH2 		0x4
//#define FUSS_CH3 		0x8
#define FUSS_CH_ALL	0xf
#define FUSS_CH_NUM	3//4

void FUSS_PIT3_ISR(void);
void FUSS_EMIOS1_F10_F11_ISR(void);
void FUSS_EMIOS1_F12_F13_ISR(void);
void FUSS_EMIOS1_F14_F15_ISR(void);
void FUSS_Init(uint16_t channels);
void FUSS_Enable(uint16_t channels);
uint32_t FUSS_GetMm(uint16_t channel);


#endif /* FREERUNUSONIC_H_ */
