/*
 * camera.c
 *
 *  Created on: JUNE 23, 2015
 *      Author: 연세대학교 기계공학과 정재학
 */

#include "camera.h"
#include "gpio.h"
#include "adc_drv.h"
#include "sys_init.h"
#include "onBoardLedSwitch.h"


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
int8_t CAM_RAN_FLAG = 0; /* 128pixel 읽은 후 1 되고 읽는동안 0인 flag*/

/* camera read value storage in camera.c */
vuint16_t CAM_TEMP1[NUM_OF_PIXEL+1] = {};//128+1
vuint16_t CAM_TEMP2[NUM_OF_PIXEL+1] = {};
//vuint16_t CAM_READ1[NUM_OF_PIXEL] = {892,1021,1022,1022,1023,1022,1022,1023,1022,1021,1022,1022,1023,1022,1023,1023,1023,1023,1022,1021,1022,1022,1022,1022,1021,1022,1022,1023,1021,1022,1023,1023,1022,1021,1023,1022,1023,1022,1022,1021,1021,1023,1021,1022,1023,1022,1021,1023,587,348,297,270,264,259,244,237,226,217,215,210,220,221,232,230,257,276,410,1023,1023,1023,1021,1022,1022,1022,1023,1023,1022,1023,1023,1023,1023,1023,1022,1021,1022,1021,1022,1022,1023,1023,1023,1023,1023,1022,1022,1021,1023,1022,1023,1021,1023,1023,1022,1023,1021,1021,1023,1022,1022,1022,1023,1023,1023,1022,1023,1022,1022,1022,1008,904,1023,1022,1017,979,889,844,769,705};//128
//vuint16_t CAM_READ2[NUM_OF_PIXEL] = {942,1021,1023,1022,1021,1023,1021,1022,1021,1022,1022,1022,1023,1023,1023,1023,1023,1023,1023,1022,1023,1023,1022,1021,1021,1023,1023,1023,1022,1023,1023,1022,1022,1023,1021,1021,1023,1022,1022,1021,1023,1023,1021,1023,1021,1022,1023,1022,1023,1022,1023,1023,1021,1021,1021,1023,1022,1022,1022,1021,1021,1022,349,301,241,227,211,206,204,210,212,212,200,207,214,229,241,258,257,300,998,1022,1022,1023,1021,1023,1022,1020,1021,1023,1022,1022,1022,1022,1021,1023,1021,1022,1022,1023,1023,1023,1022,1021,1022,1023,1022,1021,1022,1022,1021,1022,1023,1021,1023,1021,1021,1022,1021,989,880,967,911,808,783,722,683,666};
//{924,806,914,1023,1022,1023,1023,1021,1021,1022,1022,1020,1020,1022,1021,1022,1021,1022,1022,1023,1022,1022,1022,1022,1022,1023,1022,1023,1022,1023,1023,1023,1022,1023,1021,1022,1021,1022,1023,1022,1022,1023,1023,1021,1022,1021,1023,1021,1023,1022,1023,1023,1021,1022,1015,511,431,431,325,285,282,255,237,227,215,210,210,225,226,235,246,250,277,302,372,502,992,1023,1023,1023,1023,1021,1022,1023,1022,1023,1020,1023,1022,1022,1023,1022,1021,1022,1021,1022,1021,1023,1021,1023,1022,1022,1022,1022,1022,1022,1023,1023,1022,1022,1023,1022,1022,1023,1023,1022,1023,1022,1023,1011,894,944,945,869,799,701,697,689};
//{925,924,1022,1022,1022,1023,1023,1022,1023,1021,1022,1023,1023,1022,1021,1022,1022,1023,1023,1020,1020,1021,1021,1023,1022,1023,1023,1022,1023,1021,1022,1022,1021,1021,1022,1023,1023,1023,1022,1023,1022,1023,1021,1021,1022,1021,1022,1022,1023,1022,1023,1022,1022,932,439,369,304,302,262,248,234,235,227,206,224,222,224,234,233,254,319,319,357,566,1021,1022,1023,1021,1023,1021,1021,1023,1023,1023,1022,1021,1022,1022,1023,1023,1023,1022,1022,1023,1023,1022,1022,1023,1022,1021,1022,1021,1022,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1022,1023,1022,1023,1023,983,891,998,1010,953,895,767,693,666,0};
//{144,905,1023,1022,1023,1022,1022,1022,1021,1021,1021,1023,1022,1023,1022,1020,1023,1022,1023,1022,1022,1023,1023,1023,1023,1021,1021,1022,1023,1022,1022,1021,1023,1023,1022,1022,1022,1022,1022,1021,1023,1023,1023,1022,1023,1023,1022,1023,1022,1023,1022,1022,1022,767,456,353,300,286,253,238,246,235,229,228,223,204,214,227,254,277,369,336,413,446,826,1022,1023,1022,1023,1022,1023,1023,1023,1023,1022,1023,1023,1022,1023,1022,1022,1023,1023,1022,1021,1022,1022,1021,1023,1022,1021,1022,1022,1021,1021,1021,1022,1022,1023,1022,1021,1022,1023,1023,1021,1022,1023,1023,954,865,867,1022,862,809,759,792,723,672};

