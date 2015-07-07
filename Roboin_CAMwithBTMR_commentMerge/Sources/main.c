#include "stdlib.h"
#include "stdio.h"

#include "ngv_dmu.h"
#include "rappid_ref.h"
#include "rappid_utils.h"
#include "sys_init.h"
#include "gpio.h"
#include "demoled.h"
#include "demoswitch.h"
#include "basetimer.h"
#include "serialdebug.h"
#include "myuart.h"
#include "youruart.h"
#include "charlcd.h"
#include "freerunadc.h"
#include "freerunusonic.h"
#include "rotaryencoder.h"
#include "motorcontrol.h"
#include "camera.h"

#include "adc_drv.h"

void InitPeripherals(void);
void DoMainLoop(void);
void Callback_s(char* str);
void Callback_e(char* str);

void Speed_Control(int16_t proposal_speed);

void InitPeripherals(void)
{
	sys_init_fnc();
	DLED_Init();
	DSW_Init();
	BTMR_Init();
	SDBG_Init();
	MUART_Init();
	YUART_Init();
	CLCD_Init();
	FADC_Init(0xfff3);
	FUSS_Init(FUSS_CH_ALL);
	RENC_Init();
	MCTL_Init();
	CAM_Init();
	
	EnableExternalInterrupts();
}

int16_t DC_Speed = 500;
void Callback_s(char* str)
{
	//int DC_SPEED;
	
	if (sscanf(str, "%d", &DC_Speed)<1) {
		return;
	}
	
	MCTL_DC_R((int16_t)DC_Speed);
	MCTL_DC_L((int16_t)DC_Speed);
	SDBG_Printf("\n");
}


void Callback_e(char* str)
{
	int speed;
	
	if (sscanf(str, "%d", &speed)<1) {
		return;
	}
	
	//MCTL_RotateEngine((int16_t)speed);
	SDBG_Printf("\n");
}

//##########DO MAIN LOOP################//
int8_t flag = 1;
int16_t i_serial=0;
uint16_t cam_th= 200;
uint16_t dipsw = 0;
int16_t blinking_LED = DLED_LED1;

int16_t time_gap = 250;

int16_t sAngle = 0;

int32_t enc1_current=0, enc1_old=0, enc2_current=0, enc2_old=0;
int32_t enc1_rate=0, enc2_rate=0;

