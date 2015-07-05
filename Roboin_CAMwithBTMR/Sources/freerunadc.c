/*
 * freerunadc.c
 *
 *  Created on: Jan 8, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.        
 */


#include "freerunadc.h"

const vint32_t fadc_lpf_alpha[FADC_CH_MAX] = {
    FADC_LPF_ALPHA0, FADC_LPF_ALPHA1, FADC_LPF_ALPHA2, FADC_LPF_ALPHA3,
    FADC_LPF_ALPHA4, FADC_LPF_ALPHA5, FADC_LPF_ALPHA6, FADC_LPF_ALPHA7,
    FADC_LPF_ALPHA8, FADC_LPF_ALPHA9, FADC_LPF_ALPHA10, FADC_LPF_ALPHA11,
    FADC_LPF_ALPHA12, FADC_LPF_ALPHA13, FADC_LPF_ALPHA14, FADC_LPF_ALPHA15
};

const vint32_t fadc_lpf_beta[FADC_CH_MAX] = {
    FADC_LPF_BETA0, FADC_LPF_BETA1, FADC_LPF_BETA2, FADC_LPF_BETA3,
    FADC_LPF_BETA4, FADC_LPF_BETA5, FADC_LPF_BETA6, FADC_LPF_BETA7,
    FADC_LPF_BETA8, FADC_LPF_BETA9, FADC_LPF_BETA10, FADC_LPF_BETA11,
    FADC_LPF_BETA12, FADC_LPF_BETA13, FADC_LPF_BETA14, FADC_LPF_BETA15
};

vint32_t fadc_in_k[FADC_CH_MAX] = {0,};
vint32_t fadc_in_km1[FADC_CH_MAX] = {0,};
vint32_t fadc_out_k[FADC_CH_MAX] = {0,};
vint32_t fadc_out_km1[FADC_CH_MAX] = {0,};

void FADC_ADC0_EOC_ISR (void)
{
	uint16_t ch;
	
	for (ch=0; ch<FADC_CH_MAX; ch++) {
		
		if (ADC_0.JCMR0.R & (1<<ch)) {
			fadc_in_k[ch] = (vint32_t)ADC_0.CDR[ch].B.CDATA<<4;
			fadc_out_k[ch] = (fadc_lpf_alpha[ch]*fadc_out_km1[ch]+fadc_lpf_beta[ch]*(fadc_in_k[ch]+fadc_in_km1[ch])+16384)>>15;
			fadc_out_km1[ch] = fadc_out_k[ch]; 
			fadc_in_km1[ch] = fadc_in_k[ch]; 			
		}
	}
	
    ADC_0.ISR.R=0x0000001F;
}

//- Freerun ADC 초기화
//    enables B0자리부터 ADC0_P[0]에 해당함, 1이면 Enable, 0이면 Disable
void FADC_Init(uint16_t enables)
{
	ADC_0.JCMR0.R = enables;
}


//- channel(0~15)에 대해 Low Pass Filter가 적용된 ADC값 리턴
int16_t FADC_Get(int16_t channel)
{
	return (int16_t)(fadc_out_k[channel]>>4);
}
