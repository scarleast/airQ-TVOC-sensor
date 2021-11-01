#include "stm32f10x.h"

/*******
 *按键按下标置
 KEY_ON 0
 KEY_OFF 1
 ********/
#define KEY_ON	0
#define KEY_OFF	1

/*
	按键初始化
*/
void key_Init(void);
/*
	简单延时
*/
void delay(__IO uint32_t nCount);
/*
	按键输入函数
*/
uint8_t key_Scan(GPIO_TypeDef * GPIOx,u16 GPIO_Pin);