void DoMainLoop(void)
{
	//COUNT FOR TIMER
	static uint32_t t_current, t_old=(uint32_t)-1, t_test,cnt=0;
	//DIPSW - 1-Camera Run / 2-cam_th / 3-.! or %3d / 4-cam_pulsewidth(exposuretime)
	//dipsw = DSW_GetDipAll();
	//RUN CAMERA //with dip sw 1
	/*CAM RUN mode 1 or 2
	 * main loop 돌때 마다 스위치 값 읽어서 camera run mode를 바꿔주는거
	 * 카메라를 돌리는건 periodic interrupt timer channel 0 기준 */
	if(dipsw & 0b0001){//sw1 //0x1
		CAM_RUN_MODE_SETUP(2);//up
		DLED_Set(DLED_LED1,0);
		blinking_LED= DLED_LED2;
		//CAM_RUN2(); 재학은 보시오. 이거 왜 살아 있는 거냠. 실수냐, 없애도 !.은 잘작동 하더라
	}
	else{
		CAM_RUN_MODE_SETUP(1);//down
		DLED_Set(DLED_LED2,0);
		blinking_LED= DLED_LED1;
	}
	
	//SERIAL COMMUNICATE
	if (SDBG_IsEvents())
		SDBG_ExecuteCallback();
	
	if (MUART_IsEvents())
		MUART_ExecuteCallback();
	
	//servo test in main
	sAngle = (FADC_Get(1)-870)*2*37/156-37;
	MCTL_Servo(sAngle);
			
	//DC test
	//if(DSW_GetTactAll() & 0x1){
	//DC_Speed = (FADC_Get(1)-800)*10;
	MCTL_DC_R(DC_Speed);
	MCTL_DC_L(DC_Speed);
	
	//TIMER FOR EACH N ms	
	time_gap = 200;
	t_current = BTMR_GetRuntime()/time_gap;
	if (t_current != t_old) {
		t_test = CAM_MICRO_SEC_RETURN();
		SDBG_Printf("\n%u: %d", cnt++, t_test);
		t_old = t_current;
		DLED_Toggle(blinking_LED);
		
		/*DIP SWitch
		 *1-Camera Run Mode
		 *2-cam_threshold
		 *3- .! or %3d 로 카메라 pixel 중 라인 위치, 명암값 출력 
		 *4-cam_pulsewidth(exposure time)*/
		dipsw = DSW_GetDipAll();
		
		//Threshold
		if(dipsw & 0b0010)//sw2 //0x2
			cam_th = (uint16_t)(FADC_Get(1)-800)*5;
		else
			cam_th = CAM_AVG();
		//camera exposure time
		if(dipsw & 0b1000)//sw4//0x8
			CAM_PulseWidth(100);//up
		else//down
			CAM_PulseWidth(50);
		
		//ENCODER TEST
		//*/
		/*
		enc1_current = RENC_Get1();
		enc2_current = RENC_Get2();
		enc1_rate = enc1_current - enc1_old;
		enc2_rate = enc2_current - enc2_old;
		enc1_old = enc1_current;
		enc2_old = enc2_current;
		//SDBG_Printf("\n%u: Speed : %4d Rval : %4d %4d    RATE : %4d %4d", cnt++, DC_Speed,RENC_Get1(),RENC_Get2(), enc1_rate, enc2_rate);
		
		Speed_Control(3);//3cm/s
		//DC_Speed = (FADC_Get(1)-800)*10;//(A2D_GetChResult_10bit(1)-800)*10;
		MCTL_DC_R(DC_Speed);
		MCTL_DC_L(DC_Speed);
		SDBG_Printf("\n%u: Speed : %4d Rval : %4d RATE : %4d", cnt++, DC_Speed,RENC_Get1(), enc1_rate);
		//SDBG_Printf("\n%u: US : %8u  IR : %4d", cnt++, FUSS_GetMm(FUSS_CH0),FADC_Get(6));//adc6 = pd2 = IR
		*/
		
		//Servo test
		//MCTL_Servo(sAngle);
		//sAngle +=5;
		if(sAngle > MCTL_SWHEEL_MAX)
		{
			sAngle = -MCTL_SWHEEL_MAX;
		}
		//SDBG_Printf("\nServo Angle : %d",sAngle);
		
		//if(dipsw & 0x8){//sw4, SDBG
			if(flag==0){
				flag = 1;
			}
		//}
	}
	
	if(flag){
		SDBG_Printf("\n %4d TH:%3d,%3d,%3d\t",t_current,cam_th,CAM_MAX(),CAM_MIN());
		for(i_serial = 0; i_serial < NUM_OF_PIXEL; i_serial+=2){//i_serial++){
			if(dipsw & 0b0100){//up//0x04
				if(CAM_DATA1(i_serial) > cam_th){
					SDBG_Printf(".");
				}
				else{ //if(CAM_READ(i_serial) , cam_th)
					SDBG_Printf("!");
				}
			}
			else{//down
				SDBG_Printf("%3d ",CAM_DATA1(i_serial));
			}
		}
		SDBG_Printf("||");
		for(i_serial = 0; i_serial < NUM_OF_PIXEL; i_serial+=2){//i_serial++){
			if(dipsw & 0b0100){//up//0x04
				if(CAM_DATA2(i_serial) > cam_th){
					SDBG_Printf(".");
				}
				else{ //if(CAM_READ(i_serial) , cam_th)
					SDBG_Printf("!");
				}
			}
			else{//down
				SDBG_Printf("%3d ",CAM_DATA2(i_serial));
			}
		}
		flag = 0;
	}
}

void Speed_Control(int16_t proposal_speed){
	int16_t pulse_per_cm = 11;
	int32_t proposal_enc_rate = pulse_per_cm*proposal_speed*time_gap/1000;
	int16_t kp = 1;
	if(enc1_rate > proposal_enc_rate){
		DC_Speed -= kp;
	}
	if(enc1_rate < proposal_enc_rate){
		DC_Speed += kp;
	}
}


int main(void)
{
	InitPeripherals();     
	SDBG_RegisterSmallCmd('s', Callback_s);
	//SDBG_RegisterSmallCmd('e', Callback_e);
    
	//MUART_Printf("\nCAMERA Start!\n");
	SDBG_Printf("\nCAMERA Start!\n");
	while (1)
		DoMainLoop();   
	return 0;
}
