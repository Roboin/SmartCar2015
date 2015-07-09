/*
 * serialdebug.c
 *
 *  Created on: Jan 2, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다. 
 */

#include <stdio.h>
#include <stdarg.h>
#include "serialdebug.h"
#include "ngv_dmu.h"
#include "rappid_utils.h"


void (*sdbg_pSCmdCallback[SDBG_SCMD_NUM])(char *);
void (*sdbg_pCCmdCallback[SDBG_CCMD_NUM])(void);


#define SDBG_RX_SCMD 0x1
#define SDBG_RX_HCMD 0x2


vint8_t sdbg_RxFlag;
volatile char sdbg_RxCCmd;
volatile char  sdbg_RxSCmd[SDBG_SCMD_BUF_SIZE];
vint16_t sdbg_RxSCmdPtr;
vint16_t sdbg_TxQueHeadPtr, sdbg_TxQueTailPtr;
volatile char  sdbg_TxQue[SDBG_TX_QUE_SIZE];


//- UART RX0 ISR
void SDBG_RX0_ISR (void)
{
	char rxd;
	
	rxd = (char)LINFLEX_0.BDRM.R;

	//- Capital Command: 한 문자 명령어
	if ((rxd>=SDBG_CCMD_START) && (rxd<=SDBG_CCMD_END)) {
		if (!(sdbg_RxFlag & SDBG_RX_HCMD)) {
			sdbg_RxCCmd = rxd;
			sdbg_RxFlag |= SDBG_RX_HCMD;
		}
	}
	// Small Command: 한 줄 명령어
	else {
		if (!(sdbg_RxFlag & SDBG_RX_SCMD)){
			if (rxd==SDBG_ASCII_CR) {
				sdbg_RxSCmd[sdbg_RxSCmdPtr] = '\0';
				sdbg_RxFlag |= SDBG_RX_SCMD;
				sdbg_RxSCmdPtr++;
			} else if (rxd==SDBG_ASCII_BS) {
				if (sdbg_RxSCmdPtr>0)
					sdbg_RxSCmdPtr--;
			} else {
				sdbg_RxSCmd[sdbg_RxSCmdPtr] = rxd;
				if (sdbg_RxSCmdPtr < (SDBG_SCMD_BUF_SIZE-1)) {
					sdbg_RxSCmdPtr++;
				}
			}
		}
	}
	
    LINFLEX_0.LINSR.R = 0xF23F;
    LINFLEX_0.UARTSR.R = 0x4;	// DRF
}


//- UART TX0 ISR
void SDBG_TX0_ISR (void)
{
    LINFLEX_0.LINSR.R = 0xF23F;
    LINFLEX_0.UARTSR.R = 0x2;	// DTF

    if (sdbg_TxQueHeadPtr!=sdbg_TxQueTailPtr) {
		LINFLEX_0.BDRL.R = sdbg_TxQue[sdbg_TxQueTailPtr];
		sdbg_TxQueTailPtr = (sdbg_TxQueTailPtr+1)%SDBG_TX_QUE_SIZE;
	}
	else {
		LINFLEX_0.LINIER.B.DTIE = 0;
	}	
}


//- SDBG 초기화
void SDBG_Init(void)
{
	int16_t i;

	sdbg_RxFlag = 0;
	sdbg_RxSCmdPtr = 0;
	sdbg_TxQueHeadPtr = 0;
	sdbg_TxQueTailPtr = 0;

	for (i=0; i<SDBG_SCMD_NUM; i++) {
		sdbg_pSCmdCallback[i] = NULL;
	}
	
	for (i=0; i<SDBG_CCMD_NUM; i++) {
		sdbg_pCCmdCallback[i] = NULL;
	}
	
	// RX Interrupt 
	LINFLEX_0.LINIER.B.DRIE = 1;
}


//- small command callback 함수 등록
void SDBG_RegisterSmallCmd(char scmd, void (*pCallback)(char*))
{
	if ((scmd<SDBG_SCMD_START) || (scmd>SDBG_SCMD_END)) {
		return;
	}
	sdbg_pSCmdCallback[scmd-SDBG_SCMD_START] = pCallback;
}


//- capital command callback 함수 등록
void SDBG_RegisterCapitalCmd(char ccmd, void (*pCallback)(void))
{
	if ((ccmd<SDBG_CCMD_START) || (ccmd>SDBG_CCMD_END)) {
		return;
	}
	sdbg_pCCmdCallback[ccmd-SDBG_CCMD_START] = pCallback;
}


