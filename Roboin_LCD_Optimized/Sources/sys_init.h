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
 * file                   : sys_init.h
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
 * Brief Description      : This file contains system initialization code
 *
 *
 *######################################################################
*/

#ifndef  _SYS_INIT_H
#define  _SYS_INIT_H
/********************  Dependent Include files here **********************/

#include "ngv_dmu.h"
#include "intc_init.h"
#include "sysclk_init.h"
#include "linflex_init.h"
#include "adc_init.h"
#include "msr_init.h"
#include "swt_init.h"
#include "edma_init.h"
#include "i2c_init.h"
#include "flexcan_init.h"
#include "cansp_init.h"
#include "emios_init.h"
#include "dspi_init.h"
#include "siu_init.h"
#include "mpu_init.h"
#include "rgm_init.h"
#include "pit_init.h"
#include "stm_init.h"
#include "rtc_init.h"
#include "flash_init.h"

/**********************  Function Prototype here *************************/

void sys_init_fnc (void);
#if NGV_DMU_PROJECT == 0
extern void IVPRInitialize(void);
#endif


#endif  /*_SYS_INIT_H*/

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

