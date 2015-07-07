/*
 * youruart.h
 *
 *  Created on: Jan 6, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다. 
 */

#ifndef YOURUART_H_
#define YOURUART_H_

#include "jdp.h"

#define YUART_ASCII_CR 	'\r'  // 13
#define YUART_ASCII_LF	'\n'  // 10
#define YUART_ASCII_BS	'\b'  // 8

// 한 문자 명령어
#define YUART_CCMD_START	'A'
#define YUART_CCMD_END		'Z'
#define YUART_CCMD_NUM (YUART_CCMD_END-YUART_CCMD_START+1)

// \n으로 끝나는 한줄 명령어
#define YUART_SCMD_START	'a'
#define YUART_SCMD_END		'z'
#define YUART_SCMD_NUM		(YUART_SCMD_END-YUART_SCMD_START+1)

#define YUART_SCMD_BUF_SIZE		120
#define YUART_PRINTF_BUF_SIZE	160

#define YUART_TX_QUE_SIZE	300


//- FUNCTION ----------------------------------------------
void YUART_RX2_ISR(void);
void YUART_TX2_ISR(void);
void YUART_Init(void);
void YUART_RegisterSmallCmd(char scmd, void (*pCallback)(char*));
void YUART_RegisterCapitalCmd(char ccmd, void (*pCallback)(void));
int16_t YUART_IsTxQueFull(void);
void YUART_PutCh(char ch);
void YUART_PutStr(char* pstr);
void YUART_Printf(const char* format, ...);
uint16_t YUART_IsEvents(void);
void YUART_PrintNoRegCmd(void);
void YUART_ExecuteCallback(void);

#endif /* YOURUART_H_ */
