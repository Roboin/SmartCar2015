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

void GPIO_TEST(int onoff);

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

int16_t sangle = 0;

void DoMainLoop(void)
{
	//COUNT FOR TIMER
	static uint32_t t_c, t_old=(uint32_t)-1, cnt=0;
	
	//RUN CAMERA //with dip sw 1
	if(dipsw & 0x1)//high//sw1
		CAM_RUN2();
	else//low
		CAM_RUN();
	//threshhold
	if(dipsw & 0x2)//sw2
		cam_th = (FADC_Get(1)-800)*5;
	else
		cam_th = (CAM_MAX()+CAM_MIN())/2;//CAM_TRESHLOLD();
	//camera exposure time
	if(dipsw & 0x8)//sw4
		CAM_PulseWidth(50);
	else
		CAM_PulseWidth(100);
		
	//SERIAL COMMUNICATE
	if (SDBG_IsEvents())
		SDBG_ExecuteCallback();
	
	if (MUART_IsEvents())
		MUART_ExecuteCallback();
	
	//servo test in main
	sangle = (FADC_Get(1)-870)*2*37/156-37;
	MCTL_Servo(sangle);
			
	//DC test
	//if(DSW_GetTactAll() & 0x1){
		MCTL_DC_R((FADC_Get(1)-800)*5);
		MCTL_DC_L((FADC_Get(1)-800)*5);
	/*}
	else{
		MCTL_DC_R(1535);
		MCTL_DC_L(0);
	}*/
	
	//TIMER FOR EACH 100 ms	
	t_c = BTMR_GetRuntime()/500;
	if (t_c != t_old) {
		t_old = t_c;
		DLED_Toggle(DLED_LED1);
		dipsw = DSW_GetDipAll();
		//ENCODER TEST
		//SDBG_Printf("\n%u: %4d", cnt++, RENC_Get());
		
		//Servo test
		//MCTL_Servo(sangle);
		//sangle +=5;
		if(sangle > MCTL_SWHEEL_MAX)
		{
			sangle = -MCTL_SWHEEL_MAX;
		}
		//SDBG_Printf("\nServo Angle : %d",sangle);
		
		
		if(flag==0){
				flag = 1;
		}
	}
	
	if(flag){
		SDBG_Printf("\n%4d TH:%3d,%3d,%3d\t",t_c,cam_th,CAM_MAX(),CAM_MIN());
		for(i_serial = 0; i_serial < NUM_OF_PIXEL; i_serial++){
			/*if(i_serial >= NUM_OF_PIXEL){
				SDBG_Printf("\n%4d TH:%3d,%3d,%3d\t",t_c,cam_th,CAM_MAX(),CAM_MIN());
				//i_serial = 0;
				flag = 0;
			}*/
		//if( i_serial < NUM_OF_PIXEL ){
			if(dipsw & 0x4){//up
				if(CAM_DATA(i_serial) > cam_th){
					SDBG_Printf(".");
				}
				else{ //if(CAM_READ(i_serial) , cam_th)
					SDBG_Printf("!");
				}
			}
			else{//down
				SDBG_Printf("%3d ",CAM_DATA(i_serial));
			}
		}
		/*else{
			SDBG_Printf("%3d ",CAM_DATA(i_serial));
			i_serial++;
			if(i_serial >= NUM_OF_PIXEL){
				SDBG_Printf("\n%4d TH:%3d,%3d,%3d\t",t_c,cam_th,CAM_MAX(),CAM_MIN());
				i_serial = 0;
				flag = 0;
			}
		}*/
		/*for(i_serial = 0; i_serial < NUM_OF_PIXEL; i_serial++){
			if(dipsw & 0x4){//up
				if(CAM_DATA(i_serial) > cam_th){
					SDBG_Printf(".");
				}
				else{ //if(CAM_READ(i_serial) , cam_th)
					SDBG_Printf("!");
				}
			}
			else{//down
				SDBG_Printf("%3d ",CAM_DATA(i_serial));
			}
		}*/
		flag = 0;
	}
}

int main(void)
{
	InitPeripherals();     
    
	SDBG_RegisterSmallCmd('s', Callback_s);
	SDBG_RegisterSmallCmd('e', Callback_e);
	//MUART_Printf("\nCAMERA Start!\n");
	SDBG_Printf("\nCAMERA Start!\n");
	while (1)
		DoMainLoop();   
	return 0;
}

