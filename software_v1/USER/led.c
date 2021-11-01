#include "led.h"

/*
	初始化LED
*/
void LED_Init(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6 | GPIO_Pin_7,(BitAction)Bit_RESET); //初始化关闭两个LED灯
	GPIO_WriteBit(GPIOB, GPIO_Pin_0,(BitAction)Bit_RESET); //初始化关闭两个LED灯
}
