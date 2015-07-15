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

void AEB(void);
void Button_Select_RunMode(void);
void Servo_Control(int16_t* current_servo_angle_pt);

void cameraSpeedControl ( void );

/**********************  Variables, Parameters *************************/
char string_temp[256] = " ";

vuint32_t i = 0;
vuint32_t t_current = 0, t_old = 0, cnt=0;
vuint32_t time_gap = 500;

//SPEED
int16_t Speed_Proposal_inMain = 50;
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
uint8_t flag_AEB_End = 0;
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
	int16_t current_servo_pulse = -1;
	
	//Select Mode with Tact switch button
	Button_Select_RunMode();
	
	laneProcess();
	
	if(ifLaneProcessEnd()){//Servo Control
		Servo_Control(&current_servo_angle);
		MOTOR_Servo(current_servo_angle);
	}
	
	if(run_mode != 0)
		//AEB();
	
	time_gap = 250;
	t_current = TIMER_GetRuntime()/time_gap;
	if (t_current != t_old) {
		/*##################  Timer Count  #####################*/
		t_old = t_current; cnt++;
		flag = !flag;//For led blinking
		
		// Is it working?? LED	############################
		if(run_mode == 0){
			Led_Set(1,flag);
			Speed_Proposal_inMain = 0;
			Speed_Propsal_Update(Speed_Proposal_inMain);
			itoa((int32_t)cnt, string_temp);
			
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
			Speed_Propsal_Update(Speed_Proposal_inMain);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, "SP   KS          ");
			itoa((int32_t)Speed_Proposal_inMain,LCD_BUFF2);
			LCD_string(2, 0, LCD_BUFF2);
			itoa((int32_t)kServo_for_LCD,LCD_BUFF2);
			LCD_string(8, 0, LCD_BUFF2);
						
			LCD_string(0, 1, "1:              ");
			itoa((int32_t)cam1LanePositionReturn(0),LCD_BUFF2);
			LCD_string(2, 1, LCD_BUFF2);
			itoa((int32_t)cam1LanePositionReturn(1),LCD_BUFF2);
			LCD_string(5, 1, LCD_BUFF2);
			
			LCD_string(8, 1, "2:              ");
			itoa((int32_t)cam2LanePositionReturn(0),LCD_BUFF3);
			LCD_string(10, 1, LCD_BUFF3);
			itoa((int32_t)cam2LanePositionReturn(1),LCD_BUFF3);
			LCD_string(13, 1, LCD_BUFF3);	
		}
			
		else if(run_mode == 2){	
			Led_Set(2,flag);
			Speed_Proposal_inMain = 50;
			Speed_Propsal_Update(Speed_Proposal_inMain);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, "SP   KS          ");
			itoa((int32_t)Speed_Proposal_inMain,LCD_BUFF2);
			LCD_string(2, 0, LCD_BUFF2);
			itoa((int32_t)kServo_for_LCD,LCD_BUFF2);
			LCD_string(8, 0, LCD_BUFF2);
						
			LCD_string(0, 1, "1:              ");
			itoa((int32_t)cam1LanePositionReturn(0),LCD_BUFF2);
			LCD_string(2, 1, LCD_BUFF2);
			itoa((int32_t)cam1LanePositionReturn(1),LCD_BUFF2);
			LCD_string(5, 1, LCD_BUFF2);
			
			LCD_string(8, 1, "2:              ");
			itoa((int32_t)cam2LanePositionReturn(0),LCD_BUFF3);
			LCD_string(10, 1, LCD_BUFF3);
			itoa((int32_t)cam2LanePositionReturn(1),LCD_BUFF3);
			LCD_string(13, 1, LCD_BUFF3);
		}
		
		else if(run_mode == 3){	
			Led_Set(3,flag);
			Speed_Proposal_inMain = 75;
			Speed_Propsal_Update(Speed_Proposal_inMain);
			
			LCD_ON();//x, y,*string
			LCD_string(0, 0, "SP   KS          ");
			itoa((int32_t)Speed_Proposal_inMain,LCD_BUFF2);
			LCD_string(2, 0, LCD_BUFF2);
			itoa((int32_t)kServo_for_LCD,LCD_BUFF2);
			LCD_string(8, 0, LCD_BUFF2);
						
			LCD_string(0, 1, "1:              ");
			itoa((int32_t)cam1LanePositionReturn(0),LCD_BUFF2);
			LCD_string(2, 1, LCD_BUFF2);
			itoa((int32_t)cam1LanePositionReturn(1),LCD_BUFF2);
			LCD_string(5, 1, LCD_BUFF2);
			
			LCD_string(8, 1, "2:              ");
			itoa((int32_t)cam2LanePositionReturn(0),LCD_BUFF3);
			LCD_string(10, 1, LCD_BUFF3);
			itoa((int32_t)cam2LanePositionReturn(1),LCD_BUFF3);
			LCD_string(13, 1, LCD_BUFF3);
		}
		
		else if(run_mode == 4){	
			Led_Set(4,flag);
			Speed_Proposal_inMain = 100;
			Speed_Propsal_Update(Speed_Proposal_inMain);
			
			LCD_ON();
			LCD_string(0, 0, "SP   KS          ");
			itoa((int32_t)Speed_Proposal_inMain,LCD_BUFF2);
			LCD_string(2, 0, LCD_BUFF2);
			itoa((int32_t)kServo_for_LCD,LCD_BUFF2);
			LCD_string(8, 0, LCD_BUFF2);

			LCD_string(0, 1, "1:              ");
			itoa((int32_t)cam1LanePositionReturn(0),LCD_BUFF2);
			LCD_string(2, 1, LCD_BUFF2);
			itoa((int32_t)cam1LanePositionReturn(1),LCD_BUFF2);
			LCD_string(5, 1, LCD_BUFF2);
			
			LCD_string(8, 1, "2:              ");
			itoa((int32_t)cam2LanePositionReturn(0),LCD_BUFF3);
			LCD_string(10, 1, LCD_BUFF3);
			itoa((int32_t)cam2LanePositionReturn(1),LCD_BUFF3);
			LCD_string(13, 1, LCD_BUFF3);
		}
		
	}
}

