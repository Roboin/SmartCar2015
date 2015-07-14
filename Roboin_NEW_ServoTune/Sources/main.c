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

//void Speed_Control(int16_t proposal_speed);
void AEB(void);
void Button_Select_RunMode(void);
void Servo_Control(void);

/**********************  Variables, Parameters *************************/
//UART
vuint32_t i = 0;
vuint32_t t_current = 0, t_old = 0, cnt=0;
vuint32_t time_gap = 500;

//SENSOR
uint32_t USval = 1500;
uint32_t IRval = 1;

//SPEED
int16_t Speed_Proposal_inMain = 0;
int16_t k_test = 0;

//LCD		   0123456789012345
char strA[] = "Mode:           ";
char strB[] = "TIME:           ";
char string_temp[256] = " ";//"abcd1234";
char LCD_BUFF1[8];
char LCD_BUFF2[8];
char LCD_BUFF3[8];
char LCD_BUFF4[8];

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
	
	//AEB();
	
	time_gap = 250;//(ADC_Get(POT_1);
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
			
			MOTOR_Servo(0);
		}
		
		/*##################  Speed test #####################*/
		else if(run_mode == 1){
			Led_Set(1,flag);
			Speed_Proposal_inMain = (int16_t)ADC_Get(POT_1)/3;
			MOTOR_Servo(0);
			//k_test = ADC_Get(POT_1);
			//Speed_Propsal_Update(Speed_Proposal_inMain,0,k_test);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, "SP:            ");
			itoa((int32_t)Speed_Proposal_inMain,LCD_BUFF1);
			LCD_string(3, 0, LCD_BUFF1);
			LCD_string(6, 0, "US        ");
			itoa((int32_t)US_Get(),LCD_BUFF2);
			LCD_string(8, 0, LCD_BUFF2);
			LCD_string(11, 0, "IR        ");
			itoa((int32_t)IRval,LCD_BUFF2);
			LCD_string(13, 0, LCD_BUFF2);
			
			LCD_string(0, 1, "SR:              ");
			itoa((int32_t)MOTOR_Current_Speed_R(),LCD_BUFF2);
			LCD_string(3, 1, LCD_BUFF2);
			LCD_string(8, 1, "SL:              ");
			itoa((int32_t)MOTOR_Current_Speed_L(),LCD_BUFF3);
			LCD_string(11, 1, LCD_BUFF3);
		}
			
		else if(run_mode == 2){	
			/*##################  Servo tune  #####################*/
			int16_t s_angle = ADC_Get(POT_1)/14 - 37;
			Led_Set(2,flag);
		/*##################  UART Print  #####################*/
			MOTOR_Servo(s_angle);
			//MOTOR_Servo_withPWM(servo_pulse);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, "Servo Tune Angle:             ");
			LCD_string(0, 1, "SA:              ");
			itoa((int32_t)s_angle,LCD_BUFF2);
			//itoa((int32_t)servo_pulse,LCD_BUFF2);
			LCD_string(3, 1, LCD_BUFF2);
			
		}
		
		else if(run_mode == 3){
			//int16_t s_angle = ADC_Get(POT_1)/14 - 37;
			int32_t servo_pulse = ADC_Get(POT_1)*6 - 3200;
			Led_Set(3,flag);
		/*##################  UART Print  #####################*/
			//MOTOR_Servo(s_angle);
			MOTOR_Servo_withPWM(servo_pulse);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, "Servo Tune Pulse:             ");
			LCD_string(0, 1, "SA:              ");
			//itoa((int32_t)s_angle,LCD_BUFF2);
			itoa((int32_t)servo_pulse,LCD_BUFF2);
			LCD_string(3, 1, LCD_BUFF2);
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
	uint32_t t_AEB_start = 0;
	IRval = ADC_Get(IR_PIN);
	t_AEB_start = TIMER_GetRuntime();
	while(US_Get() < 1000){
		Speed_Proposal_inMain = 25;
		Speed_Propsal_Update(25,0,k_test);
		Led_Set(3,1);
		if(ADC_Get(IR_PIN) > (IR_POINT[4])){//is it wall?
			while(US_Get() < 550)//(ADC_Get(IR_PIN) > (IR_POINT[5]))
			{
				Led_Set(4,1);
				Speed_Propsal_Update(-300,0,k_test);
				Speed_Proposal_inMain = -300;
				MOTOR_DC_R(-2047);//ReverseSpeed);
				MOTOR_DC_L(-2047);//ReverseSpeed);
				//flag_Wall = 1;
			}
			MOTOR_DC_R(2047);
			MOTOR_DC_L(2047);
			TIMER_DelayMs(5);
			MOTOR_BrakeEngine();
			if( (TIMER_GetRuntime() - t_AEB_start) > 5000){
				flag_Wall = 1;}
			if(flag_Wall){
				GPIO_Set(H_EN,0);}
				
		}
		else{//slope 경사로, Re Acceleration
			GPIO_Set(H_EN,1);
			Speed_Propsal_Update(25,0,k_test);
			Led_Set(4,0);
			if(flag_Wall){
				GPIO_Set(H_EN,0);}
		}
		
	}
	//US_Get()>1000
	GPIO_Set(H_EN,1);
	Led_Set(3,0);
	Led_Set(4,0);
	Speed_Propsal_Update(Speed_Proposal_inMain,0,k_test);
	
	if(flag_Wall){
		GPIO_Set(H_EN,0);}
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

