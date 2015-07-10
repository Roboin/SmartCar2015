/*
 * freerunadc.h
 *
 *  Created on: Jan 8, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.        
 */

#ifndef FREERUNADC_H_
#define FREERUNADC_H_

#include "jdp.h"

#define FADC_CH_MAX 16

#define FADC_LPF_ALPHA0  26283	// fc = 77Hz
#define FADC_LPF_BETA0   3242

#define FADC_LPF_ALPHA1  24732	// fc = 100Hz
#define FADC_LPF_BETA1   4018

#define FADC_LPF_ALPHA2  24732
#define FADC_LPF_BETA2   4018

#define FADC_LPF_ALPHA3  24732
#define FADC_LPF_BETA3   4018

#define FADC_LPF_ALPHA4  24732
#define FADC_LPF_BETA4   4018

#define FADC_LPF_ALPHA5  24732
#define FADC_LPF_BETA5   4018

#define FADC_LPF_ALPHA6  24732
#define FADC_LPF_BETA6   4018

#define FADC_LPF_ALPHA7  24732
#define FADC_LPF_BETA7   4018

#define FADC_LPF_ALPHA8  24732
#define FADC_LPF_BETA8   4018

#define FADC_LPF_ALPHA9  24732
#define FADC_LPF_BETA9   4018

#define FADC_LPF_ALPHA10 24732
#define FADC_LPF_BETA10  4018

#define FADC_LPF_ALPHA11 24732
#define FADC_LPF_BETA11  4018

#define FADC_LPF_ALPHA12 24732
#define FADC_LPF_BETA12  4018

#define FADC_LPF_ALPHA13 24732
#define FADC_LPF_BETA13  4018

#define FADC_LPF_ALPHA14 24732
#define FADC_LPF_BETA14  4018

#define FADC_LPF_ALPHA15 24732
#define FADC_LPF_BETA15  4018

void FADC_ADC0_EOC_ISR(void);
void FADC_Init(uint16_t enables);
int16_t FADC_Get(int16_t channel);

#endif /* FREERUNADC_H_ */
