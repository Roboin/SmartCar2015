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
 * file                   : mpu_init.c
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
 * Brief Description      : This File Contains Entry configuration for MPU
 *
 ******************************************************************************** 
 *
 * Detail Description     : This File contains function containing entries
 *                         which defines the regions and master access to the
 *                         regions in supervisor and user mode. It also handles
 *                         enabling of MPU.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "rappid_ref.h"
#include "mpu_init.h"




/*********************  Initialization Function(s) ************************/

void mpu_init_fnc(void)
{

    MPU.CESR.B.VLD = 0;
        /* MPU Entries : Invalid*/

}
 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

