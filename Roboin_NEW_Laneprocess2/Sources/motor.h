/*
 * motor.h
 */
#ifndef MOTOR_H_
#define MOTOR_H_

#include "jdp.h"
#include "gpio.h"
#include "encoder.h"

#define H_EN		GPIO_PA6
#define H_A_IN1		GPIO_PA2
#define H_B_IN1		GPIO_PA3
#define H_A_IN2		GPIO_PA4
#define H_B_IN2		GPIO_PA5

#define MOTOR_SERVO_RAW_MAX		7800//(9200)
#define MOTOR_SERVO_RAW_MID		6000
#define MOTOR_SERVO_RAW_MIN		4200//(2800)
#define MOTOR_SERVO_RAW_OFFSET	-56

#define MOTOR_SERVO_MAX		(36)//(45)
#define MOTOR_SERVO_MIN		(-36)//(-45)
#define MOTOR_SERVO_OFFSET		-2//(-3)

#define MOTOR_DC_MAX	(2047)
#define MOTOR_DC_MIN	(-2047)

void MOTOR_Init(void);
void MOTOR_Servo(int16_t angle);
void MOTOR_Servo_withPWM(int32_t pulsewidth);
void MOTOR_DC_R(int16_t speed);
void MOTOR_DC_L(int16_t speed);
void MOTOR_BrakeEngine(void);

int32_t MOTOR_Current_Speed_R(void);
int32_t MOTOR_Current_Speed_L(void);

#endif /* MOTOR_H_ */
