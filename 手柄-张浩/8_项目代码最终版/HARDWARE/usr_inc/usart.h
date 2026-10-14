#ifndef __USART_H
#define __USART_H
#include "stm32f4xx.h"
void USART1_Init_Config(void);
void Usart_SendByte(USART_TypeDef * pUSARTx , uint16_t ch);
void Usart_SendString(USART_TypeDef * pUSARTx , char* str);
void USART3_Init_Config(uint32_t baud);
#endif
