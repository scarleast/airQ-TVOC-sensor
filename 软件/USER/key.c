#include "key.h"
#include "exti.h"
#include "led.h"
/*
	������ʼ��
*/
void key_Init(void)
{
	EXTI_PA11_PA12_Config(1);
}
/*
	����ʱ
*/
void delay(__IO uint32_t nCount)
{
	for(;nCount != 0;nCount --);
}

/*
	�������뺯��
*/
uint8_t key_Scan(GPIO_TypeDef * GPIOx,u16 GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)
	{
		delay(10000);
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)
		{
			while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);
			return KEY_ON;
		}
		else
		{
			return KEY_OFF;
		}
	}
	else
	{
		return KEY_OFF;
	}
}
