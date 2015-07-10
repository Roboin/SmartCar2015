/*
 *######################################################################
 *                                RAppIDJDP
 *           Rapid Application Initialization and Documentation Tool
 *                         Freescale Semiconductor Inc.
 *
 *######################################################################
 *
 * Project Name           : Roboin
 *
 * Project File           : Roboin.rsp
 *
 * Revision Number        : 1.0
 *
 * Tool Version           : 1.2.1.5
 *
 * file                   : pit_init.c
 *
 * Target Compiler        : Codewarrior
 *
 * Target Part            : MPC5606B
 *
 * Part Errata Fixes      : none
 *
 * Project Last Save Date : 10-Jul-2015 00:03:01
 *
 * Created on Date        : 10-Jul-2015 00:03:01
 *
 * Brief Description      : PIT Initialization file
 *
 ******************************************************************************** 
 *
 * Detail Description     : File contains Periodic Interrupt Timer (PIT) parameter
 *                         initialization function. Along with PIT module, the
 *                         function configures parameters such as individual
 *                         channel status, interrupt status & timer load value.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "pit_init.h"


/********************* Initialization Function(s) *********************** */
#define US 64
#define MS 64000

vuint32_t pit0cycle = 20*US;
vuint32_t pit1cycle = 200*MS;
vuint32_t pit2cycle = 1*MS;
vuint32_t pit3cycle = 20*MS;
vuint32_t pit4cycle = 500*MS;
vuint32_t pit5cycle = 1000*MS;

void pit_init_fnc(void)
{
    PIT.PITMCR.B.MDIS   = 1;    
        /*Disable PIT for initialization         */

/* ----------------------------------------------------------- */
/*                     Configure Load Value Registers                */
/* ----------------------------------------------------------- */

    PIT.CH[0].LDVAL.R  = pit0cycle;//0x00000500;    
        /*value loaded in the Timer0: 1280    */
    
    PIT.CH[1].LDVAL.R  = pit1cycle;//0x0000FA00;    
        /*value loaded in the Timer1: 64000    */
    
    PIT.CH[2].LDVAL.R  = pit2cycle;//0x0000FA00;    
        /*value loaded in the Timer2: 64000    */
    
    PIT.CH[3].LDVAL.R  = pit3cycle;//0x00271000;    
        /*value loaded in the Timer3: 2560000    */
    
    PIT.CH[4].LDVAL.R  = pit4cycle;//0x0030D400;    
        /*value loaded in the Timer4: 3200000    */
    
    PIT.CH[5].LDVAL.R  = pit5cycle;//0x00000000;    
        /*value loaded in the Timer5: 0    */
    
    PIT.CH[6].LDVAL.R  = 0x00000000;    
        /*value loaded in the Timer6: 0    */
    
    PIT.CH[7].LDVAL.R  = 0x00000000;    
        /*value loaded in the Timer7: 0    */
    


/* ----------------------------------------------------------- */
/*                     Enable Interrupts                  */
/* ----------------------------------------------------------- */

    PIT.CH[0].TCTRL.B.TIE  = 0x1;    
        /*Timer 0 Interrupt : Enabled    */
    
    PIT.CH[1].TCTRL.B.TIE  = 0x1;//0    
        /*Timer 1 Interrupt : Disabled    */
    
    PIT.CH[2].TCTRL.B.TIE  = 0x1;//0    
        /*Timer 2 Interrupt : Disabled    */
    
    PIT.CH[3].TCTRL.B.TIE  = 0x1;    
        /*Timer 3 Interrupt : Enabled    */
    
    PIT.CH[4].TCTRL.B.TIE  = 0x1;//0    
        /*Timer 4 Interrupt : Disabled    */
    
    PIT.CH[5].TCTRL.B.TIE  = 0x1;//0    
        /*Timer 5 Interrupt : Disabled    */
    
    PIT.CH[6].TCTRL.B.TIE  = 0x0;    
        /*Timer 6 Interrupt : Disabled    */
    
    PIT.CH[7].TCTRL.B.TIE  = 0x0;    
        /*Timer 7 Interrupt : Disabled    */
    


/* ----------------------------------------------------------- */
/*                   Start Timers                 */
/* ----------------------------------------------------------- */

    PIT.CH[0].TCTRL.B.TEN = 0x1;    
        /*Start Timer 0 is : Enabled    */
    
    PIT.CH[1].TCTRL.B.TEN = 0x1;    
        /*Start Timer 1 is : Enabled    */
    
    PIT.CH[2].TCTRL.B.TEN = 0x1;    
        /*Start Timer 2 is : Enabled    */
    
    PIT.CH[3].TCTRL.B.TEN = 0x1;    
        /*Start Timer 3 is : Enabled    */
    
    PIT.CH[4].TCTRL.B.TEN = 0x1;//0    
        /*Start Timer 4 is : Disabled    */
    
    PIT.CH[5].TCTRL.B.TEN = 0x1;//0    
        /*Start Timer 5 is : Disabled    */
    
    PIT.CH[6].TCTRL.B.TEN = 0x0;    
        /*Start Timer 6 is : Disabled    */
    
    PIT.CH[7].TCTRL.B.TEN = 0x0;    
        /*Start Timer 7 is : Disabled    */
    

    PIT.PITMCR.B.MDIS = 0;    
        /*PIT Module : Enabled        */

}

 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

