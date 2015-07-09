/*
 * basetimer.c
 *
 *  Created on: Jan 1, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.        
 */


#include "basetimer.h"
#include "camera.h"


vuint32_t btmr_msec = 0; 				// runtime 저장 변수, 32bit이므로 2^32-1 msec까지 측정 가능
vuint32_t btmr_micro_sec = 0; 				// runtime 저장 변수, 
vuint32_t btmr_tstart0, btmr_tstart1; 	// timer0, timer1의 시작시의 runtime 저장용
vuint32_t btmr_tset0, btmr_tset1;     	// timer0, timer1의 설정 시간

//- PIT0 ISR(Interrupt Service Routine)
//	  Recoll every 1 micro sec
void BTMR_PIT0_ISR (void)
{
	//btmr_msec++;
	btmr_micro_sec += PIT0_TIMER_GAP;//10;
	CAM_ISR();
	PIT.CH[0].TFLG.R = 0x00000001;
}


//- BTMR 초기화
void BTMR_Init(void)
{
	btmr_msec = 0;
	btmr_micro_sec=0;
}


//- msec millisecond동안 지연시키는 함수
void BTMR_DelayMs(uint32_t msec)
{
	uint32_t t0;

	if (msec==0)
		return;
	
	t0 = BTMR_GetRuntime();
	while ((BTMR_GetRuntime()-t0)<msec) {
	}
}


//- timer0를 msec millisecond로 설정하는 함수
//    이 함수 호출 이후 timer0 값은 0이 될 때까지 감소하며
//    이 값은 BTMR_GetTimr0로 확인 가능
void BTMR_SetTimer0(uint32_t msec)
{
	btmr_tstart0 = BTMR_GetRuntime();
	btmr_tset0 = msec;
}


//- timer0의 현재 값을 millisecond 단위로 리턴하는 함수
//    BTMR_settimer0와 함께 사용
uint32_t BTMR_GetTimer0(void)
{
	uint32_t etime;

	etime = BTMR_GetRuntime()-btmr_tstart0;
	
	if (etime < btmr_tset0) {
		return (btmr_tset0-etime);
	}
	
	return 0;  
}


//- timer1를 msec millisecond로 설정하는 함수
//    이 함수 호출 이후 timer1 값은 0이 될 때까지 감소하며
//    이 값은 BTMR_GetTimr1로 확인 가능
void BTMR_SetTimer1(uint32_t msec)
{
	btmr_tstart1 = BTMR_GetRuntime();
	btmr_tset1 = msec;
}


//- timer1의 현재 값을 millisecond 단위로 리턴하는 함수
// 	  BTMR_settimer1와 함께 사용
uint32_t BTMR_GetTimer1(void)
{
	uint32_t etime;
	
	etime = BTMR_GetRuntime()-btmr_tstart1;

	if (etime < btmr_tset1) {
		return (btmr_tset1-etime);
	}
	
	return 0;  
}


//- millisecond 단위로 runtime을 리턴하는 함수
uint32_t BTMR_GetRuntime(void)
{
	uint32_t runtime;
	btmr_msec = btmr_micro_sec/1000;
	runtime = btmr_msec;

	return runtime;  
}

uint32_t BTMR_GetRuntime_Micros(void)
{
	//uint32_t runtime2;
	//runtime2 = btmr_micro_sec;
	return btmr_micro_sec;//runtime2;  
}


//- millisecond 단위의 msec로 runtime을 설정하는 함수
void BMTR_SetRuntime(uint32_t msec)
{
	btmr_msec = msec;
}

