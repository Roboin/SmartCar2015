/*
 * ultra_sonic.c
 *
 *  Created on: Jul 10, 2015
 *      Author: JaeHakJeong
 */

#include "ultra_sonic.h"
#include "stdlib.h"

vuint8_t flag_US = 1;
vuint32_t us_echo;
vuint32_t us_tmeans;
vuint32_t us_a, us_b;
vuint32_t us_val = 0;
vuint32_t us_val_old = 0;




void US_PIT3_ISR(void){
	vuint32_t t0;
	GPIO_Set(US_Trig, 1);//Trig high
	t0 = PIT.CH[3].CVAL.R;
	while ((t0-PIT.CH[3].CVAL.R)<(64*11)) {}// Trig high for 11 us
	GPIO_Set(US_Trig, 0);
	
	PIT.CH[3].TFLG.R = 0x00000001;
}

void US_EMIOS1_F10_11_ISR(void)
{
	EMIOS_1.CH[10].CSR.B.FLAG = 1;
	if (EMIOS_1.CH[11].CSR.B.FLAG == 1) {
		us_a = (uint32_t)EMIOS_1.CH[11].CADR.R;
		us_b = (uint32_t)EMIOS_1.CH[11].CBDR.R;
		us_tmeans = us_a - us_b;
		EMIOS_1.CH[11].CSR.B.FLAG = 1;
	}
}

void US_Init(void){
	us_val = 0;
}

uint32_t US_Get(void){
	us_val = (us_tmeans * 2785 + 2048) >> 12;  // 0.34 mm/usec * 4 usec/count * 4096 / 2(�ݻ�) 
	if(us_val > 3500)
		us_val = 3500;
	if( abs((int)(us_val_old - us_val)) > 1000)
		us_val = us_val_old;
	
	us_val_old = us_val;
	return us_val;
}
