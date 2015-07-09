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
 * file                   : adc_init.h
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
 * Brief Description      : This file contains function declaration for ADC code File.
 *
 *
 *######################################################################
*/

#ifndef  _ADC_INIT_H
#define  _ADC_INIT_H
/********************  Dependent Include files here **********************/

#include "jdp.h"

/**********************  Function Prototype here *************************/

void adc_init_fnc(void);
void adc_0_init_fnc(void);
void adc_1_init_fnc(void);
void adc_0_channel_setup_fnc(void);
void adc_1_channel_setup_fnc(void);
void adc_ctuevent_init_fnc(void);
void adc_0_trigger_setup_fnc(void);
void adc_1_trigger_setup_fnc(void);


#endif  /*_ADC_INIT_H*/

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

