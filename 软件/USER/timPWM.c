#include "timPWM.h"

/*
	PWM�����ʼ��
*/
void TIM_PWM_Init(void)
{
	TIM_GPIO_Config();
	TIM_Mode_Config();
}

/*
	����TIM2��TIM3�������PWMʱ�õ���IO��
*/
void TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);//����TIM2��TIM3ʱ��Դ
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//����GPIOA��ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
	����TIM2 TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
*/
void TIM_Mode_Config()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1; //���ÿ������ڣ��������20ms
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ�ӷ�Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//���ö�ʱ��2
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//���ö�ʱ��3
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//����PMWģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1499;//��������ֵ�����������Ƶ����ֵ��ʱ�򣬵�ƽ������ת
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//����ʱ������ֵС��valueʱΪ�ߵ�ƽ
	
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);//ʹ��TIM2ͨ��1
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);//ʹ��TIM2ͨ��2
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);//ʹ��TIM2ͨ��3
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC4Init(TIM2,&TIM_OCInitStructure);//ʹ��TIM2ͨ��4
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);//ʹ��TIM3ͨ��1
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);//ʹ��TIM3ͨ��2
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2,ENABLE);//ʹ�ܶ�ʱ��2
	TIM_Cmd(TIM3,ENABLE);//ʹ�ܶ�ʱ��3
}
