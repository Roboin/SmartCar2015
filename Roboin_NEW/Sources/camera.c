/*
 * camera.c
 *
 *  Created on: Feb 23, 2015
 *      Author: �������б� �����а� ������
 */

#include "camera.h"
#include "gpio.h"
#include "adc_drv.h"
#include "timer.h"


//####### Pin Guide ##########
//in gpio.h
//int16_t CAM_DATA_PIN1 = 4;//4=pd0//
//int16_t CAM_DATA_PIN2 = 5;//5=pd1//
//int16_t CAM_SI_PIN= GPIO_PF0;//GPIO_PD12;
//int16_t CAM_CLK_PIN= GPIO_PF1;//GPIO_PF1;

//####### timer variables ##########
int16_t TIME_CONST= 1;//1*(16<<NGV_DMU_SYSCLK);
int16_t DELAY_SI= 25;//20*TIME_CONST;//(16 << NGV_DMU_SYSCLK);//micosec
int16_t DELAY_CLK= 50;//25*TIME_CONST;//*(16 << NGV_DMU_SYSCLK);//micosec
int16_t DELAY_WAIT= 500;//30*TIME_CONST;//*(16 << NGV_DMU_SYSCLK);//micosec
int16_t CAM_TIME_TOT= 0;// = delay_clk*128*2 + delay_additional;//11��	 //micro sec 

vuint32_t CAM_MICRO_SEC = 0;
vuint32_t CAM_MICRO_SEC_OLD = 0; 		
vuint32_t t0_CAM_SI;
vuint32_t t0_CAM_CLK;//variable for store timers
vuint32_t t0_CAM_CLK_DOWN;
vuint32_t t0_CAM_WAIT;

int8_t CAM_TIMER_CH= 4; // pit ch 3 or 4, but 3 is for FUSS
int16_t CAM_CLK_COUNTER= 0;

int8_t CAM_SI_FLAG= 1;
int8_t CAM_CLK_FLAG= 1;
int16_t CAM_RUN_MODE = 1;

vuint16_t CAM_TEMP1[NUM_OF_PIXEL+1] = {};
vuint16_t CAM_READ1[NUM_OF_PIXEL] = {};
vuint16_t CAM_TEMP2[NUM_OF_PIXEL+1] = {};
vuint16_t CAM_READ2[NUM_OF_PIXEL] = {};

void CAM_Init(void)
{	
	TIME_CONST= 1;//(16<<NGV_DMU_SYSCLK);
	DELAY_SI= DELAY_SI / TIME_CONST; //micosec
	DELAY_CLK= DELAY_CLK / TIME_CONST;//micosec
	DELAY_WAIT = DELAY_WAIT / TIME_CONST;//micosec
	CAM_TIME_TOT= DELAY_CLK*128*2*2 + DELAY_WAIT; //128*2 cause up & down//micro sec 
	CAM_RESET_TIMER();
}

void CAM_ISR(void){
	CAM_MICRO_SEC += PIT0_TIMER_GAP;//10;//recall every 10 microsec
	if(CAM_RUN_MODE == 1){
		CAM_RUN();
	}
	else{//if(CAM_RUN_MODE == 2){
		//CAM_RUN2();
	}
}

vuint32_t CAM_MICRO_SEC_RETURN(void){
	return CAM_MICRO_SEC;//cam_timer_value;
}

void CAM_RESET_TIMER(void){
	CAM_MICRO_SEC = 0;
}

void CAM_SI_ON(void){
	if(CAM_SI_FLAG==0){
		CAM_SI_FLAG = 1;
	}
}

void CAM_RUN(void)
{
	//CAM_SI();//For trigger Pulse
	if(CAM_SI_FLAG==1){
		GPIO_Set(CAM_SI_PIN,1);
		t0_CAM_SI = CAM_MICRO_SEC;//BTMR_GetRuntime_Micros();//
		CAM_SI_FLAG = 2;
	}
	if(CAM_SI_FLAG==2){
		if( (CAM_MICRO_SEC - t0_CAM_SI) >= DELAY_SI){
			GPIO_Set(CAM_SI_PIN,0);
		}
		CAM_Clock();
	}
	
	//CAM_Clock(); // For 128 + 1 Clock Pulse
	
	//CAM Restart	
	if(CAM_SI_FLAG==0){
		GPIO_Set(CAM_SI_PIN,0);
		if( (CAM_MICRO_SEC - t0_CAM_WAIT) >= DELAY_WAIT){//CAM_TIME_TOT){
			CAM_CLK_COUNTER = 0;
			CAM_SI_FLAG = 1;
			CAM_RESET_TIMER();
		}
	}
}

