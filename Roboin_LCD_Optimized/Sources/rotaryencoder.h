/*
 * rotaryencodr.h
 *
 *  Created on: Jan 11, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다. 
 */

#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

#include "jdp.h"

#define RENC_PRIORITY PORTH_PRIORITY

#define RENC_PPR_1X 1000 // Pulse Per Round

//- FUNCTION ----------------------------------------------
void RENC_IRQ16_23_ISR(void);
void RENC_Init(void);//PA12,13//11,10

void RENC_Set1(int32_t val);
int32_t RENC_Get1(void);
void RENC_Set2(int32_t val);
int32_t RENC_Get2(void);


#endif /* ROTARYENCODER_H_ */
