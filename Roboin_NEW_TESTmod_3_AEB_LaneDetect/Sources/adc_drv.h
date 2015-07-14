#ifndef _ADC_DRV_H
#define _ADC_DRV_H /* Only include header file once */

#include "typedefs.h"
#include "jdp.h"
#include "gpio.h"

uint16_t A2D_GetSingleCh_12bit(uint32_t);
uint16_t A2D_GetSingleCh_10bit(uint32_t);
uint16_t A2D_GetChResult_12bit(uint32_t);
uint16_t A2D_GetChResult_10bit(uint32_t);
void A2D_SetupCh_12bit(uint32_t);
void A2D_SetupCh_10bit(uint32_t);

uint16_t ADC_Get(uint32_t pinName);//return ADC Value withe PinName

/*adc ch 
 * 0 pot 1 pb4
 * 1 pb5
 * 4 pd0
 * 5 pd1
 * 6 pd2
 */

#define POT_1	GPIO_PB4
#define IR_PIN	GPIO_PD2
 
#endif /* End of Header File Define */

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/



