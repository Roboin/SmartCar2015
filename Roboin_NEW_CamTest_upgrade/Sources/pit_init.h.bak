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
 * file                   : pit_init.h
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
 * Brief Description      : File contains declaration for Periodic Interrupt Timer(PIT)
 *                          initialization function.
 *
 *
 *######################################################################
*/

#ifndef  _PIT_INIT_H
#define  _PIT_INIT_H
/********************  Dependent Include files here **********************/

#include "jdp.h"

/**********************  Function Prototype here *************************/

void pit_init_fnc (void);

#define US 64
#define MS 64000
#define pit0cycle 	MS//for timer
#define pit1cycle	200*MS 
#define pit2cycle 	5*MS
#define pit3cycle 	20*MS// for Ultra Sonic
#define pit4cycle 	20*US// for camera
#define pit5cycle 	40*MS//for Motor(Encoder Speed Check) - ENC_SPEED_PIT5_ISR
/*uint32_t pit0cycle = 20*US;//for timer
uint32_t pit1cycle = 200*MS; 
uint32_t pit2cycle = 1*MS;
uint32_t pit3cycle = 20*MS;// for Ultra Sonic
uint32_t pit4cycle = 20*US;// for camera
uint32_t pit5cycle = 40*MS;//for Motor(Encoder Speed Check) - ENC_SPEED_PIT5_ISR*/

#endif  /*_PIT_INIT_H*/

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

