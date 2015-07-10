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
#include "adc_drv.h"
#include "basetimer.h"
#include "stdlib.h"
#include "demoled.h"

//####### Pin Guide ##########
int16_t CAM_DATA_PIN1 = 4;//4=pd0
int16_t CAM_DATA_PIN2 = 5;//5=pd1//
int16_t CAM_SI_PIN= GPIO_PF0;//GPIO_PD12;
int16_t CAM_CLK_PIN= GPIO_PF1;//GPIO_PF1;

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
vuint16_t CAM_TEMP1[NUM_OF_PIXEL+1] = {};
vuint16_t CAM_READ1[NUM_OF_PIXEL] = {};
vuint16_t CAM_TEMP2[NUM_OF_PIXEL+1] = {};
vuint16_t CAM_READ2[NUM_OF_PIXEL] = {};

/* lane status*/
double cam1LanePosition[CAM_MAX_LANE_NUM] = {};
double cam2LanePosition[CAM_MAX_LANE_NUM] = {};
int8_t cam1LaneNum = 0;
int8_t cam2LaneNum = 0;
int8_t crossSectionFlag = 0;
int8_t schoolZoneFlag = 0;

int8_t laneProccessEndFlag = 0;

void CAM_Init(void)
{	
	TIME_CONST= 1;//(16<<NGV_DMU_SYSCLK);
	DELAY_SI= DELAY_SI / TIME_CONST; //micosec
	DELAY_CLK= DELAY_CLK / TIME_CONST;//micosec
	DELAY_WAIT = DELAY_WAIT / TIME_CONST;//micosec
	CAM_TIME_TOT= DELAY_CLK*128*2*2 + DELAY_WAIT; //128*2 cause up & down//micro sec 
	CAM_RESET_TIMER();
}

