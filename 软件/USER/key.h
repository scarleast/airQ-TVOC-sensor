#include "stm32f10x.h"

/*******
 *�������±���
 KEY_ON 0
 KEY_OFF 1
 ********/
#define KEY_ON	0
#define KEY_OFF	1

/*
	������ʼ��
*/
void key_Init(void);
/*
	����ʱ
*/
void delay(__IO uint32_t nCount);
/*
	�������뺯��
*/
uint8_t key_Scan(GPIO_TypeDef * GPIOx,u16 GPIO_Pin);
