/*
 * demoswitch.c
 *
 *  Created on: Jan 1, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.
 */

#include "ngv_dmu.h"
#include "gpio.h"
#include "demoswitch.h"


//- Switch 초기화
void DSW_Init(void)
{
}

//- Tactile Switch 4개 전체의 On/Off 상태를 리턴
//	  B0 자리가 S1에 해당, 1이면 ON, 0이면 OFF
//	  B1 자리가 S2
//	  B2 자리가 S3
//	  B3 자리가 S4
uint16_t DSW_GetTactAll(void)
{
	uint16_t val = 0;
	
	if (DSW_Get(DSW_S1)==DSW_ON)
		val |= 0x1;
	if (DSW_Get(DSW_S2)==DSW_ON)
		val |= 0x2;
	if (DSW_Get(DSW_S3)==DSW_ON)
		val |= 0x4;
	if (DSW_Get(DSW_S4)==DSW_ON)
		val |= 0x8;
	
	return val;
}

//- 4P Dip Switch 전체의 On/Off 상태를 리턴
//	  B0 자리가 DIP1에 해당, 1이면 ON, 0이면 OFF
//	  B1 자리가 DIP2
//	  B2 자리가 DIP3
//	  B3 자리가 DIP4
uint16_t DSW_GetDipAll(void)
{
	uint16_t val = 0;
	
	if (DSW_Get(DSW_DIP1)==DSW_ON)
		val |= 0x1;
	if (DSW_Get(DSW_DIP2)==DSW_ON)
		val |= 0x2;
	if (DSW_Get(DSW_DIP3)==DSW_ON)
		val |= 0x4;
	if (DSW_Get(DSW_DIP4)==DSW_ON)
		val |= 0x8;
	
	return val;
}
