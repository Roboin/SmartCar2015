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

vuint16_t fuss_en_channel=FUSS_CH_ALL;

vuint16_t fuss_flag = 1;
vuint16_t fuss_echo;
vuint16_t fuss_tmeas[FUSS_CH_NUM];
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
	fuss_flag = 1;
	PIT.CH[3].TFLG.R = 0x00000001;
}

void FUSS_EMIOS1_F10_F11_ISR (void)
{
    EMIOS_1.CH[10].CSR.B.FLAG = 1;

	if (EMIOS_1.CH[11].CSR.B.FLAG == 1) {

		fuss_a = (uint16_t)EMIOS_1.CH[11].CADR.R;
		fuss_b = (uint16_t)EMIOS_1.CH[11].CBDR.R;
		fuss_tmeas[0] = fuss_a - fuss_b;
		
		EMIOS_1.CH[11].CSR.B.FLAG = 1;
	}
}

void FUSS_EMIOS1_F12_F13_ISR(void){}
void FUSS_EMIOS1_F14_F15_ISR(void){}


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
	//uint32_t rval = 0;
	uint32_t diff =  (rval_old - rval);
	int32_t gap = 10;
	//if (channel==FUSS_CH0)
		//rval = fuss_tmeas[0];
	//else if (channel==FUSS_CH1)
		//rval = fuss_tmeas[1];
	//else if (channel==FUSS_CH2)
		//rval = fuss_tmeas[2];
	//else if (channel==FUSS_CH3)
		//rval = fuss_tmeas[3];
	/*if( (-gap > diff) || (diff> gap) ){
		fuss_flag = 1;
		rval_old = rval;		
	}*/
	
	if(fuss_flag == 1){
		rval = fuss_tmeas[0];
		rval = (rval*2785+2048)>>12;  // 0.34 mm/usec * 4 usec/count * 4096 / 2(반사) 
	}
	fuss_flag = 0 ;
	return rval;
}