//보고서-카메라그래프 viewtrack
vuint16_t CAM_READ1[NUM_OF_PIXEL] = {};//{75, 85, 93, 102, 109, 115, 123, 131, 134, 137, 145, 148, 155, 160, 158, 162, 160, 167, 170, 172, 174, 174, 177, 181, 182, 179, 184, 186, 186, 190, 188, 191, 192, 195, 192, 195, 198, 199, 199, 199, 199, 200, 202, 203, 201, 202, 202, 203, 202, 203, 201, 199, 197, 200, 199, 198, 196, 193, 194, 190, 110, 72, 70, 69, 72, 74, 81, 174, 199, 202, 204, 204, 204, 204, 204, 206, 202, 205, 209, 211, 210, 210, 209, 216, 213, 190, 182, 179, 173, 171, 169, 166, 164, 161, 158, 156, 154, 152, 153, 146, 147, 148, 141, 140, 137, 137, 137, 137, 128, 130, 131, 130, 126, 125, 118, 115, 111, 109, 103, 97, 93, 88, 83, 75, 69, 61, 54, 5,};//{76,85,94,102,109,113,121,128,130,133,139,143,148,152,150,154,151,158,161,163,165,168,171,171,173,171,175,178,178,179,178,181,182,183,181,185,187,188,187,189,189,189,191,193,188,190,190,191,190,192,191,189,189,190,192,192,193,192,194,195,193,197,195,194,193,191,194,195,194,194,196,194,194,194,192,193,189,191,195,196,193,193,192,194,195,194,195,195,192,189,189,189,187,184,180,169,82,66,65,63,63,65,64,66,66,73,149,166,159,162,163,163,159,153,149,145,140,134,128,118,115,106,99,88,77,70,61,5};//{75,85,92,102,108,113,121,128,130,132,137,141,147,151,150,154,150,160,163,164,167,169,171,174,176,172,178,180,181,183,184,186,188,188,186,188,190,193,192,194,192,192,193,193,189,189,188,183,94,70,67,69,71,72,77,167,188,190,194,195,194,199,196,195,194,193,196,199,198,198,198,198,199,193,180,179,173,173,176,174,172,169,167,168,169,167,167,167,165,164,163,164,162,162,159,159,157,157,160,154,157,158,154,155,153,155,158,159,153,155,158,159,153,148,143,138,132,125,118,107,102,94,85,71,50,44,44,5};
vuint16_t CAM_READ2[NUM_OF_PIXEL] = {};//{75, 85, 93, 102, 109, 115, 123, 131, 134, 137, 145, 148, 155, 160, 158, 162, 160, 167, 170, 172, 174, 174, 177, 181, 182, 179, 184, 186, 186, 190, 188, 191, 192, 195, 192, 195, 198, 199, 199, 199, 199, 200, 202, 203, 201, 202, 202, 203, 202, 203, 201, 199, 197, 200, 199, 198, 196, 193, 194, 190, 110, 72, 70, 69, 72, 74, 81, 174, 199, 202, 204, 204, 204, 204, 204, 206, 202, 205, 209, 211, 210, 210, 209, 216, 213, 190, 182, 179, 173, 171, 169, 166, 164, 161, 158, 156, 154, 152, 153, 146, 147, 148, 141, 140, 137, 137, 137, 137, 128, 130, 131, 130, 126, 125, 118, 115, 111, 109, 103, 97, 93, 88, 83, 75, 69, 61, 54, 5};//{76,85,94,102,109,113,121,128,130,133,139,143,148,152,150,154,151,158,161,163,165,168,171,171,173,171,175,178,178,179,178,181,182,183,181,185,187,188,187,189,189,189,191,193,188,190,190,191,190,192,191,189,189,190,192,192,193,192,194,195,193,197,195,194,193,191,194,195,194,194,196,194,194,194,192,193,189,191,195,196,193,193,192,194,195,194,195,195,192,189,189,189,187,184,180,169,82,66,65,63,63,65,64,66,66,73,149,166,159,162,163,163,159,153,149,145,140,134,128,118,115,106,99,88,77,70,61,5};//{76,84,92,100,107,112,118,124,127,130,136,138,142,146,145,148,145,154,154,155,157,159,162,163,165,162,168,169,169,171,171,174,174,175,173,176,179,180,180,181,181,181,183,184,180,182,182,182,182,182,181,179,179,182,183,183,183,181,183,183,180,184,180,175,143,72,68,68,68,68,69,70,72,132,179,184,180,183,189,190,188,188,188,191,193,192,192,192,191,190,191,189,188,188,186,185,174,165,166,158,160,160,154,153,149,146,147,146,138,138,138,138,133,129,125,121,118,112,108,101,97,93,85,79,69,64,57,5};//{45,50,53,60,63,68,73,76,79,84,82,78,85,91,89,94,98,98,96,98,92,93,108,158,185,186,196,200,203,207,209,209,211,211,211,216,220,221,223,227,230,232,234,231,179,93,81,81,82,84,84,86,105,188,233,234,237,238,240,241,241,243,246,248,248,250,248,249,249,250,251,249,251,250,247,251,244,247,245,247,243,242,241,238,241,236,238,238,234,235,230,229,227,225,226,224,221,216,212,211,207,205,202,202,199,189,194,188,187,184,182,180,168,145,177,164,160,141,119,122,131,124,109,104,94,82,74,5};//{82,94,104,115,124,129,139,149,142,155,161,163,165,170,170,168,174,179,183,188,191,194,196,201,201,198,206,207,212,210,209,213,216,215,213,214,219,220,220,220,219,217,217,219,217,217,215,215,216,215,212,210,209,207,205,77,57,60,57,60,62,176,207,208,206,205,207,208,207,205,205,205,203,201,203,201,196,198,202,200,200,198,196,195,200,198,198,197,185,193,193,190,190,189,185,184,183,179,182,171,174,174,168,166,162,161,160,157,145,148,151,151,147,141,137,131,126,122,117,110,105,98,92,83,74,68,59,1};
//{80,90,101,110,118,121,136,143,145,148,153,153,160,162,160,167,160,170,175,177,191,193,195,199,202,197,203,205,207,209,211,212,214,215,215,214,218,219,231,222,202,220,208,218,215,215,216,216,215,216,212,210,208,209,205,78,59,59,58,58,67,176,208,207,209,204,208,206,205,205,204,205,203,200,201,201,195,195,201,200,198,197,196,198,197,198,198,197,193,194,195,191,190,187,186,184,182,181,181,171,171,173,165,165,162,159,162,158,148,146,150,150,144,142,136,132,130,121,113,104,100,93,88,79,67,64,57,2};

