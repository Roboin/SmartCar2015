/*
 *######################################################################
 *                                RAppIDJDP
 *           Rapid Application Initialization and Documentation Tool
 *                         Freescale Semiconductor Inc.
 *
 *######################################################################
 *
 * Project Name           : NGV_DMU
 *
 * Project File           : NGV_DMU.rsp
 *
 * Revision Number        : 1.0
 *
 * Tool Version           : 1.2.1.5
 *
 * file                   : cansp_init.c
 *
 * Target Compiler        : Codewarrior
 *
 * Target Part            : MPC5606B
 *
 * Part Errata Fixes      : none
 *
 * Project Last Save Date : 28-Dec-2014 21:53:21
 *
 * Created on Date        : 28-Dec-2014 21:53:27
 *
 * Brief Description      : This file contains initializations for CAN Sampler
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file contains initilization of CAN sampler.
 *                         It configures CAN sampler for baud rate, receive port and mode .
 *                         It also contains CAN sampler trigger function.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "cansp_init.h"



/********************  Initialization Function(s) **********************/

void cansp_init_fnc(void)
{
    uint8_t i;

    CANSP.CR.B.BRP = 0x0;
        /* Baud rate prescaler: 1*/

    CANSP.CR.B.CAN_RX_SEL = 0x0;
        /* RX0 is selected    */

    CANSP.CR.B.MODE = 0x0;
        /* Sample & store the 2nd Frame   */

        /* Sample register initialization */
    for (i = 0; i < 12; i++)
    {
     CANSP.SR[i].R = 0x0;
    }

}

void cansp_trigger_fnc(void)
{

        /* Start CAN Sampler and wait */
    CANSP.CR.B.CAN_SMPLR_EN = 0x0;
        /* CAN Sampler Disabled   */

}
 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

