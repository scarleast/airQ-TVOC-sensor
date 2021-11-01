#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void uart_write(char * string);
void USART1_Config(void);
void USART2_Config(void);
void USART_NVIC_Configuration(void);
int fputc(int ch, FILE *f);

#endif /* __USART1_H */
