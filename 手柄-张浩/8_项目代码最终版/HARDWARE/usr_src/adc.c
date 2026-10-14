#include "adc.h"
#include "stm32f4xx.h" 
__IO uint16_t aADCDualConvertedValue[6];



/*************************************
硬件说明
PA2 -- ADC123_IN2（摇杆1的x轴）
PA3 -- ADC123_IN3（摇杆1的y轴）
PA4 -- ADC12_IN4   （按键1）
PA6 -- ADC12_IN6    （按键2）
PB0 -- ADC12_IN8   （摇杆2的x轴)
PB1 -- ADC12_IN9   （摇杆2的y轴）
选用ADC1

	3.3v  	4095
sw1	0v	0
sw2	0.47v	580-600
sw3	1v	1300-1350
sw4	1.64v	2030-2060
sw5	2.4v	2970-3020

*************************************/

void ADC1_Init(void)
{
	GPIO_InitTypeDef  	GPIO_InitStructure;
	ADC_CommonInitTypeDef 	ADC_CommonInitStructure;
	ADC_InitTypeDef       	ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//复位ADC寄存器
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	


	ADC_CommonInitStructure.ADC_Mode 			= ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay 		= ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInitStructure.ADC_DMAAccessMode 		= ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_Prescaler 			= ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution 		= ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode 		= ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode 	= ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge 	= ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign 		= ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion 		= 6;

	ADC_Init(ADC1, &ADC_InitStructure);

	// 配置 ADC 通道转换顺序和采样时间周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_15Cycles );		
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, ADC_SampleTime_28Cycles );			    
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_15Cycles );		
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 4, ADC_SampleTime_28Cycles );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_28Cycles );		
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 6, ADC_SampleTime_28Cycles );		

	// 使能 DMA 请求 after last transfer (Single-ADC mode)
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE);

}

void DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_InitStructure.DMA_Channel 			= DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&(ADC1->DR);	//数据源在ADC1->DR中 （外设地址：adc1数据寄存器）
	DMA_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)aADCDualConvertedValue;//数据目的地在aADCDualConvertedValue数组
	DMA_InitStructure.DMA_DIR 			= DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize 		= 6;    				//与通道数设置一致
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_MemoryInc 		= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_HalfWord;	// 外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_MemoryDataSize	 	= DMA_PeripheralDataSize_HalfWord;	// 存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_Mode 			= DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority 			= DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode 			= DMA_FIFOMode_Disable;            // 禁止 DMA FIFO ，使用直连模式
	// FIFO 大小， FIFO 模式禁止时，这个不用配置
	DMA_InitStructure.DMA_FIFOThreshold 		= DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst 		= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
}
