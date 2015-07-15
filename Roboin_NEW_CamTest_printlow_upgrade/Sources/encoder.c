/*
 * encoder.c
 * 
 */

#include "encoder.h"
#include "gpio.h"
#include "motor.h"


//To make 2 encoder available 

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
	//else {
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
	//else {
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
	enc_rate1 = enc_val1;// - enc_val1_old;
	enc_rate2 = enc_val2;// - enc_val2_old;
	enc_val1 = 0;
	enc_val2 = 0;
	
	Speed_Control(Speed_Proposal);
	MOTOR_DC_R(DC_Power_R);
	MOTOR_DC_L(DC_Power_L);
	
	PIT.CH[5].TFLG.R = 0x00000001;
}

void Speed_Propsal_Update(int16_t proposal_speed, int16_t select, int16_t k_in){ /* speed(cm/s) select는 왼쪽 오른쪽 모터 비례상수 선택, k_in은 비례상수 그러나 지금은 다 무시됨*/
	Speed_Proposal = proposal_speed;
	//kp = kp_in;
	if(select == 1)
		kpR = k_in;
	else if(select == 2)
		kpL = k_in;
	else if(select == 3)
		ki = k_in;
	else{}
}

int32_t cont_val_I_R = 0;
int32_t cont_val_I_L = 0;
int32_t cont_val_P_R = 0;
int32_t cont_val_P_L = 0;

void Speed_Control(){
	//kp scaling 필요.
	int32_t Speed_R = MOTOR_Current_Speed_R();
	int32_t Speed_L = MOTOR_Current_Speed_L();
	
	int32_t error_R = Speed_Proposal - Speed_R;
	int32_t error_L = Speed_Proposal - Speed_L;
	
	kpR = 10;//10;//4
	kpL = 10;//10;//4
	ki = 3;//1;//3
	
	cont_val_P_R = error_R*kpR;//(error_R<<10)/kp;
	cont_val_P_L = error_L*kpL;//(error_L<<10)/kp;
	cont_val_I_R += error_R*ki;
	cont_val_I_L += error_L*ki;
	
	
	DC_Power_R = cont_val_P_R + cont_val_I_R;
	DC_Power_L = cont_val_P_L + cont_val_I_L;
}
