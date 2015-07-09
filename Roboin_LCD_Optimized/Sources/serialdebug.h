/*
 * serialdebug.h
 *
 *  Created on: Jan 2, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다. 
 */

#ifndef SERIALDEBUG_H_
#define SERIALDEBUG_H_

#include "jdp.h"

#define SDBG_ASCII_CR 	'\r'  // 13
#define SDBG_ASCII_LF	'\n'  // 10
#define SDBG_ASCII_BS	'\b'  // 8

// 한 문자 명령어
#define SDBG_CCMD_START	'A'
#define SDBG_CCMD_END		'Z'
#define SDBG_CCMD_NUM (SDBG_CCMD_END-SDBG_CCMD_START+1)

// \n으로 끝나는 한줄 명령어
#define SDBG_SCMD_START	'a'
#define SDBG_SCMD_END		'z'
#define SDBG_SCMD_NUM		(SDBG_SCMD_END-SDBG_SCMD_START+1)

#define SDBG_SCMD_BUF_SIZE		120
#define SDBG_PRINTF_BUF_SIZE	160

#define SDBG_TX_QUE_SIZE	300


//- FUNCTION ----------------------------------------------
void SDBG_RX0_ISR(void);
void SDBG_TX0_ISR(void);
void SDBG_Init(void);
void SDBG_RegisterSmallCmd(char scmd, void (*pCallback)(char*));
void SDBG_RegisterCapitalCmd(char ccmd, void (*pCallback)(void));
int16_t SDBG_IsTxQueFull(void);
void SDBG_PutCh(char ch);
void SDBG_PutStr(char* pstr);
void SDBG_Printf(const char* format, ...);
uint16_t SDBG_IsEvents(void);
void SDBG_PrintNoRegCmd(void);
void SDBG_ExecuteCallback(void);

#endif /* SERIALDEBUG_H_ */