int16_t i_cam=0;// for for반복문

int16_t cam1LanePosition[CAM_MAX_LANE_NUM] = {};
int16_t cam2LanePosition[CAM_MAX_LANE_NUM] = {};

int16_t cam1LanePos_back[CAM_MAX_LANE_NUM] = {};
int16_t cam2LanePos_back[CAM_MAX_LANE_NUM] = {};

int16_t CAM_READ1_STORE[NUM_OF_PIXEL] = {}; // CAM_READ1 1대1로 받아오는 어레이
int16_t CAM_READ2_STORE[NUM_OF_PIXEL] = {};


int8_t cam1LaneNum = 0;
int8_t cam2LaneNum = 0;
int8_t crossSectionFlag = 0;
int8_t schoolZoneFlag = 0;
int8_t errorFlag = 0;

int8_t laneProccessEndFlag = 0;


/*#### Function Declaration #########*/
void CAM_PIT4_ISR(void){
	CAM_MICRO_SEC += 20;//10;//recall every 10 microsec
	if(CAM_RUN_MODE == 1){
		CAM_RUN();
	}
	else{//if(CAM_RUN_MODE == 2){
		//CAM_RUN2();
	}
	PIT.CH[4].TFLG.R = 0x00000001;//PIT ch4 reset
}

