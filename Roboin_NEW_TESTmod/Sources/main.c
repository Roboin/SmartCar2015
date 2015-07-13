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
void Button_Select_RunMode(void);

/**********************  Variables, Parameters *************************/
char string_temp[256] = " ";//"abcd1234";

vuint32_t i = 0;
vuint32_t t_current = 0, t_old = 0, cnt=0;
vuint32_t time_gap = 500;

uint32_t USval = 1500;
uint32_t IRval = 1;

int16_t DC_Power_R = 500;
int16_t DC_Power_L = 500;
int16_t Speed_Proposal = 5;
			// 0123456789012345
char strA[] = "Mode:            ";
char strB[] = "TIME:           ";
char LCD_BUFF1[8] = "";
char LCD_BUFF2[8] = "";
char LCD_BUFF3[8] = "";
char LCD_BUFF4[8] = "";

/*##### Flags ##########################*/
uint8_t flag = 1;
uint8_t flag_US = 0;
uint8_t flag_IR = 0;
uint8_t flag_Slope = 0;
uint8_t flag_Wall = 0;
uint8_t run_mode = 0;
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
	int16_t i;
	
	//Select Mode with Tact switch button
	Button_Select_RunMode();
	
	time_gap = 1000;//(ADC_Get(POT_1);
	t_current = TIMER_GetRuntime()/time_gap;
	if (t_current != t_old) {
		/*##################  Timer Count  #####################*/
		t_old = t_current; cnt++;
		//itoa((int32_t)cnt, string_temp);
		//UART_print(string_temp);
		flag = !flag;//For led blinking
		
		UART_print(" Test Mode ");
		itoa((int32_t)run_mode, string_temp);
		UART_print(string_temp);UART_print(" : ");
		
		// Is it working?? LED	############################
		if(run_mode == 0){
			Led_Set(1,flag);
			itoa((int32_t)cnt, string_temp);
			UART_print(string_temp);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, strA);
			itoa((int32_t)run_mode,LCD_BUFF1);
			LCD_string(5, 0, LCD_BUFF1);
			LCD_string(7, 0, "Start           ");
			LCD_string(0, 1, "TIME:           ");
			itoa((int32_t)cnt,LCD_BUFF2);
			LCD_string(6, 1, LCD_BUFF2);
		}
		
		/*##################  Lane Detecting test #####################*/
		else if(run_mode == 1){
			Led_Set(1,flag);
			//UART_println(" CAM DATA");
			
			laneProcess();
			/*UART_print("  LANE1 : ");//string_temp);
			itoa((int32_t)cam1LanePositionReturn(0), string_temp);
			UART_print(string_temp);
			UART_print(",  LANE2 : ");//string_temp);
			itoa((int32_t)cam2LanePositionReturn(0), string_temp);
			UART_println(string_temp);
			*/
			UART_println("");
			UART_println("cam1 lane");
			itoa((int32_t)cam1LanePositionReturn(0),LCD_BUFF2);
			UART_print(LCD_BUFF2); UART_print(" ");
			itoa((int32_t)cam1LanePositionReturn(1),LCD_BUFF2);
			UART_print(LCD_BUFF2); UART_print(" ");
			itoa((int32_t)cam1LanePositionReturn(2),LCD_BUFF2);
			UART_print(LCD_BUFF2); UART_print(" ");
			UART_println("");
			
			UART_println("");
			UART_println("cam2 lane");
			itoa((int32_t)cam2LanePositionReturn(0),LCD_BUFF2);
			UART_print(LCD_BUFF2); UART_print(" ");
			itoa((int32_t)cam2LanePositionReturn(1),LCD_BUFF2);
			UART_print(LCD_BUFF2); UART_print(" ");
			itoa((int32_t)cam2LanePositionReturn(2),LCD_BUFF2);
			UART_print(LCD_BUFF2); UART_print(" ");
			UART_println("");
			
			UART_println("");
			UART_println("------------------------------------------------------");

			LCD_ON();//x, y,*string
			//LCD_string(0, 0, strA);
			//itoa((int32_t)run_mode,LCD_BUFF1);
			//LCD_string(5, 0, LCD_BUFF1);
			//LCD_string(7, 0, "CAM Lane        ");
			LCD_string(0, 0, "1:              ");
			itoa((int32_t)cam1LanePositionReturn(0),LCD_BUFF2);
			LCD_string(2, 0, LCD_BUFF2);
			itoa((int32_t)cam1LanePositionReturn(1),LCD_BUFF2);
			LCD_string(6, 0, LCD_BUFF2);
			itoa((int32_t)cam1LanePositionReturn(2),LCD_BUFF2);
			LCD_string(10, 0, LCD_BUFF2);
			
			LCD_string(0, 1, "2:              ");
			itoa((int32_t)cam2LanePositionReturn(0),LCD_BUFF3);
			LCD_string(2, 1, LCD_BUFF3);
			itoa((int32_t)cam2LanePositionReturn(1),LCD_BUFF3);
			LCD_string(6, 1, LCD_BUFF3);
			itoa((int32_t)cam2LanePositionReturn(2),LCD_BUFF4);
			LCD_string(10, 1, LCD_BUFF4);
							
		}
			
		else if(run_mode == 2){	
			Led_Set(2,flag);
			/*##################  AEB  #####################*/
			USval = US_Get();
			if(USval < 500){// Near 50cm
				Speed_Proposal = 0;
				flag_US = 1;
				AEB();
			}
			else{
				flag_IR = 0;
				flag_US = 0;
				flag_Wall = 0;
				flag_Slope = 0;
				Speed_Proposal = 5;
			}
			Led_Set(LED_3,flag_Slope);
			Led_Set(LED_4,flag_Wall);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, strA);
			itoa((int32_t)run_mode,LCD_BUFF1);
			LCD_string(5, 0, LCD_BUFF1);
			LCD_string(7, 0, "AEB             ");
			LCD_string(0, 1, "US:             ");
			itoa((int32_t)USval,LCD_BUFF2);
			LCD_string(3, 1, LCD_BUFF2);
			LCD_string(8, 1, "IR:             ");
			itoa((int32_t)IRval,LCD_BUFF3);
			LCD_string(11, 1, LCD_BUFF3);
		}
		
		else if(run_mode == 3){
			Led_Set(3,flag);
		/*##################  UART Print  #####################*/
			USval = US_Get();
			UART_print(" US : ");
			itoa((int32_t)USval, string_temp);//US_Get()//sprintf(string_temp, "%d", ENC_Rate1_Return());
			UART_print(string_temp);
			
			IRval = ADC_Get(IR_PIN);//Read IR
			UART_print(",  IR : ");
			itoa((int32_t)IRval, string_temp);
			UART_print(string_temp);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, strA);
			itoa((int32_t)run_mode,LCD_BUFF1);
			LCD_string(5, 0, LCD_BUFF1);
			LCD_string(7, 0, "SensorTest   ");
			LCD_string(0, 1, "US:             ");
			itoa((int32_t)USval,LCD_BUFF2);
			LCD_string(3, 1, LCD_BUFF2);
			LCD_string(8, 1, "IR:             ");
			itoa((int32_t)IRval,LCD_BUFF3);
			LCD_string(11, 1, LCD_BUFF3);
		}
		
		else if(run_mode == 4){
			Led_Set(4,flag);
			/*ENCODER TEST*/
			UART_print("  ENC1 : ");
			itoa((int32_t)ENC_Rate1_Return(), string_temp);
			UART_print(string_temp);
			UART_print("  ENC2 : ");
			itoa((int32_t)ENC_Rate2_Return(), string_temp);
			UART_print(string_temp);
			//Speed Return test
			UART_print("  SPD1 : ");//string_temp);
			itoa((int32_t)MOTOR_Current_Speed_R(), string_temp);
			UART_print(string_temp);
			UART_print("  SPD2 : ");//string_temp);
			itoa((int32_t)MOTOR_Current_Speed_L(), string_temp);
			UART_print(string_temp);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, strA);
			itoa((int32_t)run_mode,LCD_BUFF1);
			LCD_string(5, 0, LCD_BUFF1);
			LCD_string(7, 0, "Speed            ");
			LCD_string(0, 1, "SR:              ");
			itoa((int32_t)MOTOR_Current_Speed_R(),LCD_BUFF2);
			LCD_string(3, 1, LCD_BUFF2);
			LCD_string(8, 1, "SL:              ");
			itoa((int32_t)MOTOR_Current_Speed_L(),LCD_BUFF3);
			LCD_string(11, 1, LCD_BUFF3);
		}
		
		UART_println(" ");//Newline
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
	IRval = ADC_Get(IR_PIN);
	if(flag_US){
		if( IRval > IR_POINT[4] ){//a2d(6) = PD1, NEED CONFIRM!
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

void Button_Select_RunMode(void){
	if(BUTTON_Read())
	{
		Led_Set_All(0);//Turn off all led
		switch(BUTTON_Read()){
			case 1 :{
				run_mode = 1;
				break;
			}
			case 2 :{
				run_mode = 2;
				break;
			}
			case 4 :{
				run_mode = 3;
				break;
			}
			case 8 :{
				run_mode = 4;
				break;
			}
			default:{
					break;
			}
			
		}
	}
}
/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

