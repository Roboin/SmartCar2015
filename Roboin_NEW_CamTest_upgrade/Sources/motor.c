/*
 * motorcontrol.c
 *
 *  Created on: Jan 11, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.        
 */


#include "motor.h"
#include "gpio.h"
#include "encoder.h"
#include "pit_init.h"

void MOTOR_Init(void)
{
	GPIO_Set(H_EN, 1);
}

//- RC Servo Control with angle
void MOTOR_Servo(int16_t angle)
{
	angle += MOTOR_SERVO_OFFSET;
	if (angle < MOTOR_SERVO_MIN)
		angle = MOTOR_SERVO_MIN;
	
	if (angle > MOTOR_SERVO_MAX)
		angle = MOTOR_SERVO_MAX;

	EMIOS_0.CH[1].CADR.R = (uint32_t)((angle<<5) + MOTOR_SERVO_RAW_MID);
//	EMIOS_0.CH[1].CADR.R = (angle*(MCTL_SWHEEL_RAW_MAX-MCTL_SWHEEL_RAW_MID)/MCTL_SWHEEL_MAX)+MCTL_SWHEEL_RAW_MID;
}

void MOTOR_Servo_withPWM(int32_t pulsewidth)//input -3200~+3200
{
	pulsewidth += MOTOR_SERVO_RAW_MID;
	pulsewidth += MOTOR_SERVO_RAW_OFFSET;
	if (pulsewidth < MOTOR_SERVO_RAW_MIN)
		pulsewidth = MOTOR_SERVO_RAW_MIN;
	
	if (pulsewidth > MOTOR_SERVO_RAW_MAX)
		pulsewidth = MOTOR_SERVO_RAW_MAX;

	EMIOS_0.CH[1].CADR.R = pulsewidth;
//	EMIOS_0.CH[1].CADR.R = (angle*(MCTL_SWHEEL_RAW_MAX-MCTL_SWHEEL_RAW_MID)/MCTL_SWHEEL_MAX)+MCTL_SWHEEL_RAW_MID;
}

//DC Motor Cotrol, Speed 0~2047
void MOTOR_DC_R(int16_t speed)
{
	if (speed > MOTOR_DC_MAX)
		speed = MOTOR_DC_MAX;//2047
	if (speed < MOTOR_DC_MIN)
		speed = MOTOR_DC_MIN;//-2047
	
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
	if (speed > MOTOR_DC_MAX)
		speed = MOTOR_DC_MAX;//2047
	if (speed < MOTOR_DC_MIN)
		speed = MOTOR_DC_MIN;//-2047
	
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

int32_t current_speed_r = 1;
int32_t current_speed_l = 1;
int32_t MOTOR_Current_Speed_R(void){// cm/s = enc_rate*1000/40/11 = enc_rate*2.273 = enc_rate * (2048+256+16+8)/1024
	current_speed_r = (ENC_Rate1_Return()<<10)/pit5cycle/Pulse_Per_CM;//((ENC_Rate1_Return()<<11)+(ENC_Rate1_Return()<<8)+(ENC_Rate1_Return()<<4)+(ENC_Rate1_Return()<<3))>>10;//ENC_Rate1_Return()*2.273
	return current_speed_r;
}
int32_t MOTOR_Current_Speed_L(void){
	current_speed_l = (ENC_Rate2_Return()<<10)/pit5cycle/Pulse_Per_CM;//((ENC_Rate2_Return()<<11)+(ENC_Rate2_Return()<<8)+(ENC_Rate2_Return()<<4)+(ENC_Rate2_Return()<<3))>>10;
	return current_speed_l;
}
