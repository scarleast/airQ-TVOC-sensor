#include "stm32f10x.h"

#define ON  0
#define OFF 1

/* ���κ꣬��������������һ��ʹ�� */

#if 0
#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_6);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_6)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_7);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_7)
                    
#define LED3(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#endif
#define LED1        PAout(6)
#define LED2        PAout(7)
#define LED3        PBout(0)

/*
	��ʼ��LED
*/
void LED_Init(void);

/*
	LED������ģʽ
*/
void LED_Mode(uint8_t mode);
