#include "stm32f10x.h"

/*
	外部中断配置函数
*/
void NVIC_Config(uint8_t state);

/*
	GPIO与外部中断的配置
*/
void EXTI_PA11_PA12_Config(uint8_t state);
