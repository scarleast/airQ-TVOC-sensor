#include "timPWM.h"

/*
	PWM管理初始化
*/
void TIM_PWM_Init(void)
{
	TIM_GPIO_Config();
	TIM_Mode_Config();
}

/*
	配置TIM2　TIM3复用输出PWM时用到的IO口
*/
void TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);//开启TIM2，TIM3时钟源
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//开启GPIOA的时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
	配置TIM2 TIM3输出的PWM信号的模式，如周期、极性、占空比
*/
void TIM_Mode_Config()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1; //设置控制周期：舵机周期20ms
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; //设置预分频：不预分频，即为72MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//设置时钟分频系数：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
	
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//配置定时器2
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//配置定时器3
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//配置PMW模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1499;//设置跳变值，当计数器计到这个值的时候，电平发生跳转
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//当计时器计数值小于value时为高电平
	
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);//使能TIM2通道1
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);//使能TIM2通道2
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);//使能TIM2通道3
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC4Init(TIM2,&TIM_OCInitStructure);//使能TIM2通道4
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);//使能TIM3通道1
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);//使能TIM3通道2
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2,ENABLE);//使能定时器2
	TIM_Cmd(TIM3,ENABLE);//使能定时器3
}
