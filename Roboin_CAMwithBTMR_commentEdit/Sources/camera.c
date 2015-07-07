/*
 * camera.c
 *
 *  Created on: Feb 23, 2015
 *      Author: 연세대학교 기계공학과 정재학
 */

#include "camera.h"
#include "freerunadc.h"
#include "ngv_dmu.h"
#include "gpio.h"
#include "basetimer.h"


//####### Pin Guide ##########
int16_t CAM_DATA_PIN= 5;//5=pd0//14=GPIO_PD10;//PD10;
int16_t CAM_SI_PIN= GPIO_PF0;//GPIO_PD12;
int16_t CAM_CLK_PIN= GPIO_PF1;//GPIO_PF1;

//####### timer variables ##########
int16_t TIME_CONST= 1;//1*(16<<NGV_DMU_SYSCLK);
int16_t DELAY_SI= 30;//20*TIME_CONST;//(16 << NGV_DMU_SYSCLK);//micosec
int16_t DELAY_CLK= 50;//25*TIME_CONST;//*(16 << NGV_DMU_SYSCLK);//micosec
int16_t DELAY_ADD= 300;//30*TIME_CONST;//*(16 << NGV_DMU_SYSCLK);//micosec
int16_t CAM_TIME_TOT= 0;// = delay_clk*128*2 + delay_additional;//11번	 //micro sec 

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

vint16_t CAM_TEMP[NUM_OF_PIXEL+1] = {};
vint16_t CAM_READ[NUM_OF_PIXEL] = {};
vint16_t dummy_data[NUM_OF_PIXEL] = {};

void CAM_Init(void)
{	
	TIME_CONST= 1;//(16<<NGV_DMU_SYSCLK);
	DELAY_SI= DELAY_SI / TIME_CONST; //(16 << NGV_DMU_SYSCLK);//micosec
	DELAY_CLK= DELAY_CLK / TIME_CONST;//*(16 << NGV_DMU_SYSCLK);//micosec
	DELAY_ADD= DELAY_ADD / TIME_CONST;//*(16 << NGV_DMU_SYSCLK);//micosec
	CAM_TIME_TOT= DELAY_CLK*128*2*2 + DELAY_ADD; //128*2 cause up & down//micro sec 
	CAM_RESET_TIMER();
}
//Camera Interrupt Service Routine
void CAM_ISR(void){
	CAM_MICRO_SEC += 10;//recall every 10 microsec
	if(CAM_RUN_MODE == 1)
		CAM_RUN();
	if(CAM_RUN_MODE == 2)
		CAM_RUN2();
}

vuint32_t CAM_MICRO_SEC_RETURN(void){
	return CAM_MICRO_SEC;//cam_timer_value;
}

void CAM_RESET_TIMER(void){
	CAM_MICRO_SEC = 0;
}

