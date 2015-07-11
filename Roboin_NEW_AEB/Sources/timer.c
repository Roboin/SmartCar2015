
#include "timer.h"
//include "camera.h"


vuint32_t timer_msec = 0;

//- PIT0 ISR(Interrupt Service Routine)
//	  Recoll every 1 micro sec
void TIMER_PIT0_ISR (void)
{
	timer_msec++;
	PIT.CH[0].TFLG.R = 0x00000001;
}


//- timer 초기화
void TIMER_Init(void)
{
	timer_msec = 0;
}


//- msec millisecond동안 지연시키는 함수
void TIMER_DelayMs(uint32_t msec)
{
	uint32_t t0;

	if (msec==0)
		return;
	
	t0 = timer_msec;//TIMER_GetRuntime();
	while ( (timer_msec - t0) < msec ){}//((TIMER_GetRuntime()-t0)<msec) {}
}

//- millisecond 단위로 runtime을 리턴하는 함수
uint32_t TIMER_GetRuntime(void)
{
	uint32_t runtime;
	//timer_msec = timer_micro_sec/1000;
	runtime = timer_msec;

	return runtime;  
}
