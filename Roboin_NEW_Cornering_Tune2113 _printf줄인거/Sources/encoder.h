/*
 * encodr.h
 * 
 *  Created on: Dec 29, 2014
 *  Author: 동양미래대학교 정준
 *  Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.
 *  
 *  Edited on: Jul 10, 2015
 *  Editor: JaeHakJeong
 *  Note: 본 헤더파일은 동양미래대학교 정준 교수님의 허가 하에 편집되었습니다. 
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include "jdp.h"
#include "gpio.h"

#define ENC1_A		GPIO_PA12
#define ENC1_B		GPIO_PA13
#define ENC2_B		GPIO_PA14
#define ENC2_A		GPIO_PA15

#define Pulse_Per_CM	11
#define PUlse_Per_Rev	1024
//- FUNCTION ----------------------------------------------
//void ENC_IRQ16_23_ISR(void);//PA12,13//11,10//15,14  PA11 : EIRQ16, 
void ENC1_EIRQ16_23_ISR(void);//PA12 : EIRQ17
void ENC2_EIRQ0_7_ISR(void);//PA14: EIRQ4
void ENC_Init(void);

int32_t ENC_Get1(void);
int32_t ENC_Get2(void);
void ENC_SPEED_PIT5_ISR(void);
int32_t ENC_Rate1_Return(void);
int32_t ENC_Rate2_Return(void);

void Speed_Control();
void Speed_Propsal_Update(int16_t proposal_speed, int16_t select, int16_t kp_in);


#endif /* ROTARYENCODER_H_ */
