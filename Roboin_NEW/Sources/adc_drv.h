#ifndef _ADC_DRV_H
#define _ADC_DRV_H /* Only include header file once */

#include "typedefs.h"
#include "jdp.h"

uint16_t A2D_GetSingleCh_12bit(uint32_t);
uint16_t A2D_GetSingleCh_10bit(uint32_t);
uint16_t A2D_GetChResult_12bit(uint32_t);
uint16_t A2D_GetChResult_10bit(uint32_t);
void A2D_SetupCh_12bit(uint32_t);
void A2D_SetupCh_10bit(uint32_t);

uint16_t ADC_Get(uint32_t pinName);//return ADC Value withe PinName

#define POT_1	GPIO_PB4
 
#endif /* End of Header File Define */

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/



