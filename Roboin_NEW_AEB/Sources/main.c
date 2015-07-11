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
#include "stdlib.h"
#include "stdio.h"
#include "timer.h"
/**********************  Function Prototype here *************************/
void main(void);
void init_peripherals(void);

void DoMainLoop(void);

void Speed_Control(int16_t proposal_speed);
void AEB(void);

/**********************  Variables, Parameters *************************/
char string_temp[256] = " ";//"abcd1234";

vuint32_t i = 0;

vuint32_t t_current = 0, t_old = 0;
vuint32_t time_gap = 500;

//int32_t enc1_current=0, enc1_old=0, enc2_current=0, enc2_old=0;
//int32_t enc1_rate=0, enc2_rate=0;

int32_t USval = 1500;

int16_t DC_Power_R = 500;
int16_t DC_Power_L = 500;
int16_t Proposal_Speed = 5;

/*##### Flags ##########################*/
uint8_t flag = 1;
uint8_t flag_US = 0;
uint8_t flag_IR = 0;
uint8_t flag_Slope = 0;
uint8_t flag_Wall = 0;
/*********************  Initialization Function(s) ************************/
void init_peripherals(void){
	sys_init_fnc();
	Led_Set_All(0);
	TIMER_Init();
	UartBufInit();
	LCD_initialize();
	US_Init();
	CAM_Init();
	MOTOR_Init();
}
/*********************  DoMainLoop ************************/
void DoMainLoop(){

	// Is it working?? LED	
	Led_Set(1,flag);
	
	/*##### UART Print, #############*/
	time_gap = 250;//(ADC_Get(POT_1);
	t_current = TIMER_GetRuntime()/time_gap;
	if (t_current != t_old) {
		t_old = t_current;
		
		/*##################  AEB  #####################*/
		USval = (int32_t)US_Get();
		if(USval < 500){// Near 50cm
			Proposal_Speed = 0;
			flag_US = 1;
			AEB();
		}
		else{
			flag_IR = 0;
			flag_US = 0;
			flag_Wall = 0;
			flag_Slope = 0;
			Proposal_Speed = 5;
		}
		Led_Set(LED_3,flag_Slope);
		Led_Set(LED_4,flag_Wall);
		
		//Uart print
		UART_print("US : ");
		itoa((int32_t)USval, string_temp);//US_Get()//sprintf(string_temp, "%d", ENC_Rate1_Return());
		UART_print(string_temp);
		UART_print(",  IR : ");
		itoa((int32_t)ADC_Get(IR_PIN), string_temp);//US_Get()
		UART_print(string_temp);
		
		
		/*ENCODER TEST*/
		/*enc1_current = ENC_Get1();
		enc2_current = ENC_Get2();
		enc1_rate = enc1_current - enc1_old;
		enc2_rate = enc2_current - enc2_old;
		enc1_old = enc1_current;
		enc2_old = enc2_current;*///PIT5 안쓰고 속도 구할경우 - main loop가 빠르면 이게 나을지도?
		UART_print("  ENC1 : ");//string_temp);
		itoa((int32_t)ENC_Rate1_Return(), string_temp);
		UART_print(string_temp);
		UART_print("  ENC2 : ");//string_temp);
		itoa((int32_t)ENC_Rate2_Return(), string_temp);
		UART_print(string_temp);
		
		//Speed Return test
		UART_print("  SPD1 : ");//string_temp);
		itoa((int32_t)MOTOR_Current_Speed_R(), string_temp);
		UART_print(string_temp);
		UART_print("  SPD2 : ");//string_temp);
		itoa((int32_t)MOTOR_Current_Speed_L(), string_temp);
		UART_print(string_temp);
		
		
		UART_println(" ");//Newline
		//For led blinking
		flag = !flag;
	}
}

void main(void)
{
	init_peripherals();
	EnableExternalInterrupts();
	UART_println("##  Start  ##");
	while(1)
	{
	   DoMainLoop();
	}
}

/* ################ Other Functions Declaration #############*/
void Speed_Control(int16_t proposal_speed){
	int16_t kp = 10;
	int32_t Speed_R = MOTOR_Current_Speed_R();
	int32_t Speed_L = MOTOR_Current_Speed_L();
	
	if(Speed_R > proposal_speed){
		DC_Power_R -= kp;
	}
	if(Speed_R < proposal_speed){
		DC_Power_R += kp;
	}
	if(Speed_L > proposal_speed){
		DC_Power_L -= kp;
	}
	if(Speed_L < proposal_speed){
		DC_Power_L += kp;
	}
}

/*#############  AEB ##### 그냥 DomainLoop 안에 놓지 말것, ADC 읽어오는 주기가 너무 빨라 멈춤.#################*/
void AEB(){				//0    10   20   30   40   50   60
	uint16_t IR_POINT[] ={500, 500, 500, 420, 330, 250, 210};//ADC value data for 10-50cm
	int16_t ReverseSpeed = -2047;
	int16_t AEB_Distance = 30;
	uint16_t IR_val = ADC_Get(IR_PIN);
	if(flag_US){
		if( IR_val > IR_POINT[4] ){//a2d(6) = PD1, NEED CONFIRM!
			//Stop the car.
			MOTOR_DC_R(ReverseSpeed);
			MOTOR_DC_L(ReverseSpeed);
			TIMER_DelayMs(1);
			MOTOR_BrakeEngine();
			flag_IR = 1;
			flag_Wall = 1;
		}
		else{//경사로, Re Acceleration
			//Speed_Control(25);
			flag_IR = 0;
			flag_Slope = 1;
		}
	}
	//IR detected
	if(flag_IR){
		MOTOR_BrakeEngine();
		GPIO_Set(H_EN,0);
	}
	
}
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