//Camera Interrupt Service Routine
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
		/*clock 신호(필셀 값 읽어오는 신호) 내보내기*/
		//CAM_Clock(); // For 128 + 1 Clock Pulse
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
/*clock 신호(필셀 값 읽어오는 신호) 내보내기*/
void CAM_Clock(void)
{
	if(CAM_CLK_COUNTER < (NUM_OF_PIXEL + 1) ){
		CAM_RAN_FLAG = 0;
		if( CAM_CLK_FLAG == 1 ){
			GPIO_Set(CAM_CLK_PIN,1);
			if( (CAM_MICRO_SEC - t0_CAM_CLK) >= DELAY_CLK){
				//CAM_TEMP1[CAM_CLK_COUNTER] = FADC_Get(CAM_DATA_PIN1);
				//CAM_TEMP2[CAM_CLK_COUNTER] = FADC_Get(CAM_DATA_PIN2);
				CAM_TEMP1[CAM_CLK_COUNTER] = A2D_GetSingleCh_10bit(CAM_DATA_PIN1);
				CAM_TEMP2[CAM_CLK_COUNTER] = A2D_GetSingleCh_10bit(CAM_DATA_PIN2);
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
	CAM_TEMP1[0] = A2D_GetSingleCh_10bit(CAM_DATA_PIN1);
	CAM_TEMP2[0] = A2D_GetSingleCh_10bit(CAM_DATA_PIN2);
	GPIO_Set(CAM_CLK_PIN, 0);//Dummy Clock Down
	
	for(CAM_CLK_COUNTER=1; CAM_CLK_COUNTER < NUM_OF_PIXEL+1; CAM_CLK_COUNTER++){
		GPIO_Set(CAM_CLK_PIN, 1);//CLock On
		CAM_DELAY(DELAY_CLK);
		CAM_TEMP1[CAM_CLK_COUNTER] = A2D_GetSingleCh_10bit(CAM_DATA_PIN1);
		CAM_TEMP2[CAM_CLK_COUNTER] = A2D_GetSingleCh_10bit(CAM_DATA_PIN2);
		GPIO_Set(CAM_CLK_PIN, 0);//Clock Down
		CAM_DELAY(DELAY_CLK);
		//SDBG_Printf("\n %d",PIT.CH[CAM_TIMER_CH].CVAL.R);
	}
	CAM_UPDATE_DATA();
	CAM_DELAY(DELAY_WAIT);
}

int16_t i_cam=0;
int16_t pt1 = 13;//10*100/50; //#For 100ms#
int16_t pt2 = 45;//43*100/50;
int16_t pt3 = 118;//NUM_OF_PIXEL-(NUM_OF_PIXEL-120)*100/50;
int16_t initial_value = 627;

uint16_t CAM_DATA1(int16_t pixelnum){//기울기 크면 데이터 처리.
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
	return CAM_READ1[pixelnum];
}
uint16_t CAM_DATA2(int16_t pixelnum){
	return CAM_READ2[pixelnum];
}


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

uint16_t CAM_TRESHLOLD(int16_t pixelnum)
{
	uint16_t CAM_READ_AVG = (CAM_MIN()+CAM_MAX())/2;
	if(pixelnum < pt1)
		return CAM_MAX()*3/4;
	else if( (pt1 <= pixelnum) && (pixelnum < pt2) )
		return CAM_MAX()*3/4;
	else if( (3*pt3-2*NUM_OF_PIXEL) <= pixelnum )
		return CAM_MAX()*7/8;
	else
		return CAM_READ_AVG;// + 20;//+10 to too dark
}

uint16_t CAM_AVG(void)
{
	uint16_t CAM_READ_AVG = (CAM_MIN()+CAM_MAX())/2;
	return CAM_READ_AVG;// + 20;//+10 to too dark
}

void CAM_DELAY(int32_t delay_long)
{
	int32_t delay_val = 0;
	delay_long = delay_long *(16 << NGV_DMU_SYSCLK)/PIT0_TIMER_GAP; 
	while(delay_val < delay_long)
		delay_val++;
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
/* with camera data array CAM_READ change LANE_PIXEL_INDEX 1 and 2 value
 * and change lane status flags*/
void laneProcess( void )
{
	if ( CAM_RAN_FLAG )
	{
		uint8_t dSIndex = 0; /* drop start index */
		uint8_t dEIndex = 0; /* drop end index*/
		int8_t dSucFlag = 0; /* drop successiveness flag */
		int8_t lanePixelCount = 0;
		int8_t laneEndFlag = 0;
		uint8_t laneStartIndex = 0;
		uint8_t laneEndIndex = 0;
		uint8_t i;
		uint16_t maxPhoto = 0, minPhoto = 1000; /* max and min vlaue of 128 pixels*/
		uint16_t maxDiff = 0; /* difference between maxphoto and minPhoto */
		
		crossSectionFlag = 0;
		schoolZoneFlag = 0;
		
		/*find max and min vlaue of 128 pixels of camera 1 */
		for ( i = START_PIXEL + 1; i < END_PIXEL + 1; i++ )
		{
			if ( minPhoto > CAM_READ1[i] ) minPhoto = CAM_READ1[i];
			if ( maxPhoto < CAM_READ1[i] ) maxPhoto = CAM_READ1[i];
		}
		maxDiff = maxPhoto - minPhoto;
		
		/* find location of line in pixel in camera 1*/
		for ( i = 1; i < NUM_OF_PIXEL; i++ )
		{
			if ( lanePixelCount == 0 )
			{
				if ( dSucFlag )
				{
					if ( abs( CAM_DATA1(i - 1) - CAM_DATA1(i) ) > (maxDiff) * EDGE_RATIO ) {}
					else
					{
						dSucFlag = 0;
						dEIndex = i;
						
						if ( abs( CAM_DATA1(dSIndex - 1) - CAM_DATA1(dEIndex - 1) ) > (maxDiff * TOTAL_EDGE_RATIO ) )
						{
							
							// if 1/20 of global differ continous for more than 4pixel and another rise occur it is line
							if ( abs(( CAM_DATA1(i - 1) - CAM_DATA1(i) )) < (maxDiff * LANE_RATIO) )
							{
								if ( laneEndFlag )
								{
									laneEndIndex = dSIndex;
									cam1LaneNum ++;
									if ( cam1LaneNum > 6 ) 
									{
										GPIO_Set( DLED_LED4, 0 );
										return; //경우의 수 밖!!!
									}
									else
									{
										cam1LanePosition[cam1LaneNum - 1] = ( (laneEndIndex - laneStartIndex) / 2.0 );
										
										laneStartIndex = 0;
										laneEndIndex = 0;
										laneEndFlag = 0;
										lanePixelCount = 0;
										dSIndex = 0;
										dEIndex = 0;
									}
								}
								else
								{
									lanePixelCount ++;
								}
							}
							else
							{
								//dSIndex = 0;
								//dEIndex = 0;
							}
						}
					}
				}
				else
				{
					if ( abs( CAM_DATA1(i - 1) - CAM_DATA1(i) ) > (maxDiff * EDGE_RATIO) ) /* successive pixel diff > max diff 1/6*/ 
					{
						/* cur index save, dSucFlag up */
						dSIndex = i;
						dSucFlag = 1;
					
					}
				}
			}
			
			else if ( lanePixelCount < 4 )
			{
				if ( abs( (CAM_DATA1(i - 1) - CAM_DATA1(i) ) < (maxDiff * LANE_RATIO) ) )
				{
					lanePixelCount ++;
				}
				else
				{
					dSIndex = 0;
					dEIndex = 0;
					lanePixelCount = 0;
				}
				
				
			}
			else
			{
				laneEndFlag = 1;
				laneStartIndex = dEIndex;
				
				dEIndex = 0;
				dSIndex = 0;
				lanePixelCount = 0;
			}
		}
		
		

		/* find location of line in pixel in camera 2*/
		for ( i = 1; i < NUM_OF_PIXEL; i++ )
				{
					if ( lanePixelCount == 0 )
					{
						if ( dSucFlag )
						{
							if ( abs( CAM_DATA2(i - 1) - CAM_DATA2(i) ) > (maxDiff) * EDGE_RATIO ) {}
							else
							{
								dSucFlag = 0;
								dEIndex = i;
								
								if ( abs( CAM_DATA2(dSIndex - 1) - CAM_DATA2(dEIndex - 1) ) > (maxDiff * TOTAL_EDGE_RATIO ) )
								{
									
									// if 1/20 of global differ continous for more than 4pixel and another rise occur it is line
									if ( abs(( CAM_DATA2(i - 1) - CAM_DATA2(i) )) < (maxDiff * LANE_RATIO) )
									{
										if ( laneEndFlag )
										{
											laneEndIndex = dSIndex;
											cam2LaneNum ++;
											if ( cam2LaneNum > 6 ) 
											{
												GPIO_Set( DLED_LED4, 0 );
												return; //경우의 수 밖!!!
											}
											else
											{
												/* There is drop and continuous lane part and up again and low pixel diff appear*/
												cam2LanePosition[cam2LaneNum] = ( (laneEndIndex - laneStartIndex) ) / 2.0;
												
												//laneStartIndex = 0;
												//laneEndIndex = 0;
												laneEndFlag = 0;
												lanePixelCount = 0;
												//dSIndex = 0;
												//dEIndex = 0;
											}
										}
										else
										{
											lanePixelCount ++;
										}
									}
									else
									{
										//dSIndex = 0;
										//dEIndex = 0;
									}
								}
							}
						}
						else
						{
							if ( abs( CAM_DATA2(i - 1) - CAM_DATA2(i) ) > (maxDiff * EDGE_RATIO) ) /* successive pixel diff > max diff 1/6*/ 
							{
								/* current index save, dSucFlag up */
								dSIndex = i;
								dSucFlag = 1;
							
							}
						}
					}
					
					else if ( lanePixelCount < 4 )
					{
						if ( abs( (CAM_DATA2(i - 1) - CAM_DATA2(i) ) < (maxDiff * LANE_RATIO) ) )
						{
							lanePixelCount ++;
						}
						else
						{
							//dSIndex = 0;
							//dEIndex = 0;
							lanePixelCount = 0;
						}
						
						
					}
					else
					{
						laneEndFlag = 1;
						laneStartIndex = dEIndex;
						
						//dEIndex = 0;
						//dSIndex = 0;
						lanePixelCount = 0;
					}
				}
		
		laneStatusUpdate();

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
		GPIO_Set( DLED_LED2, 0);
	}
	else if ( cam1LaneNum + cam2LaneNum  < 2 ) GPIO_Set( DLED_LED4, 0 ); //경우의 수 밖!!!
	else if ( cam1LaneNum == 1 && cam2LaneNum == 1 ) GPIO_Set( DLED_LED1, 0 );//정상적 lane
	else if ( cam1LaneNum == cam2LaneNum )  schoolZoneFlag = 1;
	else GPIO_Set( DLED_LED4, 0 ); //경우의 수 밖!!!
	
}

double cam1LanePositionReturn ( void )
{
	if ( cam1LaneNum == 1) return cam1LanePosition[0];
	else if ( cam2LaneNum == 0 ) return -1;
}

double cam2LanePositionReturn ( void )
{
	if ( cam2LaneNum == 1) return cam2LanePosition[0];
	else if ( cam2LaneNum == 0 ) return -1;
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
