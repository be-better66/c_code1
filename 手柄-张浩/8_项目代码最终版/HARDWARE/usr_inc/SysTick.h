#ifndef __SYSTICK_H
#define __SYSTICK_H
#include "stm32f4xx.h"  

/* 延时模块。
   注意：SysTick 定时器已经被 FreeRTOS 用作系统节拍，所以这里不能再配置 SysTick。
   delay_us 用 DWT 周期计数器做忙等；delay_ms 直接用 FreeRTOS 的 vTaskDelay。 */

void Delay_Init(void);
void delay_us(u32 nus);

#endif
