#include "init.h"
#include "port.h"
#include "delay.h"
/*
	ϵͳ��ʼ��
*/
void init(void)
{
    Port_Init();
	delay_init();   /* �δ��ʱ����ʼ�� */
	LED_Init();     /* LEDָʾ�Ƴ�ʼ�� */
}
