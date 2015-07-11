/*
 * motorcontrol.c
 *
 *  Created on: Jan 11, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.        
 */


#include "motor.h"
#include "gpio.h"


void MOTOR_Init(void)
{
	GPIO_Set(H_EN, 1);
}

//- RC Servo Control with angle
void MOTOR_Servo(int16_t angle)
{
	angle += MOTOR_SWHEEL_OFFSET;
	if (angle < MOTOR_SWHEEL_MIN)
		angle = MOTOR_SWHEEL_MIN;
	
	if (angle > MOTOR_SWHEEL_MAX)
		angle = MOTOR_SWHEEL_MAX;

	EMIOS_0.CH[1].CADR.R = (uint32_t)((angle<<5) + MOTOR_SWHEEL_RAW_MID);
//	EMIOS_0.CH[1].CADR.R = (angle*(MCTL_SWHEEL_RAW_MAX-MCTL_SWHEEL_RAW_MID)/MCTL_SWHEEL_MAX)+MCTL_SWHEEL_RAW_MID;
}

//DC Motor Cotrol, Speed 0~2047
void MOTOR_DC_R(int16_t speed)
{
	if (speed > MOTOR_ENGINE_MAX)
		speed = MOTOR_ENGINE_MAX;//2047
	if (speed < MOTOR_ENGINE_MIN)
		speed = MOTOR_ENGINE_MIN;//-2047
	
	if(speed > 0){
		GPIO_Set(H_A_IN2, 0);//HA IN2
		EMIOS_0.CH[2].CADR.R = speed;//PA2, AS HA IN1
	}
	else{
		GPIO_Set(H_A_IN2, 1);//HA IN2
		EMIOS_0.CH[2].CADR.R = (uint32_t)(2047 + speed);//PA2, AS HA IN1
	}
}

void MOTOR_DC_L(int16_t speed)
{
	if (speed > MOTOR_ENGINE_MAX)
		speed = MOTOR_ENGINE_MAX;//2047
	if (speed < MOTOR_ENGINE_MIN)
		speed = MOTOR_ENGINE_MIN;//-2047
	
	if(speed > 0){
		GPIO_Set(H_B_IN2, 0);//HB IN2
		EMIOS_0.CH[3].CADR.R = speed;//PA3, AS HB IN1
	}
	else{
		GPIO_Set(H_B_IN2, 1);//HB IN2
		EMIOS_0.CH[3].CADR.R = (uint32_t)(2047 + speed);//PA3, AS HB IN1
	}
}

//- DC Motor Brake
void MOTOR_BrakeEngine(void)
{
	GPIO_Set(H_A_IN2, 0);
	GPIO_Set(H_B_IN2, 0);
	EMIOS_0.CH[2].CADR.R = 0;
	EMIOS_0.CH[3].CADR.R = 0;
}
