#include "SysTick.h"
#include "stm32f4xx.h"

/* SysTick 已经被 FreeRTOS 用作系统节拍（1ms 一次中断），
   所以本文件不再操作 SysTick 寄存器。
   delay_us 用内核自带的 DWT 周期计数器做忙等，用来做很短、很精确的延时。
   毫秒级延时请直接调用 FreeRTOS 的 vTaskDelay。 */

#define DWT_CTRL_CYCCNTENA      (1UL << 0)
#define CoreDebug_DEMCR_TRCENA  (1UL << 24)

static u32 s_cpu_mhz = 168;

void Delay_Init(void)
{
	/* 使能跟踪与调试单元 */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA;

	/* 清零并启动周期计数器 */
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA;

	/* 每微秒多少个计数：168MHz 主频就是 168 个 */
	s_cpu_mhz = SystemCoreClock / 1000000u;
	if(s_cpu_mhz == 0u)
	{
		s_cpu_mhz = 168u;
	}
}

/* 微秒级忙等延时：会一直占着 CPU，只在需要很短延时时使用 */
void delay_us(u32 nus)
{
	u32 start = DWT->CYCCNT;
	u32 ticks = nus * s_cpu_mhz;

	while((DWT->CYCCNT - start) < ticks);
}
