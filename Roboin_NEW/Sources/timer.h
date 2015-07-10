#ifndef TIMER_H_
#define TIMER_H_

#include "jdp.h"

void TIMER_PIT0_ISR (void);
void TIMER_Init(void);
void TIMER_DelayMs(uint32_t msec);
uint32_t TIMER_GetRuntime(void);
uint32_t TIMER_GetRuntime_Micros(void);
void TIMER_SetRuntime(uint32_t msec);

#define PIT0_TIMER_GAP 20

#endif /* BASETIMER_H_ */
