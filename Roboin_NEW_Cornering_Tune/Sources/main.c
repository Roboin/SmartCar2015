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
void Servo_Control(int16_t* current_servo_angle_pt);//void);

void cameraSpeedControl ( int8_t schoolZoneFlag, int8_t crossSectionFlag);

/**********************  Variables, Parameters *************************/
char string_temp[256] = " ";//"abcd1234";

vuint32_t i = 0;
vuint32_t t_current = 0, t_old = 0, cnt=0;
vuint32_t time_gap = 500;

//SENSOR
uint32_t USval = 1500;
uint32_t IRval = 1;

//SPEED
int16_t Speed_Proposal_inMain = 300;
int16_t k_test = 0;

//LCD		   0123456789012345
char strA[] = "Mode:           ";
char strB[] = "TIME:           ";
char LCD_BUFF1[8] = "";
char LCD_BUFF2[8] = "";
char LCD_BUFF3[8] = "";
char LCD_BUFF4[8] = "";

int16_t kServo_for_LCD = -1;

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
	int16_t current_servo_angle = -1;
	
	//Select Mode with Tact switch button
	Button_Select_RunMode();
	
	//AEB();
	
	laneProcess();
	Servo_Control(&current_servo_angle);
	MOTOR_Servo(current_servo_angle);
	
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
			Speed_Proposal_inMain = 0;
			Speed_Propsal_Update(Speed_Proposal_inMain,0,k_test);
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
			Speed_Proposal_inMain = 25;
			Speed_Propsal_Update(Speed_Proposal_inMain,0,k_test);