void CAM_RUN(void)
{	
	/*signal(trigger)신호(카메라 작동 시작신호) 내보내기
	 */
	if(CAM_SI_FLAG==1){
		GPIO_Set(CAM_SI_PIN,1);
		t0_CAM_SI = CAM_MICRO_SEC; // 현재 시각 가져오기
		CAM_SI_FLAG = 2;
	}
	if(CAM_SI_FLAG==2){
		if( (CAM_MICRO_SEC - t0_CAM_SI) >= DELAY_SI){
			GPIO_Set(CAM_SI_PIN,0);
		}
	}
	
	/*cluck 신호(필셀 값 읽어오는 신호) 내보내기*/
	//CAM_Clock_ISR(); // For 128 + 1 Clock Pulse
	if(CAM_CLK_COUNTER < (NUM_OF_PIXEL + 1) ){
		if( CAM_CLK_FLAG == 1 ){
			GPIO_Set(CAM_CLK_PIN,1);
			if( (CAM_MICRO_SEC - t0_CAM_CLK) >= DELAY_CLK){
				CAM_TEMP[CAM_CLK_COUNTER] = FADC_Get(CAM_DATA_PIN);
				CAM_CLK_FLAG = 0;
				t0_CAM_CLK_DOWN = CAM_MICRO_SEC;
			}
		}
		else{
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
		CAM_SI_FLAG=0;
	}
	
	//CAM Restart	
	if(CAM_SI_FLAG==0){
		if( (CAM_MICRO_SEC - t0_CAM_SI) >= CAM_TIME_TOT){
			CAM_RESET_TIMER();
			CAM_CLK_COUNTER = 0;
			CAM_SI_FLAG = 1;
		}
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
	//dummy_data[CAM_CLK_COUNTER] = FADC_Get(CAM_DATA_PIN);//Read Pixel
	CAM_TEMP[0] = FADC_Get(CAM_DATA_PIN);//Read Pixel
	GPIO_Set(CAM_CLK_PIN, 0);//Dummy Clock Down
	
	for(CAM_CLK_COUNTER=1; CAM_CLK_COUNTER < NUM_OF_PIXEL+1; CAM_CLK_COUNTER++){
		GPIO_Set(CAM_CLK_PIN, 1);//CLock On
		CAM_DELAY(DELAY_CLK);
		CAM_TEMP[CAM_CLK_COUNTER] = FADC_Get(CAM_DATA_PIN);//Read Pixel
		GPIO_Set(CAM_CLK_PIN, 0);//Clock Down
		CAM_DELAY(DELAY_CLK);
		//SDBG_Printf("\n %d",PIT.CH[CAM_TIMER_CH].CVAL.R);
	}
	CAM_UPDATE_DATA();
	CAM_DELAY(DELAY_ADD);
}

int16_t i_cam=0;
int16_t pt1 = 13;//10*100/50; //#For 100ms#
int16_t pt2 = 45;//43*100/50;
int16_t pt3 = 118;//NUM_OF_PIXEL-(NUM_OF_PIXEL-120)*100/50;
int16_t initial_value = 627;

int16_t CAM_DATA(int16_t pixelnum){//기울기 크면 데이터 처리.
//50micro - 0-19 하강 & 20-85 완만하게 상승
//100micro  0-13 하강, 13-45 상승 118부터 급하강
	/* int16_t cam_read_offset = CAM_MAX()*276/800;//800/4;//CAM_MAX()/4;
	if(pixelnum <= pt1)
		return CAM_READ[pixelnum]*800/(-9.45*pixelnum+628);
	else if( (pt1 < pixelnum) && (pixelnum <= pt2) )
		return CAM_READ[pixelnum]*800/(7.7*(pixelnum-pt1)+518);
	else if( pt3 < pixelnum )
		return CAM_READ[pixelnum]*800/(-9.7*(pixelnum-pt3)+798);
	else
		return CAM_READ[pixelnum];*/
	return CAM_READ[pixelnum];
}


int16_t CAM_MAX(void){
	int16_t max_value = CAM_READ[0];
	for(i_cam=0; i_cam < (NUM_OF_PIXEL); i_cam++){
		if(max_value < CAM_READ[i_cam])
			max_value = CAM_READ[i_cam];
	}
	return max_value;
}

int16_t CAM_MIN(void){
	int16_t min_value = CAM_READ[0];
	for(i_cam=0; i_cam < (NUM_OF_PIXEL); i_cam++){
		if(min_value > CAM_READ[i_cam])
			min_value = CAM_READ[i_cam];
	}
	return min_value;
}
int16_t CAM_TRESHLOLD(int16_t pixelnum)
{
	int16_t CAM_READ_AVG = (CAM_MIN()+CAM_MAX())/2;
	if(pixelnum < pt1)
		return CAM_MAX()*3/4;
	else if( (pt1 <= pixelnum) && (pixelnum < pt2) )
		return CAM_MAX()*3/4;
	else if( (3*pt3-2*NUM_OF_PIXEL) <= pixelnum )
		return CAM_MAX()*7/8;
	else
		return CAM_READ_AVG;// + 20;//+10 to too dark
}

void CAM_DELAY(int32_t delay_long)
{
	//vuint32_t t0_CAM_DELAY = PIT.CH[CAM_TIMER_CH].CVAL.R;
	int32_t delay_val = 0;
	delay_long = delay_long * (16 << NGV_DMU_SYSCLK); 
	//while ((t0_CAM_DELAY - PIT.CH[CAM_TIMER_CH].CVAL.R)<(delay_long)) {}//32 = 1 microsec
	while(delay_val < delay_long)
		delay_val++;
}

void CAM_PulseWidth(int16_t PULSEWIDTH)
{
	//pt1 = 10*100/PULSEWIDTH;//for read to data 보정
	//pt2 = 43*100/PULSEWIDTH;
	//pt3 = NUM_OF_PIXEL-(NUM_OF_PIXEL-120)*100/PULSEWIDTH;
	//DELAY_SI= PULSEWIDTH;
	DELAY_CLK= PULSEWIDTH;
	//DELAY_SI= DELAY_SI* TIME_CONST;
	DELAY_CLK= DELAY_CLK * TIME_CONST;
}

void CAM_UPDATE_DATA(void)
{
	int16_t i=0;
	for(i=0;i<128;i++)
	{
		CAM_READ[i] = CAM_TEMP[i+1];
	}
}

void CAM_RUN_MODE_SETUP(int16_t mode)
{
	CAM_RUN_MODE = mode;
}

