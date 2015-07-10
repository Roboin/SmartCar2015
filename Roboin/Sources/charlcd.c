/*
 * charlcd.c
 *
 *  Created on: Jan 5, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.        
 */

#include <stdio.h>
#include <stdarg.h>
#include "ngv_dmu.h"
#include "charlcd.h"
#include "jdp.h"
#include "gpio.h"
#include "rappid_utils.h"


uint8_t clcd_Font[CLCD_FONTNUM*8] =
{
    0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, // User Font 0
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, // User Font 1
    0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, // User Font 2
    0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
    0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f,
    0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00,
    0x1f, 0x04, 0x1b, 0x00, 0x1f, 0x04, 0x10, 0x1f,
    0x15, 0x0a, 0x15, 0x0a, 0x15, 0x0a, 0x15, 0x0a  // User Font 7
};

vint16_t clcd_QueHeadPtr, clcd_QueTailPtr;
vuint16_t  clcd_Que[CLCD_QUE_SIZE];

vint32_t clcd_cnt;


void CLCD_PIT1_ISR (void)
{
	if (clcd_QueHeadPtr!=clcd_QueTailPtr) {
		if ((CLCD_ReadBusyFlag()&CLCD_BUSY)==0) {
			if (clcd_Que[clcd_QueTailPtr]&CLCD_INST_MASK) {
				CLCD_WriteInst((uint8_t)(clcd_Que[clcd_QueTailPtr] & 0xff));  
			} 
			else {
				CLCD_WriteCh((uint8_t)clcd_Que[clcd_QueTailPtr]);       
			}
			clcd_QueTailPtr = (clcd_QueTailPtr+1)%CLCD_QUE_SIZE;
		}
	}
	else  {
		PIT.CH[1].TCTRL.B.TIE = 0;
	}
	
	PIT.CH[1].TFLG.R = 0x00000001;
}


//- 약 us 단위로 시간을 지연시키는 함수
void CLCD_DelayUs(uint32_t us)
{
	vuint32_t cnt;
#if NGV_DMU_SYSCLK == SYSCLK_IRC_16MHZ
	cnt = us << 2;
#elif NGV_DMU_SYSCLK == SYSCLK_PLL_32MHZ
	cnt = us << 3;
#elif NGV_DMU_SYSCLK == SYSCLK_PLL_64MHZ
	cnt = us << 4;
#endif
	
	while (cnt--) {
	}
}
    

void CLCD_Init(void)
{
	uint16_t fn, i, offs;

	GPIO_Set(GPIO_PC5, 0);	// E = 0

	PIT.CH[1].TCTRL.B.TIE = 1;	// Enable PIT Interrupt
	
	CLCD_DelayUs(50000);		// 50 msec 시간 지연. 왜?

	//- CLCD 초기화
	CLCD_PutInstDir(0x28);       // 4-bit interface, 2-line display
	CLCD_PutInstDir(0x28);       // 4-bit interface, 2-line display
	CLCD_PutInstDir(0x06);       // Incremental cursor movement
	CLCD_PutInstDir(0x0c);       // Display ON, cursor OFF and blink OFF
	CLCD_PutInstDir(CLCD_CLEAR); // Clear LCD screen
	CLCD_DelayUs(2000);

	for (fn=0; fn<CLCD_FONTNUM; fn++) {
		for (i=0; i<8; i++) {
			offs = fn*8+i;
			CLCD_PutInstDir((uint8_t)(0x40 | offs));
			CLCD_PutChDir(clcd_Font[offs]);
		}
	}
}


void CLCD_WriteCh(uint8_t data)
{
	GPIO_Set(GPIO_PC3, 1); 	// RS = 1;
	GPIO_Set(GPIO_PC4, 0);	// RW = 0;
	GPIO_Set(GPIO_PC5, 0);	// E = 0;
	CLCD_WriteCore(data);    
}


void CLCD_WriteInst(uint8_t data)
{
	GPIO_Set(GPIO_PC3, 0); 	// RS = 0;
	GPIO_Set(GPIO_PC4, 0);	// RW = 0;
	GPIO_Set(GPIO_PC5, 0);	// E = 0;
	CLCD_WriteCore(data);    
}


void CLCD_WriteCore(uint8_t data)
{
	GPIO_Set(GPIO_PC12, (data>>4) & 0x1); 	// D4 = ;
	GPIO_Set(GPIO_PC13, (data>>5) & 0x1); 	// D5 = ;
	GPIO_Set(GPIO_PC14, (data>>6) & 0x1); 	// D6 = ;
	GPIO_Set(GPIO_PC15, (data>>7) & 0x1); 	// D7 = ;
	
	GPIO_Set(GPIO_PC5, 1);	// E = 1;
	CLCD_DelayUs(1); 		// > 600 nano seconds
	GPIO_Set(GPIO_PC5, 0);	// E = 0;

	GPIO_Set(GPIO_PC12, (data>>0) & 0x1); 	// D4 = ;
	GPIO_Set(GPIO_PC13, (data>>1) & 0x1); 	// D5 = ;
	GPIO_Set(GPIO_PC14, (data>>2) & 0x1); 	// D6 = ;
	GPIO_Set(GPIO_PC15, (data>>3) & 0x1); 	// D7 = ;

	GPIO_Set(GPIO_PC5, 1);	// E = 1;
	CLCD_DelayUs(1); 		// > 600 nano seconds
	GPIO_Set(GPIO_PC5, 0);	// E = 0;

}


