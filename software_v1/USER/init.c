#include "init.h"
#include "port.h"
#include "delay.h"
/*
	系统初始化
*/
void init(void)
{
    Port_Init();
	delay_init();   /* 滴答计时器初始化 */
	LED_Init();     /* LED指示灯初始化 */
}
