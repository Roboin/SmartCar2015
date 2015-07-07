/*
 * motorcontrol.c
 *
 *  Created on: Jan 11, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.        
 */


#include "motorcontrol.h"
#include "gpio.h"


void MCTL_Init(void)
{
	GPIO_Set(GPIO_PA8, 1);
}

//- RC Servo 모터에 angle에 해당하는 각도 명령을 내림
//    angle의 범위는 MCTL_SWHEEL_MIN ~ MCTL_SWHEEL_MAX
void MCTL_Servo(int16_t angle)
{
	angle += MCTL_SWHEEL_OFFSET;
	if (angle < MCTL_SWHEEL_MIN)
		angle = MCTL_SWHEEL_MIN;
	
	if (angle > MCTL_SWHEEL_MAX)
		angle = MCTL_SWHEEL_MAX;

	EMIOS_0.CH[1].CADR.R = (uint32_t)((angle<<5)+MCTL_SWHEEL_RAW_MID);
//	EMIOS_0.CH[1].CADR.R = (angle*(MCTL_SWHEEL_RAW_MAX-MCTL_SWHEEL_RAW_MID)/MCTL_SWHEEL_MAX)+MCTL_SWHEEL_RAW_MID;
}

//- DC Motor에 speed에 해당하는 PWM 출력
//	  speed는 MCTL_ENGINE_MIN ~ MCTL_ENGINE_MAX 범위
void MCTL_DC_R(int16_t speed)
{
	if (speed < MCTL_ENGINE_MIN)
		speed = MCTL_ENGINE_MIN;//-2047
	
	GPIO_Set(GPIO_PA4, 0);//HA IN2

	GPIO_Set(GPIO_PA4, 0);
	EMIOS_0.CH[2].CADR.R = speed;//PA2, AS HA IN1
}

void MCTL_DC_L(int16_t speed)
{
	if (speed < MCTL_ENGINE_MIN)
		speed = MCTL_ENGINE_MIN;//-2047
	
	GPIO_Set(GPIO_PA5, 0);//HB IN2
	
	GPIO_Set(GPIO_PA5, 0);
	EMIOS_0.CH[3].CADR.R = speed;//PA3, AS HB IN1
}

//- DC Motor Brake
void MCTL_BrakeEngle(void)
{
	GPIO_Set(GPIO_PA4, 0);
	GPIO_Set(GPIO_PA5, 0);
	EMIOS_0.CH[2].CADR.R = 0;
	EMIOS_0.CH[3].CADR.R = 0;
}
