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
			/*##################  KPR  #####################*/
			/*P control TEST*/
			MOTOR_Servo(0);
			k_test = (int16_t)ADC_Get(POT_1)/50;
			Speed_Propsal_Update(Speed_Proposal_inMain,1,k_test);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, "SP:            ");
			itoa((int32_t)Speed_Proposal_inMain,LCD_BUFF1);
			LCD_string(3, 0, LCD_BUFF1);
			LCD_string(7, 0, "kpR:        ");
			itoa((int32_t)k_test,LCD_BUFF2);
			LCD_string(11, 0, LCD_BUFF2);
			
			LCD_string(0, 1, "SR:              ");
			itoa((int32_t)MOTOR_Current_Speed_R(),LCD_BUFF2);
			LCD_string(3, 1, LCD_BUFF2);
			LCD_string(8, 1, "SL:              ");
			itoa((int32_t)MOTOR_Current_Speed_L(),LCD_BUFF3);
			LCD_string(11, 1, LCD_BUFF3);
			
		}
		
		else if(run_mode == 3){
			//int16_t s_angle = ADC_Get(POT_1)/14 - 37;
			Led_Set(3,flag);
		/*##################  KPL  #####################*/
			/*P control TEST*/
			MOTOR_Servo(0);
			k_test = (int16_t)ADC_Get(POT_1)/50;
			Speed_Propsal_Update(Speed_Proposal_inMain,2,k_test);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, "SP:            ");
			itoa((int32_t)Speed_Proposal_inMain,LCD_BUFF1);
			LCD_string(3, 0, LCD_BUFF1);
			LCD_string(7, 0, "kpL:        ");
			itoa((int32_t)k_test,LCD_BUFF2);
			LCD_string(11, 0, LCD_BUFF2);
			
			LCD_string(0, 1, "SR:              ");
			itoa((int32_t)MOTOR_Current_Speed_R(),LCD_BUFF2);
			LCD_string(3, 1, LCD_BUFF2);
			LCD_string(8, 1, "SL:              ");
			itoa((int32_t)MOTOR_Current_Speed_L(),LCD_BUFF3);
			LCD_string(11, 1, LCD_BUFF3);
			
			
			/*MOTOR_Servo(s_angle);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, "Servo Tune:             ");
			//itoa((int32_t)Speed_Proposal_inMain,LCD_BUFF1);
			//LCD_string(3, 0, LCD_BUFF1);
			//LCD_string(7, 0, "kp:        ");
			//itoa((int32_t)k_test,LCD_BUFF2);
			//LCD_string(10, 0, LCD_BUFF2);
			
			LCD_string(0, 1, "SA:              ");
			itoa((int32_t)s_angle,LCD_BUFF2);
			LCD_string(3, 1, LCD_BUFF2);*/
		}
		
		else if(run_mode == 4){
			Led_Set(4,flag);
			//####################  KPI  ####################
			/*P control TEST 300*/
			MOTOR_Servo(0);
			k_test = (int16_t)ADC_Get(POT_1)/50;
			Speed_Propsal_Update(Speed_Proposal_inMain,3,k_test);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, "SP:            ");
			itoa((int32_t)Speed_Proposal_inMain,LCD_BUFF1);
			LCD_string(3, 0, LCD_BUFF1);
			LCD_string(7, 0, "ki:        ");
			itoa((int32_t)k_test,LCD_BUFF2);
			LCD_string(10, 0, LCD_BUFF2);
			
			LCD_string(0, 1, "SR:              ");
			itoa((int32_t)MOTOR_Current_Speed_R(),LCD_BUFF2);
			LCD_string(3, 1, LCD_BUFF2);
			LCD_string(8, 1, "SL:              ");
			itoa((int32_t)MOTOR_Current_Speed_L(),LCD_BUFF3);
			LCD_string(11, 1, LCD_BUFF3);
						
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

/*#############  AEB ##### 그냥 DomainLoop 안에 놓지 말것, ADC 읽어오는 주기가 너무 빨라 멈춤.#################*/
void AEB(){				//0    10   20   30   40   50   60
	uint16_t IR_POINT[] ={500, 500, 500, 420, 330, 250, 210};//ADC value data for 10-50cm
	int16_t ReverseSpeed = -2047;
	int16_t AEB_Distance = 30;
	IRval = ADC_Get(IR_PIN);
	
	//USval = US_Get();
	while(US_Get() < 800){
		//MOTOR_BrakeEngine();
		MOTOR_DC_R(ReverseSpeed);
		MOTOR_DC_L(ReverseSpeed);
		Speed_Propsal_Update(25,0,k_test);
		
		IRval = ADC_Get(IR_PIN);
		if( IRval > (IR_POINT[2]+IR_POINT[3])/2 ){//a2d(6) = PD1, NEED CONFIRM!
			//IR detected, Stop the car.
			MOTOR_BrakeEngine();
			TIMER_DelayMs(1);
			MOTOR_DC_R(ReverseSpeed);
			MOTOR_DC_L(ReverseSpeed);
			TIMER_DelayMs(1000);
			MOTOR_BrakeEngine();
			GPIO_Set(H_EN,0);
			flag_IR = 1;
			flag_Wall = 1;
		}
		else{//경사로, Re Acceleration
			//Speed_Control(25);
			flag_IR = 0;
			flag_Slope = 1;
		}
	}
	//else{
		GPIO_Set(H_EN,1);
		flag_US = 0;
		flag_Wall = 0;
		flag_Slope = 0;
		Speed_Propsal_Update(Speed_Proposal_inMain,0,k_test);
	//}
	
	if(flag_US){
		
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

int16_t ServoAngle = 0;
void Servo_Control(void){/*cam1LanePositionReturn(), cam1LanePositionReturn(), flag*/
	int16_t Ldatum = 43;//30; 
	int16_t Rdatum = 74;//100; 
	int16_t datumline = (Ldatum+Rdatum)/2;
	int16_t kpServo=1;
	int16_t kpServo_speed=10;
	kpServo = Speed_Proposal_inMain/kpServo_speed;
	if( (ifSchoolZone==0) && (ifCrossSection==0) ){
		int16_t RCurrent = cam1LanePositionReturn(0);
		int16_t LCurrent = cam2LanePositionReturn(0);
		int16_t Currentline = (LCurrent+RCurrent)/2;
		ServoAngle = (datumline - Currentline)*kpServo;
	}
	else if(ifCrossSection()){
		ServoAngle = 0;
	}
	else if(ifSchoolZone()){
		ServoAngle = 0;//?
	}
}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

