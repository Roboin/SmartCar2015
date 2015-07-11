
#include "adc_drv.h"
#include "gpio.h"

uint16_t ADC_Get(uint32_t pinName){
	uint32_t ch = 0;
	if(pinName == POT_1)
		ch = 0;
	else//if(pinName != POT_1)
		ch = pinName - GPIO_PD0 + 4;
	return A2D_GetSingleCh_10bit( ch );//POT_1 = ADC 0, pb4
}

uint16_t A2D_GetSingleCh_12bit(uint32_t ch)
{
    uint16_t result;

    ADC_1.NCMR0.R = (uint32_t)(0x00000001 << ch);

    ADC_1.MCR.B.NSTART = 1;  /* Trigger ADC Conversion */

    while (ADC_1.CDR[ch].B.VALID != 1)
    {
    }
    result = (uint16_t)((ADC_1.CDR[ch].B.CDATA));

    return(result);
}


uint16_t A2D_GetSingleCh_10bit(uint32_t ch)
{
    uint16_t result;

    ADC_0.NCMR0.R = (uint32_t)(0x00000001 << ch);
    if (ch >= 32)
    {
        ADC_0.NCMR1.R = (uint32_t)(0x00000001 << (ch - 32));
    }
    if (ch >= 64)
    {
        ADC_0.NCMR2.R = (uint32_t)(0x00000001 << (ch - 64));
    }

    ADC_0.MCR.B.NSTART = 1;  /* Trigger ADC Conversion */

    while (ADC_0.CDR[ch].B.VALID != 1)
    {
    }
    result = (uint16_t)((ADC_0.CDR[ch].B.CDATA));

    return(result);
}

uint16_t A2D_GetChResult_12bit(uint32_t ch)
{
    uint16_t result;

    if (ch < 16)    /* Verify the channel count value */
    {
        result = (uint16_t)((ADC_1.CDR[ch].B.CDATA));
    }
    return(result);
}

uint16_t A2D_GetChResult_10bit(uint32_t ch)
{
    uint16_t result;

    if ((ch >= 32) && (ch <= 59))    /* Verify the channel count value */
    {
        result = (uint16_t)((ADC_0.CDR[ch].B.CDATA));
    }

    return(result);
}

void A2D_SetupCh_12bit(uint32_t ch)
{
   if (ch < 16)    /* Verify the channel count value */
   {
      ADC_1.NCMR0.R = (uint32_t)((0x00000001 << ch) | ADC_1.NCMR0.R);
   }
}


void A2D_SetupCh_10bit(uint32_t ch)
{
	if (ch < 96)    /* Verify the channel count value */
	{
		ADC_0.NCMR0.R = (uint32_t)((0x00000001 << ch) | ADC_0.NCMR0.R);
		if (ch >= 32)
		{
		    ADC_0.NCMR1.R = (uint32_t)((0x00000001 << (ch - 32)) | ADC_0.NCMR1.R);
		}
		if (ch >= 64)
		{
		    ADC_0.NCMR2.R = (uint32_t)((0x00000001 << (ch - 64)) | ADC_0.NCMR2.R);
		}
	}
 }
 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/