//- Tx Queue가 다 차면 NGV_DMU_TRUE 리턴, 그렇지 않으면 NGV_DMU_FALSE 리턴
int16_t SDBG_IsTxQueFull(void)
{
	int16_t size;
	
	DisableExternalInterrupts();
	size = (sdbg_TxQueHeadPtr-sdbg_TxQueTailPtr+SDBG_TX_QUE_SIZE)%SDBG_TX_QUE_SIZE;
	EnableExternalInterrupts();
	
	if (size>=(SDBG_TX_QUE_SIZE-2)) {
		return NGV_DMU_TRUE;
	}
	return NGV_DMU_FALSE;
}


//- Tx Queue에 Ch 넣음. 단, Tx Queue가 차 있으면 빌 때까지 기다림
void SDBG_PutCh(char ch)
{
	vint32_t cnt=0;
	
	while (SDBG_IsTxQueFull()) {
		if (++cnt>30000)
			break;
	}
	if (ch == SDBG_ASCII_LF)
		SDBG_PutCh(SDBG_ASCII_CR);

	DisableExternalInterrupts();

	if (LINFLEX_0.LINIER.B.DTIE == 0) {
	    LINFLEX_0.UARTSR.R = 0x2;	// DTF
		LINFLEX_0.LINIER.B.DTIE = 1;
		LINFLEX_0.BDRL.R = ch;
		LINFLEX_0.LINSR.R = 0xF23F;
	}
	else {
		sdbg_TxQue[sdbg_TxQueHeadPtr] = ch;
		sdbg_TxQueHeadPtr = (sdbg_TxQueHeadPtr+1)%SDBG_TX_QUE_SIZE;
	}
	EnableExternalInterrupts();
}


//- Tx Queue에 pstr이 가르키는 문자열 넣음
void SDBG_PutStr(char* pstr)
{
	while (*pstr!='\0') {
		SDBG_PutCh(*pstr++);
	}
}


//- Tx queue에 printf 명령어로 문자열 대입, 출력할 문자가 SDBG_PRINTF_BUF_SIZE가 넘으면 책임 못짐!
void SDBG_Printf(const char* format, ...)
{
	char buf[SDBG_PRINTF_BUF_SIZE];
	
	va_list args;
	va_start(args, format);
	vsprintf((char*)buf, format, args);
	va_end(args);
	
	SDBG_PutStr(buf);
}


//- Event(한 문자 명령어 또는 한 줄 명령어)가 있으면 NGV_DMU_TRUE 리턴, 그렇지 않으면 FASLE 리턴
uint16_t SDBG_IsEvents(void)
{
	if ((sdbg_RxFlag & SDBG_RX_SCMD) || (sdbg_RxFlag & SDBG_RX_HCMD)) {
		return NGV_DMU_TRUE;
	}
	return NGV_DMU_FALSE;
}


//- 등록되어 있지 않은 command가 있을 때 출력
void SDBG_PrintNoRegCmd(void)
{
	SDBG_Printf("NGV_DMU> \n");
}


//- SDBG_IsEvents()가 NGV_DMU_TRUE 일 때 Event에 해당하는 함수 실행
void SDBG_ExecuteCallback(void)
{
	int16_t i;
	char cmd;

	// 한 문자 명령어에 대한 callback 함수 호출
	if (sdbg_RxFlag & SDBG_RX_HCMD) {
		if ((sdbg_RxCCmd>=SDBG_CCMD_START) && (sdbg_RxCCmd<=SDBG_CCMD_END)) {
			if (sdbg_pCCmdCallback[sdbg_RxCCmd-SDBG_CCMD_START]!=NULL) {
				sdbg_pCCmdCallback[sdbg_RxCCmd-SDBG_CCMD_START]();
			}
			else {
				SDBG_PrintNoRegCmd();
			}
		}
		else {
			SDBG_PrintNoRegCmd();
		}
		sdbg_RxFlag &= ~SDBG_RX_HCMD;
	}

	// 한 줄 명령어에 대한 callback 함수 호출
	if (sdbg_RxFlag & SDBG_RX_SCMD) {
		for (i=0; i<sdbg_RxSCmdPtr; i++) {
			if ((sdbg_RxSCmd[i] == SDBG_ASCII_LF) || sdbg_RxSCmd[i] == ' ') {
				continue;
			}
			break; 
		}
		if (i!=sdbg_RxSCmdPtr) {
			cmd = sdbg_RxSCmd[i];
			if ((cmd>=SDBG_SCMD_START) && (cmd<=SDBG_SCMD_END)) {
				if (sdbg_pSCmdCallback[cmd-SDBG_SCMD_START]!=NULL) {
					sdbg_pSCmdCallback[cmd-SDBG_SCMD_START]((char*)&sdbg_RxSCmd[i+1]);
				}
				else {
					SDBG_PrintNoRegCmd();
				}
			} else {
				SDBG_PrintNoRegCmd();
			}
		}
		else {
			SDBG_PrintNoRegCmd();
		}
		sdbg_RxSCmdPtr = 0;
		sdbg_RxFlag &= ~SDBG_RX_SCMD;
	}
}


