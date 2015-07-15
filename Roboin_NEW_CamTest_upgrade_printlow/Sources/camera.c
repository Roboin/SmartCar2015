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
vuint16_t CAM_READ1[NUM_OF_PIXEL] = {69, 77, 85, 91, 97, 101, 108, 117, 116, 119, 124, 125, 131, 134, 133, 135, 131, 140, 140, 140, 144, 145, 147, 148, 150, 147, 150, 152, 153, 152, 153, 156, 157, 157, 155, 158, 160, 160, 161, 162, 160, 159, 160, 161, 159, 160, 159, 160, 161, 162, 158, 157, 158, 160, 160, 161, 161, 160, 161, 161, 161, 163, 161, 160, 160, 156, 159, 159, 157, 157, 156, 157, 156, 156, 155, 155, 152, 154, 158, 156, 153, 153, 152, 155, 155, 153, 153, 153, 152, 150, 151, 149, 148, 147, 146, 146, 144, 143, 144, 137, 140, 139, 134, 134, 131, 130, 130, 129, 119, 93, 64, 57, 56, 55, 57, 71, 97, 102, 98, 92, 87, 84, 78, 71, 64, 58, 52, 3};
vuint16_t CAM_READ2[NUM_OF_PIXEL] = {48, 52, 58, 65, 66, 68, 70, 74, 72, 69, 73, 84, 86, 88, 80, 69, 69, 84, 91, 96, 98, 99, 96, 99, 94, 92, 107, 110, 99, 97, 101, 107, 109, 107, 106, 115, 116, 114, 117, 115, 121, 124, 120, 117, 120, 116, 116, 125, 122, 125, 116, 117, 120, 118, 116, 109, 98, 113, 108, 115, 120, 114, 112, 115, 115, 114, 107, 118, 104, 101, 122, 121, 115, 116, 124, 119, 106, 140, 163, 166, 163, 163, 158, 110, 72, 70, 69, 69, 69, 104, 155, 160, 160, 158, 160, 160, 157, 154, 154, 157, 153, 151, 149, 149, 148, 141, 145, 143, 142, 142, 139, 140, 131, 114, 138, 129, 127, 112, 96, 100, 105, 100, 89, 85, 77, 70, 62, 5};
int16_t i_cam=0;// for for반복문

