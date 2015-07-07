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
 * file                   : edma_init.c
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
 * Brief Description      : This file contains eDMA and TCD initialization.
 *
 ******************************************************************************** 
 *
 * Detail Description     : File contains eDMA and TCD initialization. File
 *                         contains function for configuration of eDMA and
 *                         its interrupts and priority of channels .It also
 *                         configures all channels of TCD and eDMA MUX channel.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "rappid_ref.h"
#include "edma_init.h"
#include "dspi_var.h"
#include "linflex_var.h"







/*********************  Initialization Function(s) ************************/

void edma_init_fnc(void)
{
/* -------------------------------------------------------------- */
/*             EDMA Configuration Register (EDMA_CR)              */
/* -------------------------------------------------------------- */
	EDMA.CR.R   = 0x00000000;
    /* Buffered writes is : Disabled    */
    /* Enable Debug is : No    */
    /* Channel Arbitration scheme: Fixed Priority    */
    /* Halt on error: Disabled    */
    /* Halt DMA Operations: Disabled    */
    /* Continuous Link Mode: Disabled    */
    /* Enable Minor Loop Mapping: Disabled    */

/* -------------------------------------------------------------- */
/* EDMA Enable Error Interrupt Request Registers (EDMA_EEIRL)      */
/* -------------------------------------------------------------- */
	EDMA.EEIRL.R   = 0x0000;
    /* Error Interrupt disabled for all channels  */

/* -------------------------------------------------------------- */
/* EDMA Channel Priority Register Settings (EDMA_CPRn) Cha 0 - 15 */
/* -------------------------------------------------------------- */
                             /*  Channel,   Priority,    DPA,     Pre-emption */
	EDMA.CPR[0].R    =0x00;    /*     0,          0,     Disabled,     Disabled    */

	EDMA.CPR[1].R    =0x01;    /*     1,          1,     Disabled,     Disabled    */

	EDMA.CPR[2].R    =0x02;    /*     2,          2,     Disabled,     Disabled    */

	EDMA.CPR[3].R    =0x03;    /*     3,          3,     Disabled,     Disabled    */

	EDMA.CPR[4].R    =0x04;    /*     4,          4,     Disabled,     Disabled    */

	EDMA.CPR[5].R    =0x05;    /*     5,          5,     Disabled,     Disabled    */

	EDMA.CPR[6].R    =0x06;    /*     6,          6,     Disabled,     Disabled    */

	EDMA.CPR[7].R    =0x07;    /*     7,          7,     Disabled,     Disabled    */

	EDMA.CPR[8].R    =0x08;    /*     8,          8,     Disabled,     Disabled    */

	EDMA.CPR[9].R    =0x09;    /*     9,          9,     Disabled,     Disabled    */

	EDMA.CPR[10].R    =0x0a;    /*    10,         10,     Disabled,     Disabled    */

	EDMA.CPR[11].R    =0x0b;    /*    11,         11,     Disabled,     Disabled    */

	EDMA.CPR[12].R    =0x0c;    /*    12,         12,     Disabled,     Disabled    */

	EDMA.CPR[13].R    =0x0d;    /*    13,         13,     Disabled,     Disabled    */

	EDMA.CPR[14].R    =0x0e;    /*    14,         14,     Disabled,     Disabled    */

	EDMA.CPR[15].R    =0x0f;    /*    15,         15,     Disabled,     Disabled    */

/* -------------------------------------------------------------- */
/*  EDMA Enable Request Registers (EDMA_ERQRL)                    */
/* -------------------------------------------------------------- */
	EDMA.ERQRL.R   = 0x0000;
    /*EDMA Enable Low Request Register disabled for all channels  */

/* -------------------------------------------------------------- */
/* EDMA MUX Channel Configuration Register (DMAMUX.CHCONFIG)Cha 0 - 15   */
/* -------------------------------------------------------------- */
    DMAMUX.CHCONFIG[0].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[1].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[2].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[3].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[4].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[5].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[6].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[7].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[8].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[9].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[10].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[11].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[12].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[13].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[14].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

    DMAMUX.CHCONFIG[15].R   = 0x00; 
	/* DMA Channel Source Name -- DEFAULT (to be routed to the DMA channel.) */
	/* DMA Channel Enable: Disabled */
	/* DMA Channel Trigger Enable: Disabled */

}
 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

