/*
 * isr_init.h
 *
 *  Created on: Jul 10, 2015
 *      Author: JaeHakJeong
 */

#ifndef ISR_INIT_H_
#define ISR_INIT_H_

#include "jdp.h"

//##### intc_adc
void ADC0_EOC_ISR(void);

//##### intc_emios
//void US_EMIOS1_F10_11_ISR(void);

//##### intc_pit
#include "timer.h" //void TIMER_PIT0_ISR(void);
#include "lcd.h"//void LCD_PIT1_ISR(void);
#include "ultra_sonic.h"//void US_PIT3_ISR(void);
#include "camera.h"//void CAM_PIT4_ISR(void);
//void MOTOR_PIT5_ISR(void);
uint8_t flag_PIT(uint8_t ch);

//##### intc_siu
#include "encoder.h"
//void ENC1_EIRQ16_23_ISR(void);
void EIRQ8_15_ISR (void);
//void ENC2_EIRQ0_7_ISR(void);

//##### intc_linflex
void SDBG_RX0_ISR(void);
void SDBG_TX0_ISR(void);
void MUART_RX1_ISR(void);
void MUART_TX1_ISR(void);


#endif /* ISR_INIT_H_ */
