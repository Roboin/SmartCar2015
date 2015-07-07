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
 * file                   : i2c_init.c
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
 * Brief Description      : File contains I2C initialization. 
 *
 ******************************************************************************** 
 *
 * Detail Description     : File contains function which configures I2C Master/Slave
 *                          mode,Transmit/Receive mode, baud rate, slave address and
 *                          interrupt.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/

 
 
/********************  Dependent Include files here **********************/

#include "i2c_init.h"


/********************* I2C_0 Initialization Function(s) *********************** */

void i2c_0_init_fnc(void)
{

    I2C_0.IBCR.B.MDIS = 1;
        /* Module Disable : Disabled */

/*-----------------------------------------------------------*/
/*    I2C_0 Frequency Divider Register (I2C_0_IBFD)            */
/*-----------------------------------------------------------*/
    I2C_0.IBFD.R = 0x00;
        /* I2C Baudrate : 0.800000 MHz */
        /* Bus Clock Multiplier : 1*/

/*-----------------------------------------------------------*/
/*    I2C_0 Slave Address (I2C_0_IBAD)                         */
/*-----------------------------------------------------------*/
    I2C_0.IBAD.R = 0x00;
            /* Slave Address  : 0*/

/*-----------------------------------------------------------*/
/*    I2C_0 Control Register (I2C_0_IBCR)                      */
/*-----------------------------------------------------------*/
    I2C_0.IBCR.R = 0x80;
            /* Module Disable  : Disabled*/
            /* I-Bus Interrupt Enabled  : Disabled*/
            /* Master/Slave Mode  : Slave*/
            /* Transmit/Receive Mode  : Receieve*/
            /* Data acknowledge disable  :  Acknowledge*/
            /* Repeat Start  : Disabled*/
            /* DMA Enable  :  Disabled */

/*-----------------------------------------------------------*/
/*    I2C_0 Bus Interrupt Config Register (I2C_0_IBIC)         */
/*-----------------------------------------------------------*/
    
    I2C_0.IBIC.R = 0x00;
        /* Bus idle interrupt : Disabled*/
}
 
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

