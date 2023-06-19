#ifndef _SysTick_H
#define _SysTick_H

#include "system.h"

void SysTick_Init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#define Correct_DelayUs delay_us
#define Correct_DelayMs	delay_ms

#endif