void CAM_Clock(void)
{
	if(CAM_CLK_COUNTER < (NUM_OF_PIXEL + 1) ){
		if( CAM_CLK_FLAG == 1 ){
			GPIO_Set(CAM_CLK_PIN,1);
			if( (CAM_MICRO_SEC - t0_CAM_CLK) >= DELAY_CLK){
				CAM_TEMP1[CAM_CLK_COUNTER] = ADC_Get(CAM_DATA_PIN1);//A2D_GetSingleCh_10bit(CAM_DATA_PIN1);
				CAM_TEMP2[CAM_CLK_COUNTER] = ADC_Get(CAM_DATA_PIN2);//A2D_GetSingleCh_10bit(CAM_DATA_PIN2);
				CAM_CLK_FLAG = 0;
				t0_CAM_CLK_DOWN = CAM_MICRO_SEC;
			}
		}
		if( CAM_CLK_FLAG == 0 ){
			GPIO_Set(CAM_CLK_PIN,0);
			if( (CAM_MICRO_SEC - t0_CAM_CLK_DOWN) >= DELAY_CLK){
				t0_CAM_CLK = CAM_MICRO_SEC;
				CAM_CLK_COUNTER++;
				CAM_CLK_FLAG = 1;
			}
		}
	}
	if(CAM_CLK_COUNTER >= (NUM_OF_PIXEL + 1 )){
		GPIO_Set(CAM_CLK_PIN,0);
		t0_CAM_WAIT = CAM_MICRO_SEC;
		CAM_SI_FLAG=0;
		CAM_UPDATE_DATA();//temp to data
	}
}

void CAM_RUN2(void)
{
	CAM_CLK_COUNTER = 0;
	GPIO_Set(CAM_SI_PIN, 1);//Trigger Pulse
	CAM_DELAY(10);
	GPIO_Set(CAM_CLK_PIN, 1);//First clock pulse
	CAM_DELAY(DELAY_SI-10);
	GPIO_Set(CAM_SI_PIN, 0);//Trigger Down
	CAM_DELAY(10);
	CAM_TEMP1[0] = ADC_Get(CAM_DATA_PIN1);//A2D_GetSingleCh_10bit(CAM_DATA_PIN1);
	CAM_TEMP2[0] = ADC_Get(CAM_DATA_PIN2);//A2D_GetSingleCh_10bit(CAM_DATA_PIN2);
	GPIO_Set(CAM_CLK_PIN, 0);//Dummy Clock Down
	
	for(CAM_CLK_COUNTER=1; CAM_CLK_COUNTER < NUM_OF_PIXEL+1; CAM_CLK_COUNTER++){
		GPIO_Set(CAM_CLK_PIN, 1);//CLock On
		CAM_DELAY(DELAY_CLK);
		CAM_TEMP1[CAM_CLK_COUNTER] = ADC_Get(CAM_DATA_PIN1);//A2D_GetSingleCh_10bit(CAM_DATA_PIN1);
		CAM_TEMP2[CAM_CLK_COUNTER] = ADC_Get(CAM_DATA_PIN2);//A2D_GetSingleCh_10bit(CAM_DATA_PIN2);
		GPIO_Set(CAM_CLK_PIN, 0);//Clock Down
		CAM_DELAY(DELAY_CLK);
		//SDBG_Printf("\n %d",PIT.CH[CAM_TIMER_CH].CVAL.R);
	}
	CAM_UPDATE_DATA();
	CAM_DELAY(DELAY_WAIT);
}

uint16_t CAM_DATA1(int16_t pixelnum){
	return CAM_READ1[pixelnum];
}
uint16_t CAM_DATA2(int16_t pixelnum){
	return CAM_READ2[pixelnum];
}

int16_t i_cam=0;// for for�ݺ���
uint16_t CAM_MAX(void){
	uint16_t max_value = CAM_READ1[0];
	for(i_cam=0; i_cam < (NUM_OF_PIXEL); i_cam++){
		if(max_value < CAM_READ1[i_cam])
			max_value = CAM_READ1[i_cam];
	}
	return max_value;
}

uint16_t CAM_MIN(void){
	uint16_t min_value = CAM_READ1[0];
	for(i_cam=0; i_cam < (NUM_OF_PIXEL); i_cam++){
		if(min_value > CAM_READ1[i_cam])
			min_value = CAM_READ1[i_cam];
	}
	return min_value;
}

uint16_t CAM_TRESHLOLD(void)//(int16_t pixelnum)
{
	return CAM_AVG();
}

uint16_t CAM_AVG(void)
{
	uint16_t CAM_READ_AVG = (CAM_MIN()+CAM_MAX())/2;
	return CAM_READ_AVG;// + 20;//+10 to too dark
}

void CAM_DELAY(int32_t delay_long)
{
	int32_t delay_val = 0;
	delay_long = (delay_long << 6)/5; 
	while(delay_val < delay_long){
		delay_val++;}
}

void CAM_PulseWidth(int16_t PULSEWIDTH)
{
	DELAY_CLK= PULSEWIDTH;
	DELAY_CLK= DELAY_CLK * TIME_CONST;
}

void CAM_UPDATE_DATA(void)
{
	int16_t i=0;
	for(i=0;i<128;i++)
	{
		CAM_READ1[i] = CAM_TEMP1[i+1];
		CAM_READ2[i] = CAM_TEMP2[i+1];
	}
}

void CAM_RUN_MODE_SETUP(int16_t mode)
{
	CAM_RUN_MODE = mode;
}
