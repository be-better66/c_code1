#include "timer.h"
#include "stm32f4xx.h" 
/*********************************************
功能说明：
TIM3 -- APB1 -- 16位定时器(65535)
TIM3时钟频率： APB1*2 = 42*2 = 84MHZ
**********************************************/


void Tim3_Init(void)
{
	NVIC_InitTypeDef  NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	

	TIM_TimeBaseInitStruct.TIM_Prescaler		= 8400-1;   			//8400分频  84000 000HZ/8400 = 10000HZ
	TIM_TimeBaseInitStruct.TIM_Period		= 800-1;   			//计800个数，用时约80ms
	TIM_TimeBaseInitStruct.TIM_CounterMode		= TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision	= TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);	
	
	NVIC_InitStruct.NVIC_IRQChannel				= TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 2;
 	NVIC_InitStruct.NVIC_IRQChannelSubPriority		= 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd			= ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
}