int16_t cam1LanePosition[CAM_MAX_LANE_NUM] = {};
int16_t cam2LanePosition[CAM_MAX_LANE_NUM] = {};
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
		//CAM_RUN();
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
void laneProcess( void )
{

	int16_t index_DropStart = 0; /* drop start index */
	int16_t index_DropEnd = 0; /* drop end index*/
	int16_t index_LaneStart = 0;
	int16_t index_LaneEnd = 0;
	
	uint8_t flag_disconti = 0; /* drop successiveness flag 불연속1 연속0 //flat or parallel oe suc or disconti*/
	uint8_t flag_LaneEnd = 0;
	uint8_t dropCheckPatience;
	
	uint16_t lanePixelCount = 0;
	
	//uint16_t maxPhoto = 0, minPhoto = 1000;전역변수로/ /* max and min vlaue of 128 pixels*/
	int16_t i = 0;//for for문
	int16_t maxPhoto = 0;
	int16_t minPhoto = 1000;
	int16_t maxDiff = 0; /* slopeerence between maxphoto and minPhoto */
	int32_t slope = 0;
	int32_t totalDropSlope = 0;
	char camOutTest1[12] = " ";
	char camOutTest2[12] = " ";
	char string_tempt[12] = " ";
	

	cam1LaneNum = 0;
	cam2LaneNum = 0;
	crossSectionFlag = 0;
	schoolZoneFlag = 0;
	laneProccessEndFlag = 0;
	
	for ( i= 0; i < 3; i ++)
	{
		cam1LanePosition[i] = 0;
		cam2LanePosition[i] = 0;
	}
	
	/*################### FOR TEST #########################*/
	CAM_RAN_FLAG = 1;
	/*################### FOR TEST #########################*/
	

	if ( CAM_RAN_FLAG )
	{
		//print for test			
		UART_println("cam1 data");
		
		
		/*find max and min vlaue of 128 pixels of camera 1 */
		for ( i = 0; i < NUM_OF_PIXEL; i++ ) //0-127 >>> 1-128
		{
			CAM_READ1_STORE[i] = CAM_READ1[i];
			
			//print for test
			itoa(CAM_READ1_STORE[i], camOutTest1);
			UART_print(camOutTest1); UART_print(", ");
			
								
			if ( (i > (START_PIXEL - 1) ) && (i < (END_PIXEL + 1)) )
			{
				
				if ( minPhoto > CAM_READ1_STORE[i] ) minPhoto = CAM_READ1_STORE[i];
				if ( maxPhoto < CAM_READ1_STORE[i] ) maxPhoto = CAM_READ1_STORE[i];	
			}
		}
		
		UART_println("");
		
		//print for test
		itoa(minPhoto, camOutTest1);
		UART_print("min: "); UART_println(camOutTest1); 
		itoa(maxPhoto, camOutTest1);
		UART_print("max: "); UART_println(camOutTest1); 
		
		maxDiff = maxPhoto - minPhoto;//최대-최소 밝기차
		
		/* find location of line in pixel in camera 1*/
		
				
		maxDiff = maxPhoto - minPhoto;//최대-최소 밝기차
					
		for ( i = START_PIXEL - 3 ; i < END_PIXEL + 3; i++ )//( i = 1; i < NUM_OF_PIXEL; i++ )
		{
			slope = CAM_READ1_STORE[i+1] - CAM_READ1_STORE[i];//calc just once
			
			itoa((int32_t)slope, string_temp2);
			UART_print(string_temp2);
			UART_print("=s : ");
			
			if ( lanePixelCount == 0 )
			{	
				itoa(i, string_tempt);
				UART_print(string_tempt);
				UART_println(": Lane pixel = 0");
				if ( !flag_disconti )// 연속
				{
					
					if ( abs( slope ) > (maxDiff/EDGE_RATIO))//(maxDiff/EDGE_RATIO) )// successive pixel slope > max slope 1/6 다시 불연속확인
					{	/* cur index save, flag_disconti up */
						index_DropStart = i;//불연속이면 DropStart 기록.
						flag_disconti = 1; 
						
						itoa(i, string_tempt);
						UART_print(string_tempt); UART_println(": drop started?");
					}
				}
				
				else//if ( flag_disconti ){//불연속이었다. 처음에는 연속이므로 저 밑으로 감
				{
					itoa(i, string_tempt);
					UART_print(string_tempt);
					UART_println(": drop has started?");
					if ( abs(slope) > (maxDiff/EDGE_RATIO) ) 
					{
						itoa(i, string_tempt);
						UART_print(string_tempt);
						UART_println(": drop is going on~ "); 
					}//*1.6667 = 1/6, 다시 불연속확인, 불연속이면 씹음(필터링)
					else
					{//연속 , 어두운지 확인.
						itoa(i, string_tempt);
						UART_print(string_tempt);
						UART_println(": drop has ended");
						
						flag_disconti = 0;//연속
						index_DropEnd = i; 
						totalDropSlope = CAM_READ1_STORE[index_DropStart] - CAM_READ1_STORE[index_DropEnd];
						//CAM_READ1_STORE[index_DropStart] 아직 밝은값, 이 다음 픽셀부터 어두움. 
						//CAM_READ1_STORE[index_DropEnd]가 DropStart보다 maxDiff/2 이상 어두운가.
						
						if (  abs(totalDropSlope) > (maxDiff / TOTAL_EDGE_RATIO) )
						{
							dropCheckPatience = DROP_CHECK_PATIENCE; /* drop check 끝났고 맞다 판별되었으니 초기화 */
							
							itoa(i, string_tempt);
							UART_print(string_tempt);
							UART_println(": drop is sufficient");
							
							//UART_print("  totalDropSlope1 : ");
							//itoa((int32_t)totalDropSlope, string_temp2);
							//UART_println(string_temp2);
							
							
							
							// if 1/20 of global slope continuous for more than 4pixel and another rise occur it is line
							if ( abs(slope) < (maxDiff / LANE_RATIO) )// *0.20 = /5 //5%이내의 오차인지로 lane인지 판별
							{
								
								itoa(i, string_tempt);
								UART_print(string_tempt);
								UART_println(": steady appear right after drop");
								
								
								
								if ( flag_LaneEnd )//lane이 끝났는가, 처음에는 0 아래 else부터.
								{
									index_LaneEnd = index_DropEnd;//index_DropStart;
									cam1LaneNum ++;
									
					//#########################################################	
									itoa(i, string_tempt);
									UART_print(string_tempt);
									UART_println("lane ended");
									UART_print("  L1S : ");
									itoa((int32_t)index_LaneStart, string_temp2);
									UART_print(string_temp2);
									UART_print("  L1E : ");
									itoa((int32_t)index_LaneEnd, string_temp2);
									UART_println(string_temp2);
					//#########################################################
									
									if ( cam1LaneNum > 3 )
									{
										itoa(i, string_tempt);
										UART_print(string_tempt);
										UART_println(": err: more than 3 lane deteted!");
										//GPIO_Set( LED_4, 0 );
										errorFlag = 1; //경우의 수 밖!!!
										return; 
									}
									else// cam1LaneNum <= 3
									{
										itoa(i, string_tempt);
										UART_print(string_tempt);
										UART_println(": lane position saving~");
										
										cam1LanePosition[cam1LaneNum - 1] = ( (index_LaneEnd + index_LaneStart)>>1);// / 2 );
										index_LaneStart = 0;
										index_LaneEnd = 0;
										flag_LaneEnd = 0;
										lanePixelCount = 0;
										index_DropStart = 0;
										index_DropEnd = 0;
									}
								}
								else
								{//flag_LaneEnd == 0
									itoa(i, string_tempt);
									UART_print(string_tempt);
									UART_println(": lane start~?");
									if (totalDropSlope > 0)
									{
										itoa(i, string_tempt);
										UART_print(string_tempt);
										UART_println(": yes, it was not up!");
										lanePixelCount ++;
									}
								}
							}
							else
							{ //abs(slope) >= (maxDiff / LANE_RATIO)5보다 변동이 크냐 
								itoa(i, string_tempt);
								UART_print(string_tempt);
								UART_println(": there was drop but not steady after that");
								//index_DropStart = 0;
								//index_DropEnd = 0;
							}
						}
						else
						{
							itoa(i, string_tempt);
							UART_print(string_tempt);
							UART_println(": drop was not sufficient but");
							
							if ( dropCheckPatience > 0)
							{
								if ( abs(slope) < abs((CAM_READ1_STORE[i] - CAM_READ1_STORE[i-DROP_CHECK_PATIENCE+dropCheckPatience]) / DROP_VARIATION_RATIO) )
								{
									flag_disconti = 1;
									dropCheckPatience --;
									
									itoa(i, string_tempt);
									UART_print(string_tempt);
									UART_println(": i will give more chance");
								}
								else
								{
									flag_disconti = 0;
									dropCheckPatience = DROP_CHECK_PATIENCE;
									/* 다시 안돌아가도 상관 없을 듯? 어짜피 기울기 안 충분했다는 소리이니
									i = i - DROP_CHECK_PATIENCE;*/
									
									itoa(i, string_tempt);
									UART_print(string_tempt);
									UART_println(": no need to give chance");
								}
							}
							else
							{
								flag_disconti = 0;
								dropCheckPatience = DROP_CHECK_PATIENCE;
								
								/* 다시 안돌아가도 상관 없을 듯? 어짜피 기울기 안 충분했다는 소리이니
								i = i - DROP_CHECK_PATIENCE;*/
								
								itoa(i, string_tempt);
								UART_print(string_tempt);
								UART_println(": drop was not sufficient");
							}
						}
					}
				}
			}
			//lanePixelCount
			else 
			{
				itoa(i, string_tempt);
				UART_print(string_tempt);
				UART_println(": 0 < lane pixel");
				if ( abs(slope) < (maxDiff/LANE_RATIO) )
				{//25%변동이내 계속 어두운 채인가.
					itoa(i, string_tempt);
					UART_print(string_tempt);
					UART_println(": still steady");
					lanePixelCount ++;
				}
				else
				{ //abs(slope) >= (maxDiff * LANE_RATIO)25보다 변동이 크냐 
					if ( lanePixelCount < LANE_WIDTH_PIXEL )
					{
						itoa(i, string_tempt);
						UART_print(string_tempt);
						UART_println(": lane width is too narrow, it is not lane");
						index_DropStart = 0;
						index_DropEnd = 0;
						lanePixelCount = 0;
					}
					else if ( lanePixelCount < THICK_LANE_WIDTH_PIXEL )
					{
						itoa(i, string_tempt);
						UART_print(string_tempt);
						UART_println(": 4<lane pixel<14");
						flag_LaneEnd = 1;
						index_LaneStart = index_DropStart;//index_DropEnd;
						i --;
						
						index_DropEnd = 0;
						index_DropStart = 0;
						lanePixelCount = 0;
						
					}
					else
					{
						itoa(i, string_tempt);
						UART_print(string_tempt);
						UART_println(": 14<lane pixel -> ");
						
						flag_LaneEnd = 1;
						index_LaneStart = index_DropStart;//index_DropEnd;
						schoolZoneFlag = 1;
						crossSectionFlag = 1;
						i--;
						
						index_DropEnd = 0;
						index_DropStart = 0;
						lanePixelCount = 0;
					}
				}
			}
			
		}//end of for1

	/*####################### find location of line in pixel in camera 2 ###########################################*/
		//print for test
		UART_println("");
		UART_println("cam2 data");
		
		
		/*camera2 initialization*/
		lanePixelCount = 0;
		flag_LaneEnd = 0;
		flag_disconti = 0;
		minPhoto = 1000;
		maxPhoto = 0;

		
		/*camera 2 min max*/
		for ( i = 0; i < NUM_OF_PIXEL; i++ ) //0-127 >>> 1-128
			{
				CAM_READ2_STORE[i] = CAM_READ2[i];
				
				//print for test
				itoa(CAM_READ2_STORE[i], camOutTest1);
				UART_print(camOutTest1); UART_print(", ");
				
									
				if ( (i > (START_PIXEL - 1) ) && (i < (END_PIXEL + 1)) )
				{
					
					if ( minPhoto > CAM_READ2_STORE[i] ) minPhoto = CAM_READ2_STORE[i];
					if ( maxPhoto < CAM_READ2_STORE[i] ) maxPhoto = CAM_READ2_STORE[i];	
				}
			}
		
		UART_println("");
		
		//print for test
		itoa(minPhoto, camOutTest1);
		UART_print("min: "); UART_print(camOutTest1); 
		itoa(maxPhoto, camOutTest1);
		UART_print("max: "); UART_print(camOutTest1); 
		
		maxDiff = maxPhoto - minPhoto;//최대-최소 밝기차
		
		
		
		/* find location of line in pixel in camera 2*/
		for ( i = START_PIXEL - 3 ; i < END_PIXEL + 3; i++ )//( i = 1; i < NUM_OF_PIXEL; i++ )
		{
			slope = CAM_READ2_STORE[i+1] - CAM_READ2_STORE[i];//calc just once
			
			itoa((int32_t)slope, string_temp2);
			UART_print(string_temp2);
			UART_print("=s : ");
			
			if ( lanePixelCount == 0 )
			{	
				itoa(i, string_tempt);
				UART_print(string_tempt);
				UART_println(": Lane pixel = 0");
				if ( !flag_disconti )// 연속
				{
					
					if ( abs( slope ) > (maxDiff/EDGE_RATIO))//(maxDiff/EDGE_RATIO) )// successive pixel slope > max slope 1/6 다시 불연속확인
					{	/* cur index save, flag_disconti up */
						index_DropStart = i;//불연속이면 DropStart 기록.
						flag_disconti = 1; 
						
						itoa(i, string_tempt);
						UART_print(string_tempt); UART_println(": drop started?");
					}
				}
				
				else//if ( flag_disconti ){//불연속이었다. 처음에는 연속이므로 저 밑으로 감
				{
					itoa(i, string_tempt);
					UART_print(string_tempt);
					UART_println(": drop has started?");
					if ( abs(slope) > (maxDiff/EDGE_RATIO) ) 
					{
						itoa(i, string_tempt);
						UART_print(string_tempt);
						UART_println(": drop is going on~ "); 
					}//*1.6667 = 1/6, 다시 불연속확인, 불연속이면 씹음(필터링)
					else
					{//연속 , 어두운지 확인.
						itoa(i, string_tempt);
						UART_print(string_tempt);
						UART_println(": drop has ended");
						
						flag_disconti = 0;//연속
						index_DropEnd = i; 
						totalDropSlope = CAM_READ2_STORE[index_DropStart] - CAM_READ2_STORE[index_DropEnd];
						//CAM_READ2_STORE[index_DropStart] 아직 밝은값, 이 다음 픽셀부터 어두움. 
						//CAM_READ2_STORE[index_DropEnd]가 DropStart보다 maxDiff/2 이상 어두운가.
						
						if (  abs(totalDropSlope) > (maxDiff / TOTAL_EDGE_RATIO) )
						{
							dropCheckPatience = DROP_CHECK_PATIENCE; /* drop check 끝났고 맞다 판별되었으니 초기화 */
							
							itoa(i, string_tempt);
							UART_print(string_tempt);
							UART_println(": drop is sufficient");
							
							//UART_print("  totalDropSlope2 : ");
							//itoa((int32_t)totalDropSlope, string_temp2);
							//UART_println(string_temp2);
							
							
							
							// if 1/20 of global slope continuous for more than 4pixel and another rise occur it is line
							if ( abs(slope) < (maxDiff / LANE_RATIO) )// *0.20 = /5 //5%이내의 오차인지로 lane인지 판별
							{
								
								itoa(i, string_tempt);
								UART_print(string_tempt);
								UART_println(": steady appear right after drop");
								
								
								
								if ( flag_LaneEnd )//lane이 끝났는가, 처음에는 0 아래 else부터.
								{
									index_LaneEnd = index_DropEnd;//index_DropStart;
									cam2LaneNum ++;
									
					//#########################################################	
									itoa(i, string_tempt);
									UART_print(string_tempt);
									UART_println("lane ended");
									UART_print("  L2S : ");
									itoa((int32_t)index_LaneStart, string_temp2);
									UART_print(string_temp2);
									UART_print("  L2E : ");
									itoa((int32_t)index_LaneEnd, string_temp2);
									UART_println(string_temp2);
					//#########################################################
									
									if ( cam2LaneNum > 3 )
									{
										itoa(i, string_tempt);
										UART_print(string_tempt);
										UART_println(": err: more than 3 lane deteted!");
										//GPIO_Set( LED_4, 0 );
										errorFlag = 1; //경우의 수 밖!!!
										return; 
									}
									else// cam2LaneNum <= 3
									{
										itoa(i, string_tempt);
										UART_print(string_tempt);
										UART_println(": lane position saving~");
										
										cam2LanePosition[cam2LaneNum - 1] = ( (index_LaneEnd + index_LaneStart)>>1);// / 2 );
										index_LaneStart = 0;
										index_LaneEnd = 0;
										flag_LaneEnd = 0;
										lanePixelCount = 0;
										index_DropStart = 0;
										index_DropEnd = 0;
									}
								}
								else
								{//flag_LaneEnd == 0
									itoa(i, string_tempt);
									UART_print(string_tempt);
									UART_println(": lane start~?");
									if (totalDropSlope > 0)
									{
										itoa(i, string_tempt);
										UART_print(string_tempt);
										UART_println(": yes, it was not up!");
										lanePixelCount ++;
									}
								}
							}
							else
							{ //abs(slope) >= (maxDiff / LANE_RATIO)5보다 변동이 크냐 
								itoa(i, string_tempt);
								UART_print(string_tempt);
								UART_println(": there was drop but not steady after that");
								//index_DropStart = 0;
								//index_DropEnd = 0;
							}
						}
						else
						{
							itoa(i, string_tempt);
							UART_print(string_tempt);
							UART_println(": drop was not sufficient but");
							
							if ( dropCheckPatience > 0)
							{
								if ( slope < ((CAM_READ2_STORE[i] - CAM_READ2_STORE[i-DROP_CHECK_PATIENCE+dropCheckPatience]) / DROP_VARIATION_RATIO) )
								{
									flag_disconti = 1;
									dropCheckPatience --;
									
									itoa(i, string_tempt);
									UART_print(string_tempt);
									UART_println(": i will give more chance");
								}
								else
								{
									flag_disconti = 0;
									dropCheckPatience = DROP_CHECK_PATIENCE;
									/* 다시 안돌아가도 상관 없을 듯? 어짜피 기울기 안 충분했다는 소리이니
									i = i - DROP_CHECK_PATIENCE;*/
									
									itoa(i, string_tempt);
									UART_print(string_tempt);
									UART_println(": no need to give chance");
								}
							}
							else
							{
								flag_disconti = 0;
								dropCheckPatience = DROP_CHECK_PATIENCE;
								
								/* 다시 안돌아가도 상관 없을 듯? 어짜피 기울기 안 충분했다는 소리이니
								i = i - DROP_CHECK_PATIENCE;*/
								
								itoa(i, string_tempt);
								UART_print(string_tempt);
								UART_println(": drop was not sufficient");
							}
						}
					}
				}
			}
			//lanePixelCount
			else 
			{
				itoa(i, string_tempt);
				UART_print(string_tempt);
				UART_println(": 0 < lane pixel");
				if ( abs(slope) < (maxDiff/LANE_RATIO) )
				{//25%변동이내 계속 어두운 채인가.
					itoa(i, string_tempt);
					UART_print(string_tempt);
					UART_println(": still steady");
					lanePixelCount ++;
				}
				else
				{ //abs(slope) >= (maxDiff * LANE_RATIO)25보다 변동이 크냐 
					if ( lanePixelCount < LANE_WIDTH_PIXEL )
					{
						itoa(i, string_tempt);
						UART_print(string_tempt);
						UART_println(": lane width is too narrow, it is not lane");
						index_DropStart = 0;
						index_DropEnd = 0;
						lanePixelCount = 0;
					}
					else if ( lanePixelCount < THICK_LANE_WIDTH_PIXEL )
					{
						itoa(i, string_tempt);
						UART_print(string_tempt);
						UART_println(": 4<lane pixel<14");
						flag_LaneEnd = 1;
						index_LaneStart = index_DropStart;//index_DropEnd;
						i --;
						
						index_DropEnd = 0;
						index_DropStart = 0;
						lanePixelCount = 0;
						
					}
					else
					{
						itoa(i, string_tempt);
						UART_print(string_tempt);
						UART_println(": 14<lane pixel -> ");
						
						flag_LaneEnd = 1;
						index_LaneStart = index_DropStart;//index_DropEnd;
						schoolZoneFlag = 1;
						crossSectionFlag = 1;
						i--;
						
						index_DropEnd = 0;
						index_DropStart = 0;
						lanePixelCount = 0;
					}
				}
			}
			
		}//end of for2
		
	
		laneStatusUpdate();
		CAM_RAN_FLAG = 0;
		laneProccessEndFlag = 1;
		
		//print for test////////////////////////////////////////////////////////////////////////
		UART_println("");
		UART_println("cam1 lane");
		itoa((int32_t)cam1LanePositionReturn(0),string_tempt);
		UART_print(string_tempt); UART_print(" ");
		itoa((int32_t)cam1LanePositionReturn(1),string_tempt);
		UART_print(string_tempt); UART_print(" ");
		itoa((int32_t)cam1LanePositionReturn(2),string_tempt);
		UART_print(string_tempt); UART_print(" ");
		UART_println("");
		
		UART_println("");
		UART_println("cam2 lane");
		itoa((int32_t)cam2LanePositionReturn(0),string_tempt);
		UART_print(string_tempt); UART_print(" ");
		itoa((int32_t)cam2LanePositionReturn(1),string_tempt);
		UART_print(string_tempt); UART_print(" ");
		itoa((int32_t)cam2LanePositionReturn(2),string_tempt);
		UART_print(string_tempt); UART_print(" ");
		UART_println("");
		
	}	
	return;
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
	else if ( cam1LaneNum + cam2LaneNum  < 2 ) {} //errorFlag = 1; //GPIO_Set( LED_4, 0 );  1개여도 따라가면 그만.
	else if ( (cam1LaneNum == 1) && (cam2LaneNum == 1) ) {}//GPIO_Set( LED_1, 0 );//정상적 lane
	else if ( !schoolZoneFlag && !crossSectionFlag && (cam1LaneNum > 2 || cam2LaneNum > 2) // 일반 도로인데 3개 이상 감지
			|| (cam1LaneNum == 0 && cam2LaneNum == 3 ) || (cam1LaneNum == 3 && cam2LaneNum == 0)) // 스쿨존 등인데 3개 0개 감지
	{
		errorFlag = 1; //경우의 수 밖!!!
		cam2LanePosition[0] = cam2LanePosition[cam2LaneNum - 1];
	}
	else if ( cam1LaneNum > 2 || cam2LaneNum > 2 ) cam2LanePosition[0] = cam2LanePosition[cam2LaneNum - 1];
}

int16_t cam1LanePositionReturn ( int8_t laneNum )
{
	//for test
	if (cam1LaneNum == 0) return -1;
	else return cam1LanePosition[laneNum];
	/*
	if ( cam1LaneNum == 1) return cam1LanePosition[0];
	else if ( cam2LaneNum == 0 ) return -1;
	*/

}

int16_t cam2LanePositionReturn ( int8_t laneNum )
{
	if (cam2LaneNum == 0) return -1;
	else return cam2LanePosition[laneNum];
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
