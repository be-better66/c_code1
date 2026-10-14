
#include "usart.h"
#include "stm32f4xx.h" 
#include <stdio.h>



//pa9--》tx
//pa10--》rx
void USART1_Init_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//1.使能RX 和TX 引脚GPIO(PA9 PA10)时钟和USART1 时钟；
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//PA9 PA10 GPIO引脚初始化，配置成复用模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//2.初始化GPIO，并将PA9和PA10 复用到USART 上；
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//3.配置USART参数
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART1,&USART_InitStructure);
	
	//4.配置中断控制器并使能USART接收中断；
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART1,ENABLE);
	
}

//pb10--》tx
//pb11--》rx
void USART3_Init_Config(uint32_t baud)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//1.使能RX 和TX 引脚GPIO(PB10 PB11)时钟和USART3 时钟；
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//PB10 PB11 GPIO引脚初始化，配置成复用模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//2.初始化GPIO，并将PB10和PB11 复用到USART 上；
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	
	//3.配置USART参数
	USART_InitStructure.USART_BaudRate=baud;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART3,&USART_InitStructure);
	
	//4.配置中断控制器并使能USART接收中断；
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART3,ENABLE);
	
}


//重定向c库函数printf到串口1，这样我们就可以使用printf函数打印输出信息到串口1了
int fputc(int ch, FILE *stream)
{
	USART_SendData(USART1,(uint16_t)ch);
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);

	return ch;
}

//发送一个字符
void Usart_SendByte(USART_TypeDef * pUSARTx , uint16_t ch)
{
	USART_SendData(pUSARTx,ch);
	
	//等待发送数据寄存器为空,TXE 位由硬件置 1，
	//它表示：USART_DR (数据寄存器)中可写入下一个数据，而不会覆盖前一个数据。
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);

}



//发送字符串
void Usart_SendString(USART_TypeDef * pUSARTx , char* str)
{
	uint16_t k = 0;
	
	do
	{
		Usart_SendByte(pUSARTx,*(str+k));
		k++;
	}while(*(str+k)!='\0');
	
}
