/*
 * encodr.h
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include "jdp.h"
#include "gpio.h"

#define ENC1_A		GPIO_PA12
#define ENC1_B		GPIO_PA13
#define ENC2_B		GPIO_PA14
#define ENC2_A		GPIO_PA15

#define PulsePerCM 11;
#define PUlsePerRev 1024
//- FUNCTION ----------------------------------------------
//void ENC_IRQ16_23_ISR(void);//PA12,13//11,10//15,14  PA11 : EIRQ16, 
void ENC1_EIRQ16_23_ISR(void);//PA12 : EIRQ17
void ENC2_EIRQ0_7_ISR(void);//PA14: EIRQ4
void ENC_Init(void);

int32_t ENC_Get1(void);
int32_t ENC_Get2(void);


#endif /* ROTARYENCODER_H_ */
