#ifndef TIMER_H_
#define TIMER_H_

#include "jdp.h"

void TIMER_PIT0_ISR(void);
void TIMER_Init(void);
void TIMER_DelayMs(uint32_t msec);
uint32_t TIMER_GetRuntime(void);

#endif /* BASETIMER_H_ */