uint8_t CLCD_ReadBusyFlag(void)
{
	uint8_t data=0;

	SIU.PCR[GPIO_PC15].R = 0x0101;
	SIU.PCR[GPIO_PC14].R = 0x0101;
	SIU.PCR[GPIO_PC13].R = 0x0101;
	SIU.PCR[GPIO_PC12].R = 0x0101;
	
	GPIO_Set(GPIO_PC3, 0); 	// RS = 0;
	GPIO_Set(GPIO_PC4, 1);	// RW = 1;
	GPIO_Set(GPIO_PC5, 0);	// E = 0;
	

	GPIO_Set(GPIO_PC5, 1);	// E = 1;
	CLCD_DelayUs(1); 		// > 320 nano seconds
	data = GPIO_Get(GPIO_PC15) ? 0x80 : 0;
	GPIO_Set(GPIO_PC5, 0);	// E = 0;
	
	CLCD_DelayUs(1); 		// > 600 nano seconds

	GPIO_Set(GPIO_PC5, 1);	// E = 1;
	CLCD_DelayUs(1); 		// > 600 nano seconds
	GPIO_Set(GPIO_PC5, 0);	// E = 0;
	
	SIU.PCR[GPIO_PC15].R = 0x0201;
	SIU.PCR[GPIO_PC14].R = 0x0201;
	SIU.PCR[GPIO_PC13].R = 0x0201;
	SIU.PCR[GPIO_PC12].R = 0x0201;
	CLCD_DelayUs(10);

	return data;
}


void CLCD_PutChDir(char ch)
{
	CLCD_WriteCh(ch);
	CLCD_DelayUs(100);
}


void CLCD_PutInstDir(uint8_t data)
{
	CLCD_WriteInst(data);
	CLCD_DelayUs(100);
}


uint8_t CLCD_IsQueFull(void)
{
	int16_t size;
	
	DisableExternalInterrupts();
	size = (clcd_QueHeadPtr-clcd_QueTailPtr+CLCD_QUE_SIZE)%CLCD_QUE_SIZE;
	EnableExternalInterrupts();
	
	if (size>=(CLCD_QUE_SIZE-2)) {
		return NGV_DMU_TRUE;
	}
	return NGV_DMU_FALSE;
}


void CLCD_PutCh(char ch)
{
	vint32_t cnt=0;
	
	while (CLCD_IsQueFull()) {
		if (++cnt>30000)
			break;	
	}
	clcd_Que[clcd_QueHeadPtr] = (uint16_t)ch & 0x00ff;
	clcd_QueHeadPtr = (clcd_QueHeadPtr+1)%CLCD_QUE_SIZE;
	
	PIT.CH[1].TCTRL.B.TIE = 1;  
}


void CLCD_PutStr(char* pstr)
{
	while (*pstr!='\0') {
		CLCD_PutCh(*pstr++);
	}
}


void CLCD_PutInst(uint8_t data)
{
	while (CLCD_IsQueFull()) {
	}
	clcd_Que[clcd_QueHeadPtr] = (uint16_t)data | CLCD_INST_MASK;
	clcd_QueHeadPtr = (clcd_QueHeadPtr+1)%CLCD_QUE_SIZE;
	
	PIT.CH[1].TCTRL.B.TIE = 1;  
}


void CLCD_GotoXy(uint8_t x, uint8_t y)
{
	switch (y) {
	case 0:
		CLCD_PutInst(0x80+x);
	break;
	case 1:
		CLCD_PutInst(0xc0+x);
	break;
	case 2:
		CLCD_PutInst(0x94+x);
	break;
	case 3:
		CLCD_PutInst(0xd4+x);
	break;
	}
}


void CLCD_Printf(const char *format, ...)
{
	char buf[CLCD_PRINTF_BUF_SIZE];
	va_list args;
	
	va_start(args, format);
	vsprintf((char*)buf, format, args);
	va_end(args);
	
	CLCD_PutStr(buf);
}


void CLCD_PrintfXy(uint8_t x, uint8_t y, const char *format, ...)
{
	char buf[CLCD_PRINTF_BUF_SIZE];
	va_list args;
	
	CLCD_GotoXy(x, y);

	va_start(args, format);
	vsprintf((char*)buf, format, args);
	va_end(args);
	
	CLCD_PutStr(buf);
	
}

