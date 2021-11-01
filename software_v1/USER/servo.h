#include "stm32f10x.h"
#include "timPWM.h"
#include "sysTick.h"

/*
	初始化舵机，舵机置中
*/
void Servo_Init(void);

/*
	舵机控制函数：对6个舵机发送一次指令，向对于0度角
*/
void Servo_control(uint8_t * angle);

/*
	舵机控制函数：对6个舵机发送一次指令，向对于置中角度
*/
void Servo_control_relative(int8_t * angle_relative);

/*
	舵机控制函数：缓和执行角度切换
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
