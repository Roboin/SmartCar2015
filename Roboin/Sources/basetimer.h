/*
 * basetimer.h
 *
 *  Created on: Jan 1, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.        
 */

#ifndef BASETIMER_H_
#define BASETIMER_H_

#include "jdp.h"

void BTMR_PIT0_ISR (void);
void BTMR_Init(void);
void BTMR_DelayMs(uint32_t msec);
void BTMR_SetTimer0(uint32_t msec);
uint32_t BTMR_GetTimer0(void);
void  BTMR_SetTimer1(uint32_t msec);
uint32_t BTMR_GetTimer1(void);
uint32_t BTMR_GetRuntime(void);

uint32_t BTMR_GetRuntime_Micros(void);

void BMTR_SetRuntime(uint32_t msec);

#define PIT0_TIMER_GAP 25

#endif /* BASETIMER_H_ */
