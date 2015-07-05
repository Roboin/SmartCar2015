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


void InitPeripherals(void);
void DoMainLoop(void);
void Callback_s(char* str);
void Callback_e(char* str);

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

void Callback_s(char* str)
{
	int angle;
	
	if (sscanf(str, "%d", &angle)<1) {
		return;
	}
	
	MCTL_Servo((int16_t)angle);
	SDBG_Printf("\n%Angle : d",angle);
}

void Callback_e(char* str)
{
	int speed;
	
	if (sscanf(str, "%d", &speed)<1) {
		return;
	}
	
	MCTL_Servo((int16_t)speed);
	SDBG_Printf("\nSpeed %d",speed);
}

int8_t flag = 1;
int16_t i_serial=0;
int16_t cam_th= 200;
uint16_t dipsw = 0;
int16_t blinking_LED = DLED_LED1;
int16_t sAngle = 0;

void DoMainLoop(void)
{
	//COUNT FOR TIMER
	static uint32_t t_current, t_old=(uint32_t)-1, t_test,cnt=0;
	
	//DIPSW - 1-Camera Run / 2-cam_th / 3-.! or %3d / 4-cam_pulsewidth(exposuretime)
	//dipsw = DSW_GetDipAll();
	//RUN CAMERA //with dip sw 1
	if(dipsw & 0b0001)//high//sw1 0x1
		CAM_RUN2();
	else//low
		CAM_RUN();
		
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
	MCTL_DC_R((FADC_Get(1)-800)*5);
	MCTL_DC_L((FADC_Get(1)-800)*5);
	/*}
	else{
		MCTL_DC_R(1535);
		MCTL_DC_L(0);
	}*/
	
	//TIMER FOR EACH N ms	
	t_current = BTMR_GetRuntime()/500;
	if (t_current != t_old) {
		t_test = CAM_MICRO_SEC_RETURN();
		SDBG_Printf("\n%u: %d", cnt++, t_test);
		t_old = t_current;
		DLED_Toggle(blinking_LED);
		
		//DIPSW - 1-Camera Run / 2-cam_th / 3-.! or %3d / 4-cam_pulsewidth(exposuretime)
		dipsw = DSW_GetDipAll();
		//CAM RUN mode 1 or 2
		if(dipsw & 0b0001){//sw1 //0x1
			CAM_RUN_MODE_SETUP(2);//up
			blinking_LED= DLED_LED2;
		}
		else{
			CAM_RUN_MODE_SETUP(1);//down
			blinking_LED= DLED_LED1;
		}
		//Threshold
		if(dipsw & 0b0010)//sw2 //0x2
			cam_th = (FADC_Get(1)-800)*5;
		else
			cam_th = (CAM_MAX()+CAM_MIN())/2;//CAM_TRESHLOLD();
		//camera exposure time
		if(dipsw & 0b1000)//sw4//0x8
			CAM_PulseWidth(100);//up
		else//down
			CAM_PulseWidth(50);
		
		//ENCODER TEST
		//SDBG_Printf("\n%u: %4d", cnt++, RENC_Get());
		
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
		for(i_serial = 0; i_serial < NUM_OF_PIXEL; i_serial++){
			if(dipsw & 0b0100){//up//0x04
				if(CAM_DATA(i_serial) > cam_th){
					SDBG_Printf(".");
				}
				else{ //if(CAM_READ(i_serial) , cam_th)
					SDBG_Printf("!");
				}
			}
			else{//down
				//SDBG_Printf("%3d ",CAM_DATA(i_serial));
			}
		}
		flag = 0;
	}
}

int main(void)
{
	InitPeripherals();     
    
	//SDBG_RegisterSmallCmd('s', Callback_s);
	//SDBG_RegisterSmallCmd('e', Callback_e);
	//MUART_Printf("\nCAMERA Start!\n");
	SDBG_Printf("\nCAMERA Start!\n");
	while (1)
		DoMainLoop();   
	return 0;
}
