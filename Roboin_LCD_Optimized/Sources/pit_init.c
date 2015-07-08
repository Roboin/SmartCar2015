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
 * file                   : pit_init.c
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
#include "ngv_dmu.h"
#include "basetimer.h"
#include "lcd.h"



/********************* Initialization Function(s) *********************** */


void pit_init_fnc(void)
{
    PIT.PITMCR.B.MDIS   = 1;    
        /*Disable PIT for initialization         */

/* ----------------------------------------------------------- */
/*                     Configure Load Value Registers                */
/* ----------------------------------------------------------- */
#if NGV_DMU_SYSCLK == SYSCLK_IRC_16MHZ
    PIT.CH[0].LDVAL.R  = 16*PIT0_TIMER_GAP;//400;//160;//16000;    
        /*value loaded in the Timer0: 16000    */
#elif NGV_DMU_SYSCLK == SYSCLK_PLL_32MHZ
    PIT.CH[0].LDVAL.R  = 32*PIT0_TIMER_GAP;//800;//320;//32000;    
        /*value loaded in the Timer0: 32000    */
#elif NGV_DMU_SYSCLK == SYSCLK_PLL_64MHZ
    PIT.CH[0].LDVAL.R  = 64*PIT0_TIMER_GAP;//1600;//640;//0x0000FA00;    
        /*value loaded in the Timer0: 64000    */
#endif
    
#if NGV_DMU_SYSCLK == SYSCLK_IRC_16MHZ
    PIT.CH[1].LDVAL.R  = 16000;    
        /*value loaded in the Timer1: 1600    */
#elif NGV_DMU_SYSCLK == SYSCLK_PLL_32MHZ
    PIT.CH[1].LDVAL.R  = 32000;    
        /*value loaded in the Timer1: 3200    */
#elif NGV_DMU_SYSCLK == SYSCLK_PLL_64MHZ
    PIT.CH[1].LDVAL.R  = 64000;//0x00001900;    
        /*value loaded in the Timer1: 6400    */
#endif
    
#if NGV_DMU_SYSCLK == SYSCLK_IRC_16MHZ
    PIT.CH[2].LDVAL.R  = 16000;    
        /*value loaded in the Timer0: 16000    */
#elif NGV_DMU_SYSCLK == SYSCLK_PLL_32MHZ
    PIT.CH[2].LDVAL.R  = 32000;    
        /*value loaded in the Timer0: 32000    */
#elif NGV_DMU_SYSCLK == SYSCLK_PLL_64MHZ
    PIT.CH[2].LDVAL.R  = 0x0000FA00;    
        /*value loaded in the Timer0: 64000    */
#endif
    
#if NGV_DMU_SYSCLK == SYSCLK_IRC_16MHZ
    PIT.CH[3].LDVAL.R  = 640000;    
        /*value loaded in the Timer3: 640000    */
#elif NGV_DMU_SYSCLK == SYSCLK_PLL_32MHZ
    PIT.CH[3].LDVAL.R  = 0x00271000;//1280000;    
        /*value loaded in the Timer3: 1280000    */
#elif NGV_DMU_SYSCLK == SYSCLK_PLL_64MHZ
    PIT.CH[3].LDVAL.R  = 0x00271000;    
        /*value loaded in the Timer3: 2560000    */
#endif

#if NGV_DMU_SYSCLK == SYSCLK_IRC_16MHZ
    PIT.CH[4].LDVAL.R  = 640000;    
        /*value loaded in the Timer3: 640000    */
#elif NGV_DMU_SYSCLK == SYSCLK_PLL_32MHZ
    PIT.CH[4].LDVAL.R  = 0x00271000;//1280000;    
        /*value loaded in the Timer3: 1280000    */
#elif NGV_DMU_SYSCLK == SYSCLK_PLL_64MHZ
    PIT.CH[4].LDVAL.R  = 0x00271000;    
        /*value loaded in the Timer3: 2560000    */
#endif
    //PIT.CH[4].LDVAL.R  = 0x00000000;    
        /*value loaded in the Timer4: 0    */
    
    PIT.CH[5].LDVAL.R  = 0x00000000;    
        /*value loaded in the Timer5: 0    */
    
    PIT.CH[6].LDVAL.R  = 0x00000000;    
        /*value loaded in the Timer6: 0    */
    
    PIT.CH[7].LDVAL.R  = 0x00000000;    
        /*value loaded in the Timer7: 0    */
    


/* ----------------------------------------------------------- */
/*                     Enable Interrupts                  */
/* ----------------------------------------------------------- */

    PIT.CH[0].TCTRL.B.TIE  = 0x1;    
        /*Timer 0 Interrupt : Enabled     */
    
    PIT.CH[1].TCTRL.B.TIE  = 0x0;    
        /*Timer 1 Interrupt : Disabled    */
    
    PIT.CH[2].TCTRL.B.TIE  = 0x0;    
        /*Timer 2 Interrupt : Disabled    */
    
    PIT.CH[3].TCTRL.B.TIE  = 0x1;    
        /*Timer 3 Interrupt : Enabled     */
    
    PIT.CH[4].TCTRL.B.TIE  = 0x0;    
        /*Timer 4 Interrupt : EN//Disabled    */
    
    PIT.CH[5].TCTRL.B.TIE  = 0x0;    
        /*Timer 5 Interrupt : Disabled    */
    
    PIT.CH[6].TCTRL.B.TIE  = 0x0;    
        /*Timer 6 Interrupt : Disabled    */
    
    PIT.CH[7].TCTRL.B.TIE  = 0x0;    
        /*Timer 7 Interrupt : Disabled    */
    


/* ----------------------------------------------------------- */
/*                   Start Timers                 */
/* ----------------------------------------------------------- */

    PIT.CH[0].TCTRL.B.TEN = 0x1;    
        /*Start Timer 0 is : Enabled     */
    
    PIT.CH[1].TCTRL.B.TEN = 0x1;    
        /*Start Timer 1 is : Enabled     */
    
    PIT.CH[2].TCTRL.B.TEN = 0x1;    
        /*Start Timer 2 is : Enabled     */
    
    PIT.CH[3].TCTRL.B.TEN = 0x1;    
        /*Start Timer 3 is : Enabled     */
    
    PIT.CH[4].TCTRL.B.TEN = 0x0;    
        /*Start Timer 4 is : EN//Disabled    */
    
    PIT.CH[5].TCTRL.B.TEN = 0x0;    
        /*Start Timer 5 is : Disabled    */
    
    PIT.CH[6].TCTRL.B.TEN = 0x0;    
        /*Start Timer 6 is : Disabled    */
    
    PIT.CH[7].TCTRL.B.TEN = 0x0;    
        /*Start Timer 7 is : Disabled    */
    

    PIT.PITMCR.B.MDIS = 0;    
        /*PIT Module : Enabled           */

}

 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