void GPIO_TEST(int onoff)
{
	GPIO_Set(GPIO_PA0, onoff);
	GPIO_Set(GPIO_PA1, onoff);
	GPIO_Set(GPIO_PA2, onoff);
	GPIO_Set(GPIO_PA3, onoff);
	GPIO_Set(GPIO_PA7, onoff);
	GPIO_Set(GPIO_PA8, onoff);
	GPIO_Set(GPIO_PA9, onoff);
	GPIO_Set(GPIO_PA10, onoff);
	GPIO_Set(GPIO_PA11, onoff);
	GPIO_Set(GPIO_PA12, onoff);
	GPIO_Set(GPIO_PA13, onoff);
	GPIO_Set(GPIO_PA14, onoff);
	GPIO_Set(GPIO_PA15, onoff);
	
	GPIO_Set(GPIO_PB0, onoff);
	GPIO_Set(GPIO_PB1, onoff);
	GPIO_Set(GPIO_PB6, onoff);
	GPIO_Set(GPIO_PB7, onoff);
	GPIO_Set(GPIO_PB8, onoff);
	GPIO_Set(GPIO_PB9, onoff);
	GPIO_Set(GPIO_PB10, onoff);
	GPIO_Set(GPIO_PB11, onoff);
	GPIO_Set(GPIO_PB12, onoff);
	GPIO_Set(GPIO_PB13, onoff);
	GPIO_Set(GPIO_PB14, onoff);
	GPIO_Set(GPIO_PB15, onoff);
	
	GPIO_Set(GPIO_PC8, onoff);
	GPIO_Set(GPIO_PC9, onoff);
	
	//GPIO_Set(GPIO_PD12, onoff);
	GPIO_Set(GPIO_PD13, onoff);
	GPIO_Set(GPIO_PD14, onoff);
	GPIO_Set(GPIO_PD15, onoff);
	
	//GPIO_Set(GPIO_PE6, onoff);
	//GPIO_Set(GPIO_PE7, onoff);
	GPIO_Set(GPIO_PE8, onoff);
	GPIO_Set(GPIO_PE9, onoff);
	GPIO_Set(GPIO_PE10, onoff);
	GPIO_Set(GPIO_PE11, onoff);
	GPIO_Set(GPIO_PE12, onoff);
	GPIO_Set(GPIO_PE13, onoff);
	GPIO_Set(GPIO_PE14, onoff);
	GPIO_Set(GPIO_PE15, onoff);
	
	//GPIO_Set(GPIO_PF0, onoff);
	//GPIO_Set(GPIO_PF1, onoff);
	//GPIO_Set(GPIO_PF2, onoff);
	//GPIO_Set(GPIO_PF3, onoff);
	GPIO_Set(GPIO_PF4, onoff);
	GPIO_Set(GPIO_PF5, onoff);
	GPIO_Set(GPIO_PF6, onoff);
	GPIO_Set(GPIO_PF7, onoff);
	GPIO_Set(GPIO_PF8, onoff);
	GPIO_Set(GPIO_PF9, onoff);
	GPIO_Set(GPIO_PF10, onoff);
	GPIO_Set(GPIO_PF11, onoff);
	GPIO_Set(GPIO_PF12, onoff);
	GPIO_Set(GPIO_PF13, onoff);
	GPIO_Set(GPIO_PF14, onoff);
	GPIO_Set(GPIO_PF15, onoff);
	
	GPIO_Set(GPIO_PG0, onoff);
	GPIO_Set(GPIO_PG1, onoff);
	GPIO_Set(GPIO_PG2, onoff);
	GPIO_Set(GPIO_PG6, onoff);
	GPIO_Set(GPIO_PG7, onoff);
	GPIO_Set(GPIO_PG8, onoff);
	GPIO_Set(GPIO_PG9, onoff);
	GPIO_Set(GPIO_PG14, onoff);
	GPIO_Set(GPIO_PG15, onoff);
	
	GPIO_Set(GPIO_PH4, onoff);
	GPIO_Set(GPIO_PH5, onoff);
	GPIO_Set(GPIO_PH6, onoff);
	GPIO_Set(GPIO_PH7, onoff);
}
