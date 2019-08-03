#include "stm32f10x.h"

/*
	PWM管理初始化
*/
void TIM_PWM_Init(void);

/*
	配置TIM2　TIM3复用输出PWM时用到的IO口
*/
void TIM_GPIO_Config(void);

/*
	配置TIM2 TIM3输出的PWM信号的模式，如周期、极性、占空比
*/
void TIM_Mode_Config(void);
