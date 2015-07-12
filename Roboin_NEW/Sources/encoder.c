/*
 * encoder.c
 * 
 */

#include "encoder.h"
#include "gpio.h"


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

void ENC_SPEED_PIT5_ISR(void){
	enc_rate1 = enc_val1;// - enc_val1_old;
	enc_rate2 = enc_val2;// - enc_val2_old;
	enc_val1 = 0;
	enc_val2 = 0;
	PIT.CH[5].TFLG.R = 0x00000001;
}

int32_t ENC_Rate1_Return(void){
	return enc_rate1;
}
int32_t ENC_Rate2_Return(void){
	return enc_rate2;
}
