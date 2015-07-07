/*
 * charlcd.h
 *
 *  Created on: Jan 5, 2015
 *      Author: 동양미래대학교 정준
 *        Note: 2015년 NGV 장학생 외 무단 전재, 복제, 배포를 금합니다.        
 */

#ifndef CHARLCD_H_
#define CHARLCD_H_

#include "jdp.h"

//- MACRO -------------------------------------------------
#define CLCD_COLUMN     16
#define CLCD_CLEAR      0x1
#define CLCD_CHOME      0x2
#define CLCD_CURSOR_ON  0xa
#define CLCD_CURSOR_OFF 0x8
#define CLCD_BUSY       0x80

#define CLCD_FONTNUM 8

#define CLCD_QUE_SIZE  200
#define CLCD_INST_MASK 0x100

#define CLCD_PRINTF_BUF_SIZE 160

//- FUNCTION ----------------------------------------------
void CLCD_PIT1_ISR (void);

void CLCD_Init(void);
void CLCD_DelayUs(uint32_t us);

void CLCD_WriteCh(uint8_t data);
void CLCD_WriteInst(uint8_t data);
void CLCD_WriteCore(uint8_t data);
uint8_t CLCD_ReadBusyFlag(void);

void CLCD_PutChDir(char ch);
void CLCD_PutInstDir(uint8_t data);

uint8_t CLCD_IsQueFull(void);
void CLCD_PutCh(char ch);
void CLCD_PutStr(char* pstr);
void CLCD_PutInst(uint8_t data);
void CLCD_GotoXy(uint8_t x, uint8_t y);
void CLCD_Printf(const char *format, ...);
void CLCD_PrintfXy(uint8_t x, uint8_t y, const char *format, ...);


#endif /* CHARLCD_H_ */
