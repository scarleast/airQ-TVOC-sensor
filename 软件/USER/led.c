#include "led.h"

/*
	��ʼ��LED
*/
void LED_Init(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6 | GPIO_Pin_7,(BitAction)Bit_RESET); //��ʼ���ر�����LED��
	GPIO_WriteBit(GPIOB, GPIO_Pin_0,(BitAction)Bit_RESET); //��ʼ���ر�����LED��
}
