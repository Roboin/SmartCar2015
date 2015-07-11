
#include "timer.h"
//include "camera.h"


vuint32_t timer_msec = 0; 				
vuint32_t timer_micro_sec = 0; 				
vuint32_t timer_tstart0, timer_tstart1; 	
vuint32_t timer_tset0, timer_tset1;

//- PIT0 ISR(Interrupt Service Routine)
//	  Recoll every 1 micro sec
void TIMER_PIT0_ISR (void)
{
	//timer_msec++;
	timer_micro_sec += PIT0_TIMER_GAP;//20;
	//CAM_ISR();
	PIT.CH[0].TFLG.R = 0x00000001;
}


//- timer �ʱ�ȭ
void TIMER_Init(void)
{
	timer_msec = 0;
	timer_micro_sec=0;
}


//- msec millisecond���� ������Ű�� �Լ�
void TIMER_DelayMs(uint32_t msec)
{
	uint32_t t0;

	if (msec==0)
		return;
	
	t0 = TIMER_GetRuntime();
	while ((TIMER_GetRuntime()-t0)<msec) {
	}
}

//- millisecond ������ runtime�� �����ϴ� �Լ�
uint32_t TIMER_GetRuntime(void)
{
	uint32_t runtime;
	timer_msec = timer_micro_sec/1000;
	runtime = timer_msec;

	return runtime;  
}

uint32_t TIMER_GetRuntime_Micros(void)
{
	//uint32_t runtime2;
	//runtime2 = timer_micro_sec;
	return timer_micro_sec;//runtime2;  
}