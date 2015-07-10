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
 * file                   : main.c
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
 * Brief Description      : This file contains main() function call.
 *
 ******************************************************************************** 
 *
 * Detail Description     : This file contains main() routine which calls system
 *                         initialization routine and interrupt enable routine if defined.
 *
 ******************************************************************************** 
 *
 *######################################################################
*/
/********************  Dependent Include files here **********************/
#include "rappid_ref.h"
#include "rappid_utils.h"
#include "sys_init.h"
/**********************  Function Prototype here *************************/
void main(void);
void init_peripherals(void);

void DoMainLoop();
/**********************  Flags, Variables, Parameters *************************/
char stringtest[] = "abcd1234";
vuint8_t flag = 1;
vuint32_t i = 0;
vuint32_t t_current = 0, t_old = 0;
vuint32_t time_gap = 500;
/*********************  Initialization Function(s) ************************/
void init_peripherals(void){
	sys_init_fnc();
	LedSetAll(0);
	TIMER_Init();
	UartBufInit();
	LCD_initialize();
	US_Init();
	CAM_Init();
	MOTOR_Init();
}
/*********************  DoMainLoop ************************/
void DoMainLoop(){
	
	LedSet(1,flag);
	LedSet(2,1-flag);
	LedSet(3,flag);
	LedSet(4,1-flag);
	
	time_gap = 100;//(ADC_Get(POT_1);
	t_current = TIMER_GetRuntime()/time_gap;
	if (t_current != t_old) {
		int32_t USval = (int32_t)US_Get();
		itoa((int32_t)USval, stringtest);//US_Get()
		UART_println(stringtest);
		t_old = t_current;
		
		if(flag == 1){
			flag = 0;
		}
		else{
			flag = 1;
		}
	}
}

void main(void)
{
	init_peripherals();
	EnableExternalInterrupts();
   
	while(1)
	{
	   DoMainLoop();
	}
}
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

