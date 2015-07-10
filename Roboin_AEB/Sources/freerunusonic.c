/*
 * freerunusonic.c
 *
 *  Created on: Jan 10, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.        
 */


#include "freerunusonic.h"
#include "ngv_dmu.h"
#include "gpio.h"
#include "stdlib.h"

vuint16_t fuss_en_channel=FUSS_CH_ALL;
vuint16_t fuss_tmeans;
vuint16_t fuss_a, fuss_b;
vuint32_t rval = 0;
vuint32_t rval_old = 0;

void FUSS_PIT3_ISR(void)
{
	vuint32_t t0;
	
	//if (fuss_en_channel & FUSS_CH0)
		GPIO_Set(GPIO_PG10, 1);
	
	t0 = PIT.CH[3].CVAL.R;
	
	#if NGV_DMU_SYSCLK == SYSCLK_IRC_16MHZ
		while ((t0-PIT.CH[3].CVAL.R)<(16*11)) {
		}
	#elif NGV_DMU_SYSCLK == SYSCLK_PLL_32MHZ
		while ((t0-PIT.CH[3].CVAL.R)<(32*11)) {
		}
	#elif NGV_DMU_SYSCLK == SYSCLK_PLL_64MHZ
		while ((t0-PIT.CH[3].CVAL.R)<(64*11)) {
		}
	#endif

	GPIO_Set(GPIO_PG10, 0);
	PIT.CH[3].TFLG.R = 0x00000001;
}

void FUSS_EMIOS1_F10_F11_ISR (void)
{
    EMIOS_1.CH[10].CSR.B.FLAG = 1;

	if (EMIOS_1.CH[11].CSR.B.FLAG == 1) {

		fuss_a = (uint16_t)EMIOS_1.CH[11].CADR.R;
		fuss_b = (uint16_t)EMIOS_1.CH[11].CBDR.R;
		fuss_tmeans = fuss_a - fuss_b;
		EMIOS_1.CH[11].CSR.B.FLAG = 1;
	}
}

void FUSS_EMIOS1_F12_F13_ISR(void){
	EMIOS_1.CH[12].CSR.B.FLAG = 1;
	EMIOS_1.CH[13].CSR.B.FLAG = 1;
}
void FUSS_EMIOS1_F14_F15_ISR(void){
	EMIOS_1.CH[14].CSR.B.FLAG = 1;
	EMIOS_1.CH[15].CSR.B.FLAG = 1;
}


void FUSS_Init(uint16_t channels)
{
	FUSS_Enable(channels);
}


void FUSS_Enable(uint16_t channels)
{
	fuss_en_channel = channels;
}


uint32_t FUSS_GetMm(void)//(uint16_t channel)
{
	rval = (fuss_tmeans * 2785 + 2048) >> 12;  // 0.34 mm/usec * 4 usec/count * 4096 / 2(반사) 
	if(rval > 3500)
		rval = 3500;
	if( abs((int)(rval_old - rval)) > 1000)
		rval = rval_old;
	
	rval_old = rval;
	return rval;
}

