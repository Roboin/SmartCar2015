/*
 * camera.c
 *
 *  Created on: June 23, 2015
 *      Author: 연세대학교 기계공학과 정재학
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "jdp.h"
#include "gpio.h"

#define CAM_SI_PIN 		GPIO_PF0
#define CAM_CLK_PIN 	GPIO_PF1
#define CAM_DATA_PIN1	GPIO_PD0
#define CAM_DATA_PIN2	GPIO_PD1


void CAM_Init(void);//calc delay variables

void CAM_PIT4_ISR(void);//recall every 20 microsec with PIT4, 
vuint32_t CAM_MICRO_SEC_RETURN(void);//for recall cam_micro_sec
void CAM_RESET_TIMER(void);//make cam_micro_sec 0;

void CAM_RUN(void);// running camera using inturrupt
void CAM_Clock(void);//clock pulse
//void CAM_SI(void);//Triger Pulse
void CAM_PulseWidth(int16_t PULSEWIDTH);//change camera clock, trig width

#define NUM_OF_PIXEL 128
//int16_t CAM_MAX(void);//MAX Readed value
//int16_t CAM_MIN(void);//Min Readed value 
//uint16_t CAM_AVG(void);//Calc mid data of max and min
//uint16_t CAM_TRESHLOLD(void);//(int16_t pixelnum);

void CAM_DELAY(int32_t delay_long);//delay using While
void CAM_RUN2(void);//Non-Inturrupte
void CAM_UPDATE_DATA(void);//CAM_TEMP to CAM_READ

uint16_t CAM_DATA1(int16_t pixelnum);//Processed CAM_READ
uint16_t CAM_DATA2(int16_t pixelnum);//Processed CAM_READ

void CAM_RUN_MODE_SETUP(int16_t mode);
void CAM_SI_ON(void);

/* ########### lane detecting constants ######################*/

#define START_PIXEL	5 			/*start pixel to find max and min luminosity */  
#define END_PIXEL 	122		//118 /* end pixel to find max and min luminosity */  
#define EDGE_RATIO 	8//6			//0.166666 /* 기울기,이거보다 급해야 확인, 얕으면 conti// ratio of difference between two successive pixels luminosity to max difference of 128 pixel, which determine that two points are along edge or not*/
#define TOTAL_EDGE_RATIO 4	/* 최대비, 절반보다 어두움 ratio of difference between pixels luminosity to max difference of 128 pixel, which determine that two points are along edge or not*/
#define LANE_RATIO 	10			//0.05 /* 어두운상태에서 밝기진폭 전체 5% 이하 ratio of difference between two successive pixels luminosity to max difference of 128 pixel, which determine that two points are along lane or not*/
#define CAM_MAX_LANE_NUM 3		/*number of lane valid per one camera*/
#define LANE_WIDTH_PIXEL 3 //LANE_WIDTH in PIXEL
#define THICK_LANE_WIDTH_PIXEL 14 // Thick LANE_WIDTH in PIXEL
#define K_CAMCAM_DISTANCE 15 // proportional constant for gap between camera1 and 2 in pixel
#define CROSS_SCHOOL_RATIO 3 // ratio between gap which determine cross and total view range 
#define DROP_CHECK_PATIENCE 1 // 몇 번이나 drop 판별시 노이즈 참는지
#define DROP_VARIATION_RATIO 2 // drop 시 얼마나 노이즈 견디는지

#define MASK_SIZE 5

/*data out function*/
int16_t cam1LanePositionReturn ( int8_t laneNum );
int16_t cam2LanePositionReturn ( int8_t laneNum );
int8_t cam1LaneNumReturn (void);
int8_t cam2LaneNumReturn ( void );
int8_t ifSchoolZone ( void );
int8_t ifCrossSection (void);
int8_t ifLaneProcessEnd ( void );
int8_t ifError ( void ); // up when some occasion out of our thinking happens

/* line detecting function */
void laneProcess( void );
void laneStatusUpdate( void );

void laneProcess21( void );
void laneProcess22( void );

/*filter*/
int16_t  median( int16_t intArray[] );
void quicksort( int16_t x , int16_t y, int16_t intArray[] );
void swap( int16_t i, int16_t j, int16_t intArray[] );

#endif /* CAMERA_H_ */
