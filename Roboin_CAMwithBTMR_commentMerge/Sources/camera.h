/*
 * camera.c
 *
 *  Created on: June 23, 2015
 *      Author: 연세대학교 기계공학과 정재학
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "jdp.h"

void CAM_Init(void);//calc delay variables

/* camera Interrupt Service Routine
 * Periodic interrupt timmer channel 0 기준(basetimer.c에) 25us 마다 카메라 run1 or run2  실행시키는 함수
 */
void CAM_ISR(void);//recall every 10 microsec with btmr
vuint32_t CAM_MICRO_SEC_RETURN(void);//for recall cam_micro_sec
void CAM_RESET_TIMER(void);//make cam_micro_sec 0;

void CAM_RUN(void);// running camera using inturrupt
void CAM_Clock(void);//clock pulse
//void CAM_SI(void);//Triger Pulse
void CAM_PulseWidth(int16_t PULSEWIDTH);//change camera clock, trig width

#define NUM_OF_PIXEL 128
uint16_t CAM_MAX(void);//MAX Readed value
uint16_t CAM_MIN(void);//Min Readed value 
uint16_t CAM_AVG(void);
uint16_t CAM_TRESHLOLD(int16_t pixelnum);//Calc mid data of max and min

void CAM_DELAY(int32_t delay_long);//delay using While
void CAM_RUN2(void);//Non-Inturrupte
void CAM_UPDATE_DATA(void);//CAM_TEMP to CAM_READ

/*Processed CAM_READ of camera 1
 * parameter: index of pixel
 * return: value of brightness of that pixel */
uint16_t CAM_DATA1(int16_t pixelnum);

/*Processed CAM_READ of camera 2
 * parameter: index of pixel
 * return: value of brightness of that pixel */
uint16_t CAM_DATA2(int16_t pixelnum);

void CAM_RUN_MODE_SETUP(int16_t mode);
void CAM_SI_ON(void);

#endif /* CAMERA_H_ */
