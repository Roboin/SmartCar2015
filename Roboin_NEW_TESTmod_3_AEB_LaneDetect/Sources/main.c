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

void cameraSpeedControl ( void );

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
			Speed_Propsal_Update(Speed_Proposal_inMain,1,k_test);
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
			//UART_println(" CAM datum");
			
			laneProcess();
			Servo_Control(&current_servo_angle);
			MOTOR_Servo(current_servo_angle);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, "CSA:              ");
			itoa((int32_t)current_servo_angle,LCD_BUFF2);
			LCD_string(4, 0, LCD_BUFF2);
						
			LCD_string(0, 1, "1:              ");
			itoa((int32_t)cam1LanePositionReturn(0),LCD_BUFF2);
			LCD_string(2, 1, LCD_BUFF2);
			/*itoa((int32_t)cam1LanePositionReturn(1),LCD_BUFF2);
			LCD_string(6, 0, LCD_BUFF2);
			itoa((int32_t)cam1LanePositionReturn(2),LCD_BUFF2);
			LCD_string(10, 0, LCD_BUFF2);*/
			
			LCD_string(8, 1, "2:              ");
			itoa((int32_t)cam2LanePositionReturn(0),LCD_BUFF3);
			LCD_string(10, 1, LCD_BUFF3);
			/*itoa((int32_t)cam2LanePositionReturn(1),LCD_BUFF3);
			LCD_string(6, 1, LCD_BUFF3);
			itoa((int32_t)cam2LanePositionReturn(2),LCD_BUFF4);
			LCD_string(10, 1, LCD_BUFF4);*/
			
			
			UART_print("  LANE1 : ");//string_temp);
			itoa((int32_t)cam1LanePositionReturn(0), string_temp);
			UART_print(string_temp);
			UART_print(",  LANE2 : ");//string_temp);
			itoa((int32_t)cam2LanePositionReturn(0), string_temp);
			UART_println(string_temp);
			
			
			
			itoa((int32_t)current_servo_angle, string_temp);
			UART_println(string_temp);
			
			//MOTOR_Servo( current_servo_angle );
			
			/*
			 * UART_println("");
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
			int16_t s_angle = ADC_Get(POT_1)/14 - 37;
			Led_Set(3,flag);
		/*##################  KPL  #####################*/
			/*P control TEST*/
			//MOTOR_Servo(0);
			/*k_test = (int16_t)ADC_Get(POT_1)/50;
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
			LCD_string(11, 1, LCD_BUFF3);*/
			
			Speed_Propsal_Update(0,0,k_test);
			
			MOTOR_Servo(s_angle);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, "Servo Tune:             ");
			//itoa((int32_t)Speed_Proposal_inMain,LCD_BUFF1);
			//LCD_string(3, 0, LCD_BUFF1);
			//LCD_string(7, 0, "kp:        ");
			//itoa((int32_t)k_test,LCD_BUFF2);
			//LCD_string(10, 0, LCD_BUFF2);
			
			LCD_string(0, 1, "SA:              ");
			itoa((int32_t)s_angle,LCD_BUFF2);
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
	int16_t kpServo=10;//000;
	int16_t kpServo_speed_and_servoWheelRatio = 10;
	int16_t RCurrent = 0;
	int16_t LCurrent = 0;
	int16_t currentLine = 0;
	int16_t error_center = 0;
	char temptStr[8] = " ";
	
	RCurrent = cam1LanePositionReturn(0);
	LCurrent = cam2LanePositionReturn(0);

	if( ifCrossSection() && !ifSchoolZone() ) //두꺼운거 잡힌 경우
	{	 
		ServoAngle = 0;

	}
	
	/* schoolZone flag만 올라가는 경우는 없다.*/
	
	
	//	if( (ifSchoolZone()==0) && (ifCrossSection()==0)
	//	/*두꺼운거 잡혔는데 1개만 잡힌경우*/ || (cam1LanePositionReturn(0) == -1 || cam2LanePositionReturn(0) == -1) && (cam1LanePositionReturn(0) == -1 || cam2LanePositionReturn(0) == -1) )
	//	{//정상경우
	if ( RCurrent == -1 ) //&& LCurrent != -1
	{
		currentLine = LCurrent / 2;
	}
	else if ( LCurrent == -1 ) //&& RCurrent != -1
	{
		currentLine = (RCurrent + NUM_OF_PIXEL) / 2;
	}
	else
	{
		currentLine = (LCurrent+RCurrent) / 2;
	}

	ServoAngle = kpServo_speed_and_servoWheelRatio * (Speed_Proposal_inMain * Speed_Proposal_inMain ) * ( 312 / 10000 * ((datumline - currentLine)* kpServo )*((datumline - currentLine)* kpServo ) + 4321 / 10000 * ((datumline - currentLine)* kpServo ));
	
	error_center = (datumline - currentLine);
	
	
	kpServo=2;
	//atan 모드
	//ServoAngle = kpServo_speed_and_servoWheelRatio * (Speed_Proposal_inMain * Speed_Proposal_inMain ) * ( 312 / 10000 * (error_center* kpServo )*(error_center* kpServo ) + 4321 / 10000 * (error_center* kpServo ));
	
	//정비례 모드
	ServoAngle = error_center/kpServo;//(Speed_Proposal_inMain * Speed_Proposal_inMain )*error_center/kpServo;//*kpServo;
	
	ServoPulse = kpServo_speed_and_servoWheelRatio * (Speed_Proposal_inMain * Speed_Proposal_inMain ) * ( 312 / 10000 * (error_center* kpServo )*(error_center* kpServo ) + 4321 / 10000 * (error_center* kpServo ));
	
	//print for test
	//itoa( ServoAngle * 1000, temptStr );
	itoa( ServoPulse * 1000, temptStr );
	UART_print(temptStr);
	
	
	if( ifSchoolZone() )
	{
		ServoAngle = 0;//?
	}
	*current_servo_angle_pt = ServoAngle;
	//MOTOR_Servo(ServoAngle);
}

void cameraSpeedControl ( void ) /*camera서 오는 값으로 속도 조절*/
{
	
	if ( ifSchoolZone() && ifCrossSection() ) 
	{
		if ( ( cam1LanePositionReturn(0) + K_CAMCAM_DISTANCE + NUM_OF_PIXEL - cam2LanePositionReturn(0) )
				< ( (NUM_OF_PIXEL + K_CAMCAM_DISTANCE) / CROSS_SCHOOL_RATIO ) )
		{
			Speed_Propsal_Update(20,0,0);
		}
	}
	
}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

