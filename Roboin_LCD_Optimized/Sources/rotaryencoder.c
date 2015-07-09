/*
 * rotaryencodr.c
 *
 *  Created on: Jan 11, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다. 
 */

#include "rotaryencoder.h"
#include "gpio.h"


//To make 2 endcoder available 

vint32_t renc_val1;
vint32_t renc_val2;

void RENC_IRQ16_23_ISR(void)
{
	SIU.ISR.R = 0x00FF0000;
	
	if (GPIO_Get(GPIO_PA12) == 1) {
		if (GPIO_Get(GPIO_PA13) == 1) {
			renc_val1++;
		}
		else {
			renc_val1--;
		}
	}
	//else {
	if(GPIO_Get(GPIO_PA12) == 0){
		if (GPIO_Get(GPIO_PA13) == 1) {
			renc_val1--;
		}
		else {
			renc_val1++;
		}
	}
	
	
	//encoder 2 
	if (GPIO_Get(GPIO_PA11) == 1) {
		if (GPIO_Get(GPIO_PA10) == 1) {
			renc_val2++;
		}
		else {
			renc_val2--;
		}
	}
	//else {
	if (GPIO_Get(GPIO_PA11) == 0) {
		if (GPIO_Get(GPIO_PA10) == 1) {
			renc_val2--;
		}
		else {
			renc_val2++;
		}
	}
}


void RENC_Init(void)
{
	renc_val1 = 0;
	renc_val2 = 0;
}

// val 값으로 encoder 회전각(단위: pulse)를 설정
void RENC_Set1(int32_t val)
{
	renc_val1 = val;
}
// Pulse 단위의 Encoder 회전각을 리턴
int32_t RENC_Get1(void)
{
	return renc_val1;
}

//Encoder 2
void RENC_Set2(int32_t val)
{
	renc_val2 = val;
}
// Pulse 단위의 Encoder 회전각을 리턴
int32_t RENC_Get2(void)
{
	return renc_val2;
}
