/*
 * encoder.c
 * 
 */

#include "encoder.h"
#include "gpio.h"
#include "motor.h"

vint32_t enc_val1 = 1;
vint32_t enc_val2 = 1;
vint32_t enc_rate1 = 1;
vint32_t enc_rate2 = 1;

void ENC1_EIRQ16_23_ISR(void)
{
	SIU.ISR.R = 0x000F0000;
	if (GPIO_Get(ENC1_A) == 1) {
		if (GPIO_Get(ENC1_B) == 1) {
			enc_val1++;
		}
		else {
			enc_val1--;
		}
	}
	if(GPIO_Get(ENC1_A) == 0){
		if (GPIO_Get(ENC1_B) == 1) {
			enc_val1--;
		}
		else {
			enc_val1++;
		}
	}
}

void ENC2_EIRQ0_7_ISR(void)
{
	SIU.ISR.R = 0x000000F0;
	if (GPIO_Get(ENC2_B) == 1) {
		if (GPIO_Get(ENC2_A) == 1) {
			enc_val2++;
		}
		else {
			enc_val2--;
		}
	}
	if (GPIO_Get(ENC2_B) == 0) {
		if (GPIO_Get(ENC2_A) == 1) {
			enc_val2--;
		}
		else {
			enc_val2++;
		}
	}
}

void ENC_Init(void)
{
	enc_val1 = 0;
	enc_val2 = 0;
}

//Return Encoder1 Pulse
int32_t ENC_Get1(void)
{
	return enc_val1;
}

//Return Encoder2 Pulse
int32_t ENC_Get2(void)
{
	return enc_val2;
}

int32_t ENC_Rate1_Return(void){
	return enc_rate1;
}
int32_t ENC_Rate2_Return(void){
	return enc_rate2;
}

int32_t DC_Power_R = 500;
int32_t DC_Power_L = 500;

int16_t Speed_Proposal = 0;
int16_t kpR = 1;
int16_t kpL = 1;
int16_t ki = 1;
void ENC_SPEED_PIT5_ISR(void){
	enc_rate1 = enc_val1;
	enc_rate2 = enc_val2;
	enc_val1 = 0;
	enc_val2 = 0;
	
	Speed_Control(Speed_Proposal);
	MOTOR_DC_R(DC_Power_R);
	MOTOR_DC_L(DC_Power_L);
	
	PIT.CH[5].TFLG.R = 0x00000001;
}

void Speed_Propsal_Update(int16_t proposal_speed){
	Speed_Proposal = proposal_speed;
}

int32_t cont_val_I_R = 0;
int32_t cont_val_I_L = 0;
int32_t cont_val_P_R = 0;
int32_t cont_val_P_L = 0;

void Speed_Control(){
	int32_t Speed_R = MOTOR_Current_Speed_R();
	int32_t Speed_L = MOTOR_Current_Speed_L();
	
	int32_t error_R = Speed_Proposal - Speed_R;
	int32_t error_L = Speed_Proposal - Speed_L;
	
	kpR = 10;
	kpL = 10;
	ki = 3;
	
	cont_val_P_R = error_R*kpR;
	cont_val_P_L = error_L*kpL;
	cont_val_I_R += error_R*ki;
	cont_val_I_L += error_L*ki;
	
	DC_Power_R = cont_val_P_R + cont_val_I_R;
	DC_Power_L = cont_val_P_L + cont_val_I_L;
}