//			laneProcess();
//			Servo_Control(&current_servo_angle);
//			MOTOR_Servo(current_servo_angle);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, "SP   CSA   kS      ");
			itoa((int32_t)Speed_Proposal_inMain,LCD_BUFF2);
			LCD_string(2, 0, LCD_BUFF2);
			itoa((int32_t)current_servo_angle,LCD_BUFF2);
			LCD_string(8, 0, LCD_BUFF2);
			itoa((int32_t)kServo_for_LCD,LCD_BUFF2);
			LCD_string(13, 0, LCD_BUFF2);
						
			LCD_string(0, 1, "1:              ");
			itoa((int32_t)cam1LanePositionReturn(0),LCD_BUFF2);
			LCD_string(2, 1, LCD_BUFF2);
			itoa((int32_t)cam1LanePositionReturn(1),LCD_BUFF2);
			LCD_string(5, 1, LCD_BUFF2);
			//itoa((int32_t)cam1LanePositionReturn(2),LCD_BUFF2);
			//LCD_string(10, 0, LCD_BUFF2);
			
			LCD_string(8, 1, "2:              ");
			itoa((int32_t)cam2LanePositionReturn(0),LCD_BUFF3);
			LCD_string(10, 1, LCD_BUFF3);
			itoa((int32_t)cam2LanePositionReturn(1),LCD_BUFF3);
			LCD_string(13, 1, LCD_BUFF3);
			//itoa((int32_t)cam2LanePositionReturn(2),LCD_BUFF4);
			//LCD_string(10, 1, LCD_BUFF4);
			
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
		}
			
		else if(run_mode == 2){	
			Led_Set(2,flag);
			Speed_Proposal_inMain = 100;
			Speed_Propsal_Update(Speed_Proposal_inMain,0,k_test);
//			laneProcess();
//			Servo_Control(&current_servo_angle);
//			MOTOR_Servo(current_servo_angle);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, "SP   CSA   kS      ");
			itoa((int32_t)Speed_Proposal_inMain,LCD_BUFF2);
			LCD_string(2, 0, LCD_BUFF2);
			itoa((int32_t)current_servo_angle,LCD_BUFF2);
			LCD_string(8, 0, LCD_BUFF2);
			itoa((int32_t)kServo_for_LCD,LCD_BUFF2);
			LCD_string(13, 0, LCD_BUFF2);
						
			LCD_string(0, 1, "1:              ");
			itoa((int32_t)cam1LanePositionReturn(0),LCD_BUFF2);
			LCD_string(2, 1, LCD_BUFF2);
			itoa((int32_t)cam1LanePositionReturn(1),LCD_BUFF2);
			LCD_string(5, 1, LCD_BUFF2);
			//itoa((int32_t)cam1LanePositionReturn(2),LCD_BUFF2);
			//LCD_string(10, 0, LCD_BUFF2);
			
			LCD_string(8, 1, "2:              ");
			itoa((int32_t)cam2LanePositionReturn(0),LCD_BUFF3);
			LCD_string(10, 1, LCD_BUFF3);
			itoa((int32_t)cam2LanePositionReturn(1),LCD_BUFF3);
			LCD_string(13, 1, LCD_BUFF3);
			//itoa((int32_t)cam2LanePositionReturn(2),LCD_BUFF4);
			//LCD_string(10, 1, LCD_BUFF4);
			
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

void Servo_Control( int16_t* current_servo_angle_pt  ){/*cam1LanePositionReturn(), cam1LanePositionReturn(), flag*/
	int16_t ServoAngle = 0;
	int32_t ServoPulse = 0;
	int16_t Ldatum = 71;//43;//30; 정상 상태 왼쪽 선 위치
	int16_t Rdatum = 91;//74;//100; 정상 상태 오른쪽 선 위치
	int16_t datumline = (Ldatum+Rdatum)/2;
	int16_t datumOffset = (Ldatum+Rdatum)/2 - (0+128)/2;
	int16_t kpServo=10;//000;
	int16_t kpServo_speed_and_servoWheelRatio = 10;
	int16_t RCurrent = 0;
	int16_t LCurrent = 0;
	int16_t currentLine = 0;
	int16_t error_center = 0;
	char temptStr[8] = " ";
	
	if( (ifSchoolZone()==0) && (ifCrossSection()==0)
	/*두꺼운거 잡혔는데 1개만 잡힌경우*/ || (cam1LanePositionReturn(0) == -1 || cam2LanePositionReturn(0) == -1) && (cam1LanePositionReturn(0) == -1 || cam2LanePositionReturn(0) == -1) )
	{//정상경우
		RCurrent = cam1LanePositionReturn(0);
		LCurrent = cam2LanePositionReturn(0);
		
		if ( RCurrent == -1 ) //&& LCurrent != -1
		{
			currentLine = (LCurrent +128)/2;//+ Rdatum)/2;//+ START_PIXEL) / 2;
		}
		if ( LCurrent == -1 ) //&& RCurrent != -1
		{
			currentLine = (RCurrent)/2;//+ Ldatum)/2;// END_PIXEL) / 2;
		}
		if ( (RCurrent == -1) && (LCurrent == -1) ) //both -1, straight
		{
			currentLine = datumline;
		}
		if ( (RCurrent != -1) && (LCurrent != -1) )//둘다잡힘
		{
			currentLine = (LCurrent+RCurrent) / 2;
		}

		error_center = (datumline - currentLine);

		kpServo = ADC_Get(POT_1)/100;
		kServo_for_LCD = kpServo;
		//atan 모드
		//ServoAngle = kpServo_speed_and_servoWheelRatio * (Speed_Proposal_inMain * Speed_Proposal_inMain ) * ( 312 / 10000 * (error_center* kpServo )*(error_center* kpServo ) + 4321 / 10000 * (error_center* kpServo ));
		
		//정비례 모드
		ServoAngle = error_center/kpServo;//(Speed_Proposal_inMain * Speed_Proposal_inMain )*error_center/kpServo;//*kpServo;
		
		//ServoPulse = kpServo_speed_and_servoWheelRatio * (Speed_Proposal_inMain * Speed_Proposal_inMain ) * ( 312 / 10000 * (error_center* kpServo )*(error_center* kpServo ) + 4321 / 10000 * (error_center* kpServo ));
		UART_print(" Servo Angle ");
		itoa( ServoAngle * 1000, temptStr );
		//itoa( ServoPulse * 1000, temptStr );
		UART_print(temptStr);
	}
	if( ifCrossSection() && ifSchoolZone() ) //두꺼운거 잡힌 경우
	{	
		// 근데 2개 이상 인식된 경우 
		{
			ServoAngle = 0;
		}
	}
	if( ifSchoolZone() )
	{
		ServoAngle = 0;//?
	}
	*current_servo_angle_pt = ServoAngle;
	//MOTOR_Servo(ServoAngle);
}

void cameraSpeedControl ( int8_t schoolZoneFlag, int8_t crossSectionFlag ) /*camera서 오는 값으로 속도 조절*/
{
	if ( schoolZoneFlag && crossSectionFlag ) Speed_Propsal_Update(20,0,0);
	else{}
}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

