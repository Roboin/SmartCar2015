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
 * file                   : i2c_util.c
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
 * Brief Description      : This File contains I2C start function.
 *
 ******************************************************************************** 
 *
 * Detail Description     : This function generates I2C start condition by making
 *                          Master Transmit mode. It also configures data register
 *                          with calling address.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "i2c_init.h"
#include "rappid_ref.h"
#include "i2c_util.h"



void i2c_0_start_fnc(void)
{
    while (I2C_0.IBSR.B.IBB == 1 ) {;} //wait in loop for IBB flag to clear
    I2C_0.IBCR.R = 0x38;//Master,Transmit, Noack selected
    //Generate start condition
    I2C_0.IBDR.R = I2C_0.IBAD.R;// send the calling address to the data register
}
 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

