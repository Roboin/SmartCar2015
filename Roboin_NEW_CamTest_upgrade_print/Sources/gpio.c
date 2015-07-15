#include "gpio.h"

uint8_t GPIO_Get(uint16_t ch)//Get GPIO Status
{
    uint8_t result;

    result = (uint8_t)SIU.GPDI[ch].B.PDI;

    return(result);
}

void GPIO_Set (uint16_t ch, int value)//Set GPIO High Or Low
{
    SIU.GPDO[ch].B.PDO = value;
}

 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/
