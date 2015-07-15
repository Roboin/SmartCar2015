/*
 * basetimer.c
 *
 *  Created on: Jan 1, 2015
 *      Author: ����̷����б� ����
 *        Note: 2015�� NGV ���л� �� ���� ����, ����, ������ ���մϴ�.        
 */


#include "basetimer.h"
#include "camera.h"


vuint32_t btmr_msec = 0; 				// runtime ���� ����, 32bit�̹Ƿ� 2^32-1 msec���� ���� ����
vuint32_t btmr_micro_sec = 0; 			//  base timer runtime ���� ����, 
vuint32_t btmr_tstart0, btmr_tstart1; 	// timer0, timer1�� ���۽��� runtime �����
vuint32_t btmr_tset0, btmr_tset1;     	// timer0, timer1�� ���� �ð�

//- Base Timer PIT0(periodic interrupt timer channel 0) ISR(Interrupt Service Routine) 
// �� �Լ��� IntcInterrupt.c �� �� �Լ��� ������ְ� ���������δ�  PIT0 ( �ý��� Ŭ���� ���� ī��Ʈ�ؼ� 10us ���� �۵� ) �� �������� �۵���
//	  Recall every 10? micro sec
void BTMR_PIT0_ISR (void)
{
	//btmr_msec++;
	btmr_micro_sec += PIT0_TIMER_GAP;//10;
	CAM_ISR();
	PIT.CH[0].TFLG.R = 0x00000001; // Ÿ�̸�(periodic interrupt Timer) ch 0 ����
}


//- BTMR �ʱ�ȭ
void BTMR_Init(void)
{
	btmr_msec = 0;
	btmr_micro_sec=0;
}


//- msec millisecond���� ������Ű�� �Լ�
void BTMR_DelayMs(uint32_t msec)
{
	uint32_t t0;

	if (msec==0)
		return;
	
	t0 = BTMR_GetRuntime();
	while ((BTMR_GetRuntime()-t0)<msec) {
	}
}


//- timer0�� msec millisecond�� �����ϴ� �Լ�
//    �� �Լ� ȣ�� ���� timer0 ���� 0�� �� ������ �����ϸ�
//    �� ���� BTMR_GetTimr0�� Ȯ�� ����
void BTMR_SetTimer0(uint32_t msec)
{
	btmr_tstart0 = BTMR_GetRuntime();
	btmr_tset0 = msec;
}


//- timer0�� ���� ���� millisecond ������ �����ϴ� �Լ�
//    BTMR_settimer0�� �Բ� ���
uint32_t BTMR_GetTimer0(void)
{
	uint32_t etime;

	etime = BTMR_GetRuntime()-btmr_tstart0;
	
	if (etime < btmr_tset0) {
		return (btmr_tset0-etime);
	}
	
	return 0;  
}


//- timer1�� msec millisecond�� �����ϴ� �Լ�
//    �� �Լ� ȣ�� ���� timer1 ���� 0�� �� ������ �����ϸ�
//    �� ���� BTMR_GetTimr1�� Ȯ�� ����
void BTMR_SetTimer1(uint32_t msec)
{
	btmr_tstart1 = BTMR_GetRuntime();
	btmr_tset1 = msec;
}


//- timer1�� ���� ���� millisecond ������ �����ϴ� �Լ�
// 	  BTMR_settimer1�� �Բ� ���
uint32_t BTMR_GetTimer1(void)
{
	uint32_t etime;
	
	etime = BTMR_GetRuntime()-btmr_tstart1;

	if (etime < btmr_tset1) {
		return (btmr_tset1-etime);
	}
	
	return 0;  
}


//- millisecond ������ runtime�� �����ϴ� �Լ�
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


//- millisecond ������ msec�� runtime�� �����ϴ� �Լ�
void BMTR_SetRuntime(uint32_t msec)
{
	btmr_msec = msec;
}
