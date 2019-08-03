#include "stm32f10x.h"
#include "timPWM.h"
#include "sysTick.h"

/*
	��ʼ��������������
*/
void Servo_Init(void);

/*
	������ƺ�������6���������һ��ָ������0�Ƚ�
*/
void Servo_control(uint8_t * angle);

/*
	������ƺ�������6���������һ��ָ���������нǶ�
*/
void Servo_control_relative(int8_t * angle_relative);

/*
	������ƺ���������ִ�нǶ��л�
*/
void Servo_Control_break(int8_t * angle_relative, int8_t * angle_relative_pre, int8_t time);

void Servo_Walk_Start(void);
void Servo_Walk(void);
void Servo_Walk_Three(void);
void Servo_ForwardSomersault(void);
void Servo_BackwardSomersault(void);

void Servo_Walk_Start_two(void);
void Servo_Walk_two(void);
void Servo_Walk_Three_two(void);
void Servo_ForwardSomersault_two(void);
void Servo_BackwardSomersault_two(void);
