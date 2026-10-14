#include "stm32f4xx.h"
#include "exti.h"
#include "timer.h"
#include "SysTick.h"
#include "usart.h"
#include "stdio.h"
#include "adc.h"
#include "string.h"
#include "stdlib.h"

/* FreeRTOS 头文件：FreeRTOS.h 必须放在最前面 */
#include "FreeRTOS.h"
#include "task.h"

/* 任务栈大小，单位是“字”（1 字 = 4 字节） */
#define UART_TASK_STACK   128
#define ADC_TASK_STACK    128

/* 任务优先级，数字越大优先级越高（0 是空闲任务） */
#define UART_TASK_PRIO    3
#define ADC_TASK_PRIO     2

/* ADC 的原始值，由 DMA2 通道0 源源不断地写进来 */
extern __IO uint16_t aADCDualConvertedValue[6];

/* 用来比较的上一帧数据：变化超过阈值才发送 */
unsigned int temp_adcVal01 = 2048, temp_adcVal02 = 2048, temp_adcVal03 = 2048;
unsigned int temp_adcVal04 = 2048, temp_adcVal05 = 4095, temp_adcVal06 = 4095;
char msgstr[64];

/* 串口收到的一帧数据，以 ':' 作为结束符 */
u8 buffer[32] = {0};
u8 rx_buffer[32] = {0};
u8 count = 0;
u8 rx_i, rx_flag = 0;  //rx_flag 为 1 表示收到了一帧完整数据

/* 两个任务的入口函数声明 */
static void UART_Task(void *pvParameters);
static void ADC_Task(void *pvParameters);
static void USART3_send(char *str);

/* 主函数：只做硬件初始化 + 建任务 + 启动调度器 */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_Init();
	USART1_Init_Config();
	USART3_Init_Config(9600);
	DMA_Config();
	ADC1_Init();
	EXTI_Init_Key();

	/* 原来的写法是 Tim3_Init() 每 80ms 中断一次来采集 ADC。
	   用了 FreeRTOS 以后不需要定时器中断了，直接在任务里用 vTaskDelay 定时，
	   所以这里不再调用 Tim3_Init()。*/

	/* 开始 ADC 转换，软件启动 */
	ADC_SoftwareStartConv(ADC1);

	/* 创建两个任务 */
	xTaskCreate(UART_Task, "uart", UART_TASK_STACK, NULL, UART_TASK_PRIO, NULL);
	xTaskCreate(ADC_Task,  "adc",  ADC_TASK_STACK,  NULL, ADC_TASK_PRIO,  NULL);

	/* 启动调度器，开始任务调度，这个函数不会返回 */
	vTaskStartScheduler();

	while(1);
}

/* 任务1：串口接收。
   每一帧数据由 USART1/USART3 中断接收，收到 ':' 表示一帧结束并把 rx_flag 置 1。
   本任务每 20ms 来看一次标志位，有数据就打印出来。 */
static void UART_Task(void *pvParameters)
{
	char local[32];

	for(;;)
	{
		if(rx_flag == 1)
		{
			/* 关中断复制，防止复制过程中串口中断又改写了 rx_buffer */
			taskENTER_CRITICAL();
			memcpy(local, rx_buffer, sizeof(local));
			memset(rx_buffer, 0, sizeof(rx_buffer));
			rx_flag = 0;
			taskEXIT_CRITICAL();

			printf("rx_buffer:%s\r\n", local);
		}

		/* 让出 CPU，20ms 后再来 */
		vTaskDelay(20);
	}
}

/* 任务2：采集 ADC 并上报。
   每 80ms 读一次 DMA 送来的 6 路 ADC 值，
   和上一帧比较，变化超过 248 就把电压值通过 USART1 和 USART3 发出去。 */
static void ADC_Task(void *pvParameters)
{
	unsigned int adcVal01, adcVal02, adcVal03, adcVal04, adcVal05, adcVal06;

	for(;;)
	{
		/* 1.X1轴电压 2.Y1轴电压 3.X2轴电压 4.Y2轴电压 5.左按键电压 6.右按键电压 */
		adcVal01 = aADCDualConvertedValue[0];
		adcVal02 = aADCDualConvertedValue[1];
		adcVal03 = aADCDualConvertedValue[2];
		adcVal04 = aADCDualConvertedValue[3];
		adcVal05 = aADCDualConvertedValue[4];
		adcVal06 = aADCDualConvertedValue[5];

		if(abs((int)temp_adcVal01-(int)adcVal01)>248 ||
		   abs((int)temp_adcVal02-(int)adcVal02)>248 ||
		   abs((int)temp_adcVal03-(int)adcVal03)>248 ||
		   abs((int)temp_adcVal04-(int)adcVal04)>248 ||
		   abs((int)temp_adcVal05-(int)adcVal05)>248 ||
		   abs((int)temp_adcVal06-(int)adcVal06)>248)
		{
			sprintf(msgstr,"%.1f:%.1f:%.1f:%.1f:%.1f:%.1f\n",
			        (adcVal01/4095.0)*3.3,(adcVal02/4095.0)*3.3,
			        (adcVal03/4095.0)*3.3,(adcVal04/4095.0)*3.3,
			        (adcVal05/4095.0)*3.3,(adcVal06/4095.0)*3.3);
			Usart_SendString(USART1, msgstr);
			USART3_send(msgstr);
			printf("\r\n");
		}

		/* 每 80ms 采集一次 */
		vTaskDelay(80);
	}
}

static void USART3_send(char *str)
{
	while(*str)
	{
		USART_SendData(USART3, *str);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET);
		str++;
	}
}

/* FreeRTOS 钩子函数：某个任务的栈用超了就会进到这里。
   停在死循环里，方便调试时发现（说明要把任务的栈开大一点）。 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
	(void)xTask;
	(void)pcTaskName;

	taskDISABLE_INTERRUPTS();
	while(1);
}