void main(void)
{
	init_peripherals();
	EnableExternalInterrupts();
	
	LCD_ON();//x, y,*string
	LCD_string(0, 0, " Start            ");

	while(1)
	{
	   DoMainLoop();
	}
}

/* ################ Other Functions Declaration #############*/

/*#############  AEB ######################*/
int16_t ReverseSpeed = -2047;
int16_t ReAccSpeed = 256;
void AEB(){				//0    10   20   30   40   50   60
	uint16_t IR_POINT[] ={500, 500, 500, 420, 330, 250, 210};
	int16_t ReverseSpeed = -2047;
	int16_t ReAccSpeed = 256;
	int16_t AEB_Distance = 30;
	uint32_t t_AEB_start = 0;

	t_AEB_start = TIMER_GetRuntime();
	
	while(US_Get() < 800){//while
		Speed_Propsal_Update(25);
		Led_Set(3,1);
		if(ADC_Get(IR_PIN) > (IR_POINT[4])){//is it wall?
			while(US_Get() < 500)
			{
				Led_Set(4,1);
				Speed_Propsal_Update(0);
				MOTOR_DC_R(ReverseSpeed);
				MOTOR_DC_L(ReverseSpeed);
				if( (TIMER_GetRuntime() - t_AEB_start) > 1500){
					flag_Wall = 1;}
			}
			MOTOR_DC_R(ReAccSpeed);
			MOTOR_DC_L(ReAccSpeed);
		}
		else{//slope, Re Acceleration
			//GPIO_Set(H_EN,1);
			//Speed_Propsal_Update(25);
			Led_Set(4,0);
			if(flag_Wall){
				Speed_Propsal_Update(0);
				MOTOR_DC_R(ReAccSpeed);
				MOTOR_DC_L(ReAccSpeed);
				TIMER_DelayMs(1);
				GPIO_Set(H_EN,0);
				flag_AEB_End = 1;
			}
		}		
	}
	//US_Get()>distance, slope end
	if(!flag_AEB_End){
		//GPIO_Set(H_EN,1);
		Led_Set(3,0);
		Led_Set(4,0);
		Speed_Propsal_Update(Speed_Proposal_inMain);
	}
	if(flag_AEB_End)
			GPIO_Set(H_EN,0);
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


int32_t cont_val_I_Servo = 0;
int32_t cont_val_P_Servo = 0;

void Servo_Control( int16_t* current_servo_angle_pt  ){
	int16_t ServoAngle = 0;
	int32_t ServoPulse = 0;
	int16_t Rdatum = 84;
	int16_t Ldatum = 38;
	int16_t datumline = (Ldatum+Rdatum)/2;
	int16_t datumOffset = (Ldatum+Rdatum)/2 - (0+128)/2;
	int16_t kpServo=10;
	int16_t kpServo_speed_and_servoWheelRatio = 10;
	int16_t RCurrent = 0;
	int16_t LCurrent = 0;
	int16_t currentLine = 0;
	int16_t error_center = 0;
	char temptStr[8] = " ";
	
	if( (ifSchoolZone()==0) && (ifCrossSection()==0) || (cam1LanePositionReturn(0) == -1 || cam2LanePositionReturn(0) == -1) && (cam1LanePositionReturn(0) == -1 || cam2LanePositionReturn(0) == -1) )
	{
		RCurrent = cam1LanePositionReturn(0);
		LCurrent = cam2LanePositionReturn(0);
		
		if ( RCurrent == -1 ) //&& LCurrent != -1
		{
			currentLine = (LCurrent +128)/2;
		}
		if ( LCurrent == -1 ) //&& RCurrent != -1
		{
			currentLine = (RCurrent)/2;
		}
		if ( (RCurrent != -1) && (LCurrent != -1) )//둘다잡힘
		{
			currentLine = (LCurrent+RCurrent) / 2;
		}

		error_center = (datumline - currentLine);

		kpServo = ADC_Get(POT_1);
		kServo_for_LCD = kpServo;
		
		cont_val_P_Servo = kpServo*error_center*MOTOR_Current_Speed_R();
		
		ServoAngle = cont_val_P_Servo/500/100;
	}
	if( ifCrossSection() && ifSchoolZone() ) //두꺼운거 잡힌 경우
	{	
		// 근데 2개 이상 인식된 경우 
		{
			ServoAngle = 0;
		}
	}
	if( ifCrossSection() )
	{
		ServoAngle = 0;
	}
	
	*current_servo_angle_pt = ServoAngle;
}

void cameraSpeedControl ( void )
{
	
	if ( ifSchoolZone() && ifCrossSection() ) 
	{
		if ( ( cam1LanePositionReturn(0) + K_CAMCAM_DISTANCE + NUM_OF_PIXEL - cam2LanePositionReturn(0) )
				< ( (NUM_OF_PIXEL + K_CAMCAM_DISTANCE) / CROSS_SCHOOL_RATIO ) )
		{
			Speed_Propsal_Update(20);
		}
	}
	if ( ifError() )
	{
		Speed_Propsal_Update(15);
	}
	
}

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

