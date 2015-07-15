/*
 * isr_init.c
 *
 *  Created on: Jul 10, 2015
 *      Author: JaeHakJeong
 */

#include "isr_init.h"

//##### intc_adc.c
void ADC0_EOC_ISR (void){
    ADC_0.ISR.R=0x0000001F;
}

//##### intc_emios.c
/*void US_EMIOS1_F10_11_ISR (void){
    EMIOS_1.CH[10].CSR.B.FLAG = 1;
    EMIOS_1.CH[11].CSR.B.FLAG = 1;
}*/

//##### intc_pit.c
/*void TIMER_PIT0_ISR (void)
{
    PIT.CH[0].TFLG.R = 0x00000001;

}*/

/*void LCD_PIT1_ISR (void){
	flag_PIT_ARRAY[1] = !flag_PIT_ARRAY[1];
    PIT.CH[1].TFLG.R = 0x00000001;
}*/
void PIT2_ISR(void){
	PIT.CH[2].TFLG.R = 0x00000001;
}
/*void US_PIT3_ISR (void){
	flag_PIT_ARRAY[3] = !flag_PIT_ARRAY[3];
    PIT.CH[3].TFLG.R = 0x00000001;
}*/
/*
void CAM_PIT4_ISR (void){
	flag_PIT_ARRAY[4] = !flag_PIT_ARRAY[4];
    PIT.CH[4].TFLG.R = 0x00000001;
}*/


/*void MOTOR_PIT5_ISR (void){
	flag_PIT_ARRAY[5] = !flag_PIT_ARRAY[5];
    PIT.CH[5].TFLG.R = 0x00000001;
}*/

//##### intc_siu.c
#include "encoder.h"
/*void ENC2_EIRQ0_7_ISR (void){
    SIU.ISR.R = 0x000000FF;
}

void ENC1_EIRQ16_23_ISR (void){
    SIU.ISR.R = 0x00FF0000;
}*/

void EIRQ8_15_ISR (void){
    SIU.ISR.R = 0x0000FF00;
}