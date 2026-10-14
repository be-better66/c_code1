/*
 * FreeRTOSConfig.h  --  FreeRTOS 配置文件
 * 内核版本： FreeRTOS V202212.01
 * 芯片    ： STM32F407ZG (Cortex-M4F, 168MHz)
 *
 * 这个文件是 FreeRTOS 的“总开关”，内核编译时会来读这里的宏定义。
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
	#include <stdint.h>
	extern uint32_t SystemCoreClock;
#endif

/* ------------------- 调度器基础设置 ------------------- */
#define configUSE_PREEMPTION					1	/* 1=抢占式调度：高优先级任务可以打断低优先级任务 */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1	/* 1=用硬件指令找最高优先级任务，更快 */
#define configUSE_TICKLESS_IDLE					0	/* 不使用低功耗模式 */
#define configCPU_CLOCK_HZ						( SystemCoreClock )	/* CPU 主频 168MHz */
#define configTICK_RATE_HZ						( ( TickType_t ) 1000 )	/* 系统节拍 1000Hz，即 1 个节拍 = 1ms */
#define configMAX_PRIORITIES					( 5 )	/* 可用优先级个数 0~4 */
#define configMINIMAL_STACK_SIZE				( ( unsigned short ) 128 )	/* 空闲任务的栈，单位是“字” */
#define configMAX_TASK_NAME_LEN					( 12 )	/* 任务名字最长 12 个字符 */
#define configUSE_16_BIT_TICKS					0	/* 0=节拍计数器为 32 位 */
#define configIDLE_SHOULD_YIELD					1	/* 空闲任务把 CPU 让给同优先级任务 */
#define configUSE_TIME_SLICING					1	/* 同优先级任务按时间片轮流执行 */
#define configUSE_NEWLIB_REENTRANT				0

/* ------------------- 内存管理 ------------------- */
/* 使用 heap_4.c，堆大小 48KB（主 SRAM 一共 128KB） */
#define configTOTAL_HEAP_SIZE					( ( size_t ) ( 48 * 1024 ) )
#define configSUPPORT_STATIC_ALLOCATION			0	/* 不使用静态创建任务 */
#define configSUPPORT_DYNAMIC_ALLOCATION		1	/* 使用动态创建任务 xTaskCreate */
#define configUSE_MALLOC_FAILED_HOOK			0	/* 申请内存失败时不额外处理 */

/* ------------------- 钩子函数 ------------------- */
#define configUSE_IDLE_HOOK						0
#define configUSE_TICK_HOOK						0
#define configCHECK_FOR_STACK_OVERFLOW			1	/* 1=检查任务栈是否溢出 */
#define configUSE_DAEMON_TASK_STARTUP_HOOK		0

/* ------------------- 任务通知/队列/信号量 ------------------- */
#define configUSE_TASK_NOTIFICATIONS			1
#define configUSE_MUTEXES						1
#define configUSE_RECURSIVE_MUTEXES				1
#define configUSE_COUNTING_SEMAPHORES			1
#define configQUEUE_REGISTRY_SIZE				8
#define configUSE_QUEUE_SETS					0

/* ------------------- 软件定时器 ------------------- */
/* 本工程没有用软件定时器，所以关掉，也就不需要编译 timers.c */
#define configUSE_TIMERS						0
#define configTIMER_TASK_PRIORITY				( 2 )
#define configTIMER_QUEUE_LENGTH				10
#define configTIMER_TASK_STACK_DEPTH			( configMINIMAL_STACK_SIZE * 2 )

/* ------------------- 统计与调试 ------------------- */
#define configUSE_TRACE_FACILITY				0
#define configGENERATE_RUN_TIME_STATS			0
#define configUSE_STATS_FORMATTING_FUNCTIONS	0
#define configUSE_APPLICATION_TASK_TAG			0

/* ------------------- 可选 API 裁剪（1=编译进来） ------------------- */
#define INCLUDE_vTaskPrioritySet				1
#define INCLUDE_uxTaskPriorityGet				1
#define INCLUDE_vTaskDelete						1
#define INCLUDE_vTaskSuspend					1
#define INCLUDE_vTaskDelayUntil					1
#define INCLUDE_vTaskDelay						1	/* 要用 vTaskDelay 就必须打开这个 */
#define INCLUDE_xTaskGetSchedulerState			1
#define INCLUDE_xTaskGetCurrentTaskHandle		1
#define INCLUDE_uxTaskGetStackHighWaterMark		1

/* ------------------- Cortex-M 中断优先级 ------------------- */
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS						__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS						4
#endif

/* 本工程使用 NVIC_PriorityGroup_2，有 4 位抢占优先级，共 16 级 */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5

#define configKERNEL_INTERRUPT_PRIORITY \
	( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )

/* 只有优先级“数值”大于等于 5 的中断，才允许调用带 FromISR 后缀的 FreeRTOS 接口。
   本工程没有在中断里调用 FreeRTOS 接口，所以各外设的中断优先级都不用改。 */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )

/* ------------------- 断言 ------------------- */
#define configASSERT( x ) \
	if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/* ------------------- 把内核用到的异常挂到标准向量名上 ------------------- */
#define vPortSVCHandler			SVC_Handler
#define xPortPendSVHandler		PendSV_Handler
#define xPortSysTickHandler		SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