void CAM_Init(void)
{	
	TIME_CONST= 1;//(16<<NGV_DMU_SYSCLK);
	DELAY_SI= DELAY_SI / TIME_CONST; //micosec
	DELAY_CLK= DELAY_CLK / TIME_CONST;//micosec
	DELAY_WAIT = DELAY_WAIT / TIME_CONST;//micosec
	CAM_TIME_TOT= DELAY_CLK*128*2*2 + DELAY_WAIT; //128*2 cause up & down//micro sec 
	CAM_RESET_TIMER();
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

/* cam ISR의해 주기적으로 불러와 져서 PIT0 따라 상태 확인하는 식으로 카메라 작동 시키는 함수*/
void CAM_RUN(void)
{
	//CAM_SI();/*signal(trigger)신호(카메라 작동 시작신호) 내보내기*/
	if(CAM_SI_FLAG==1){
		GPIO_Set(CAM_SI_PIN,1);
		t0_CAM_SI = CAM_MICRO_SEC; // 현재 시각 가져오기
		CAM_SI_FLAG = 2;
	}
	if(CAM_SI_FLAG==2){
		if( (CAM_MICRO_SEC - t0_CAM_SI) >= DELAY_SI){
			GPIO_Set(CAM_SI_PIN,0);
		}
		/*clock 신호(셀 값 읽어오는 신호) 내보내기*/
		CAM_Clock();// For 128 + 1 Clock Pulse
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

/*clock 신호(필셀 값 읽어오는 신호) 내보내기*/
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
		CAM_UPDATE_DATA();//temp to data (cam_read[])
		CAM_RAN_FLAG = 1;
		// put signal process function, steering function here. we can do that right after camera read 128 pixcel. triggered by PIT
		// but if that process take longer that unit PIT time interrupt can be overlapped.
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


/* CAM_MAX,MIN,TRESHOLD,AVG 삭제.*/
/*uint16_t CAM_TRESHLOLD(int16_t pixelnum)
{
	return CAM_AVG();
}

uint16_t CAM_AVG(void)
{
	uint16_t CAM_READ_AVG = (CAM_MIN()+CAM_MAX())/2;
	return CAM_READ_AVG;// + 20;//+10 to too dark
}
*/
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

/*삭제 무필요
//int16_t CAM_MAX(void){
//	int16_t max_value = maxPhoto;//CAM_READ1[0];
//	/*for(i_cam=0; i_cam < (NUM_OF_PIXEL); i_cam++){
//		if(max_value < CAM_READ1[i_cam])
//			max_value = CAM_READ1[i_cam];
//	}*/
//	return max_value;
//}
//
//int16_t CAM_MIN(void){
//	int16_t min_value = minPhoto;//CAM_READ1[0];
//	/*for(i_cam=0; i_cam < (NUM_OF_PIXEL); i_cam++){
//		if(min_value > CAM_READ1[i_cam])
//			min_value = CAM_READ1[i_cam];
//	}*/
//	return min_value;
//}
/*###############  Lane Detecting  ####################*/
/* with camera data array CAM_READ change LANE_PIXEL_INDEX 1 and 2 value
 * and change lane status flags*/
/* lane status*/

char string_temp2[256] = " ";

void laneProcess2( void ){
	int16_t index_DropStart = 0; /* drop start index */
	int16_t index_DropEnd = 0; /* drop end index*/
	int16_t index_LaneStart = 0;
	int16_t index_LaneEnd = 0;
	
	uint8_t flag_disconti = 0;
	uint8_t flag_LaneEnd = 0;
	uint8_t flag_firstLane = 1;//is first pixels high or low, usually high, 
	
	uint16_t lanePixelCount = 0;
	
	int16_t i = 0, j1=0, j2=0;//for for문
	int16_t maxPhoto1 = 0;
	int16_t minPhoto1 = 1000;
	int16_t maxDiff1 = 0; /* slopeerence between maxphoto and minPhoto */
	//int16_t maxPhoto2 = 0;
	//int16_t minPhoto2 = 1000;
	//int16_t maxDiff2 = 0;
	int32_t slope = 0;
	int32_t totalDropSlope = 0;
	int16_t CAM_1_Status[NUM_OF_PIXEL] = {}; // CAM_Status, -1 dark, 1 bright, 2 up, -2 drop
	//int16_t CAM_2_Status[NUM_OF_PIXEL] = {};
	char camOutTest1[12] = " ";
	char camOutTest2[12] = " ";
	

	cam1LaneNum = 0;
	//cam2LaneNum = 0;
	crossSectionFlag = 0;
	schoolZoneFlag = 0;
	laneProccessEndFlag = 0;
	
	for ( i= 0; i < 3; i ++)//lane position initialisation
	{
		cam1LanePosition[i] = 0;
		//cam2LanePosition[i] = 0;
	}
	
	/*################ FOR TEST #######################*/
	//CAM_RAN_FLAG = 1;

	if ( CAM_RAN_FLAG )
	{
		UART_print( "Camera 1" );
		//################# Cam 1 processing###################
		/*find max and min vlaue of 128 pixels of camera 1 */
		minPhoto1 = 1000; maxPhoto1 = 0;
		for ( i = 0; i < NUM_OF_PIXEL; i++ ){ //cam data get{
			CAM_READ1_STORE[i] = CAM_READ1[i];
			itoa(i, camOutTest1);
			UART_print(camOutTest1);
			UART_print(":");
			itoa(CAM_READ1_STORE[i], camOutTest1);
			UART_println(camOutTest1);
			
			if ( (i > (START_PIXEL - 1) ) && (i < (END_PIXEL + 1)) ){
				if ( minPhoto1 > CAM_READ1_STORE[i] ){
					minPhoto1 = CAM_READ1_STORE[i];
				}
				if ( maxPhoto1 < CAM_READ1_STORE[i] ){
					maxPhoto1 = CAM_READ1_STORE[i];
				}
			}
		}
		
		maxDiff1 = maxPhoto1 - minPhoto1;
		
		//print max min
		itoa(maxPhoto1, camOutTest1);
		UART_print("maxPhoto = "); UART_println(camOutTest1);
		itoa(minPhoto1, camOutTest2);
		UART_print("minPhoto = "); UART_println(camOutTest2);
		itoa(maxDiff1, camOutTest1);
		UART_print("maxDiff = "); UART_println(camOutTest1);
		
		//drop or up
		for ( i = START_PIXEL-1 ; i < END_PIXEL - 1; i++ ){
			slope = (CAM_READ1_STORE[i] - CAM_READ1_STORE[i+1]);
			
			if( slope > (maxDiff1/EDGE_RATIO) )//drop  = -2
				CAM_1_Status[i+1] = -2;
			else if( slope < -(maxDiff1/EDGE_RATIO))//up 2
				CAM_1_Status[i+1] = 2;
			
			itoa(CAM_1_Status[i+1], camOutTest1);
			UART_print(camOutTest1);
		}
		
		//find line
		j1=0;
		for( i = START_PIXEL-1 ; i < END_PIXEL; i++ ){//( i = 0 ; i < NUM_OF_PIXEL-1; i++ ){
			if(CAM_1_Status[i] == -2){
				index_LaneStart = i+1;
			}
			else if(CAM_1_Status[i] == 2){
				index_LaneEnd = i+1;
			}
			
			if( (index_LaneEnd - index_LaneStart) > CAM_MAX_LANE_NUM){
				cam1LanePosition[j1] = (index_LaneEnd + index_LaneStart)/2;
				j1++;
			}
		}
		
		for ( i= 0; i < 3; i ++)//lane position initialisation
		{
			cam1LanePos_back[i] = cam1LanePosition[i];
		}

		UART_println("LanePosition Cam1");
		itoa(cam1LanePosition[0], string_temp2);
		UART_print(string_temp2); UART_print(", ");
		itoa(cam1LanePosition[1], string_temp2);
		UART_print(string_temp2); UART_print(", ");
		itoa(cam1LanePosition[2], string_temp2);
		UART_print(string_temp2); UART_print(", ");
	}
}

void laneProcess22( void ){
	int16_t index_DropStart = 0; /* drop start index */
	int16_t index_DropEnd = 0; /* drop end index*/
	int16_t index_LaneStart = 0;
	int16_t index_LaneEnd = 0;
	
	uint8_t flag_disconti = 0;
	uint8_t flag_LaneEnd = 0;
	uint8_t flag_firstLane = 1;//is first pixels high or low, usually high, 
	
	uint16_t lanePixelCount = 0;
	
	int16_t i = 0, j1=0, j2=0;//for for문
	int16_t maxPhoto1 = 0;
	int16_t minPhoto1 = 1000;
	//int16_t maxPhoto2 = 0;
	//int16_t minPhoto2 = 1000;
	int16_t maxDiff1 = 0; /* slopeerence between maxphoto and minPhoto */
	//int16_t maxDiff2 = 0;
	int32_t slope = 0;
	int32_t totalDropSlope = 0;
	//int16_t CAM_1_Status[NUM_OF_PIXEL] = {}; // CAM_Status, -1 dark, 1 bright, 2 up, -2 drop
	int16_t CAM_2_Status[NUM_OF_PIXEL] = {};
	char camOutTest1[12] = " ";
	char camOutTest2[12] = " ";
	

	//cam1LaneNum = 0;
	cam2LaneNum = 0;
	crossSectionFlag = 0;
	schoolZoneFlag = 0;
	laneProccessEndFlag = 0;
	
	for ( i= 0; i < 3; i ++)//lane position initialisation
	{
		//cam1LanePosition[i] = 0;
		cam2LanePosition[i] = 0;
	}
	
	/*################ FOR TEST #######################*/
	//CAM_RAN_FLAG = 1;

	if ( CAM_RAN_FLAG )
	{
		UART_print( "Camera 2" );
		//################# Cam 1 processing###################
		/*find max and min vlaue of 128 pixels of camera 1 */
		minPhoto1 = 1000; maxPhoto1 = 0;
		for ( i = 0; i < NUM_OF_PIXEL; i++ ){ //cam data get{
			CAM_READ2_STORE[i] = CAM_READ2[i];
			itoa(i, camOutTest1);
			UART_print(camOutTest1);
			UART_print(":");
			itoa(CAM_READ2_STORE[i], camOutTest1);
			UART_println(camOutTest1);
			
			if ( (i > (START_PIXEL - 1) ) && (i < (END_PIXEL + 1)) ){
				if ( minPhoto1 > CAM_READ2_STORE[i] ){
					minPhoto1 = CAM_READ2_STORE[i];
				}
				if ( maxPhoto1 < CAM_READ2_STORE[i] ){
					maxPhoto1 = CAM_READ2_STORE[i];
				}
			}
		}
		
		maxDiff1 = maxPhoto1 - minPhoto1;
		
		//print max min
		itoa(maxPhoto1, camOutTest1);
		UART_print("maxPhoto = "); UART_println(camOutTest1);
		itoa(minPhoto1, camOutTest2);
		UART_print("minPhoto = "); UART_println(camOutTest2);
		itoa(maxDiff1, camOutTest1);
		UART_print("maxDiff = "); UART_println(camOutTest1);
		
		//drop or up
		for ( i = START_PIXEL-1 ; i < END_PIXEL - 1; i++ ){
			slope = (CAM_READ2_STORE[i] - CAM_READ2_STORE[i+1]);
			
			if( slope > (maxDiff1/EDGE_RATIO) )//drop  = -2
				CAM_2_Status[i+1] = -2;
			else if( slope < -(maxDiff1/EDGE_RATIO))//up 2
				CAM_2_Status[i+1] = 2;
			
			itoa(CAM_2_Status[i+1], camOutTest1);
			UART_print(camOutTest1);
		}
		
		//find line
		j1=0;
		for( i = START_PIXEL-1 ; i < END_PIXEL; i++ ){//( i = 0 ; i < NUM_OF_PIXEL-1; i++ ){
			if(CAM_2_Status[i] == -2){
				index_LaneStart = i+1;
			}
			else if(CAM_2_Status[i] == 2){
				index_LaneEnd = i+1;
			}
			
			if( (index_LaneEnd - index_LaneStart) > CAM_MAX_LANE_NUM){
				cam2LanePosition[j1] = (index_LaneEnd + index_LaneStart)/2;
				j1++;
			}
		}

		for ( i= 0; i < 3; i ++)//lane position initialisation
		{
			cam2LanePos_back[i] = cam2LanePosition[i];
		}
		
		UART_println("LanePosition Cam1");
		itoa(cam2LanePosition[0], string_temp2);
		UART_print(string_temp2); UART_print(", ");
		itoa(cam2LanePosition[1], string_temp2);
		UART_print(string_temp2); UART_print(", ");
		itoa(cam2LanePosition[2], string_temp2);
		UART_print(string_temp2); UART_print(", ");
	}
}

/*test 포함
 * lane : LED1
 * school: LED2
 * 교차로: LED3
 * 예외경우: LED4
 */
void laneStatusUpdate( void )
{
	if ( cam1LaneNum + cam2LaneNum == 0 ) 
	{
		crossSectionFlag = 1;
		GPIO_Set( LED_2, 0);
	}
	else if ( cam1LaneNum + cam2LaneNum  < 2 ) ;//errorFlag = 1; //GPIO_Set( LED_4, 0 ); //경우의 수 밖!!! 1개여도 따라가면 그만.
	else if ( (cam1LaneNum == 1) && (cam2LaneNum == 1) ) ;//GPIO_Set( LED_1, 0 );//정상적 lane
	else if ( !schoolZoneFlag && !crossSectionFlag ) // 두꺼운거 잡혔을 때는 라인 여러개 생각해야 하므로
	{
		if ( cam1LaneNum > 2 || cam2LaneNum > 2 ) errorFlag = 1; //경우의 수 밖!!!
		else if ( cam1LaneNum == 2 || cam2LaneNum == 2 ) 
		{
			//cam1LanePosition[0] = cam1LanePosition[cam1LaneNum - 1];
			//cam2LanePosition[0] = cam2LanePosition[cam2LaneNum - 1];
			cam1LanePos_back[0] = cam1LanePosition[cam1LaneNum - 1];
			cam2LanePos_back[0] = cam2LanePosition[cam2LaneNum - 1];
		}	 
	}
}

int16_t cam1LanePositionReturn ( int8_t laneNum )
{
	//for test
	if (cam1LaneNum == 0) return -1;
	else return cam1LanePos_back[laneNum];//cam1LanePosition[laneNum];
	/*
	if ( cam1LaneNum == 1) return cam1LanePosition[0];
	else if ( cam2LaneNum == 0 ) return -1;
	*/

}

int16_t cam2LanePositionReturn ( int8_t laneNum )
{
	if (cam2LaneNum == 0) return -1;
	else return cam2LanePos_back[laneNum];//cam2LanePosition[laneNum];
	/*
	if ( cam2LaneNum == 1) return cam2LanePosition[0];
	else if ( cam2LaneNum == 0 ) return -1;
	*/
}

int8_t ifLaneProcessEnd ( void )
{
	return laneProccessEndFlag;
}

int8_t ifSchoolZone ( void )
{
	return schoolZoneFlag;
}

int8_t ifCrossSection ( void )
{
	return crossSectionFlag;
}

int8_t ifError ( void )
{
	return errorFlag;
}

int8_t cam1LaneNumReturn (void)
{
	return cam1LaneNum;
}

int8_t cam2LaneNumReturn ( void )
{
	return cam2LaneNum;
}

/* efficient cameara max and min
 * find max and min value of pixels together and change global variable 하려 했으나 global variable 넘 많아지는거 같아서 line process */
/*
void eff_CAM_MAXMIN ( void )
 {
	 uint16_t min_value = CAM_READ1[0];
	 uint16_t max_value = CAM_READ1[0];
	 	for(i_cam=0; i_cam < (NUM_OF_PIXEL); i_cam++){
	 		if(min_value > CAM_READ1[i_cam])
	 			min_value = CAM_READ1[i_cam];
	 	}
	 	return min_value;
 }*/
