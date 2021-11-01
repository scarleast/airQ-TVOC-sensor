#include "servo.h"
/*
	初始化舵机，舵机置中
*/
void Servo_Init(void)
{
	//uint8_t angle[6] = {85,94,88,84,95,83}; //FF 55 5E 58 54 5F 53 FE
	uint8_t angle[6] = {0X6A,0X5A,0X5A,0X6A,0X5A,0X5A};
	Servo_control(angle);
}

/*
	舵机控制函数：对6个舵机发送一次指令，向对于0度角
*/
void Servo_control(uint8_t * angle)
{
	u16 value[6] = {0};
	uint8_t Channel = 0;
	for(Channel = 0;Channel < 6;Channel ++)
	{
		value[Channel] = ((float)angle[Channel] / 180) * 2000 + 499;//0~180:0.5ms~2.5ms
		if(value[Channel] > 2499)
		{
			value[Channel] = 2499;
		}
		if(value[Channel] < 499)
		{
			value[Channel] = 499;
		}
	}	
	
	TIM2->CCR1 = value[0];
	TIM2->CCR2 = value[1];
	TIM2->CCR3 = value[2];
	TIM2->CCR4 = value[3];
	TIM3->CCR1 = value[4];
	TIM3->CCR2 = value[5];
}

/*
	舵机控制函数：对6个舵机发送一次指令，向对于置中角度
*/
void Servo_control_relative(int8_t * angle_relative)
{
	uint8_t angle[6];
	angle[0] = 0 + angle_relative[0];
	angle[1] = 0 + angle_relative[1];
	angle[2] = 0 + angle_relative[2];
	angle[3] = 0 + angle_relative[3];
	angle[4] = 0 + angle_relative[4];
	angle[5] = 0 + angle_relative[5];
	Servo_control(angle);
}
/*
	舵机控制函数：缓和执行角度切换
*/
void Servo_Control_break(int8_t * angle_relative, int8_t * angle_relative_pre, int8_t time)
{
	uint8_t channel = 0;
	uint8_t i = 0;
	int8_t angle_relative_temp[6] = {0};
	for(i = 0;i < time;i ++)
	{
		for(channel = 0;channel < 6;channel ++)
		{
			angle_relative_temp[channel] = (float)(angle_relative[channel] - angle_relative_pre[channel]) / time * (i + 1) + angle_relative_pre[channel];
		}
		Servo_control_relative(angle_relative_temp);
		delay_ms(21);
	}
}

#define step_walk 10 

void Servo_Walk_Start(void)
{
int8_t angle[step_walk][6] = {					0,0,0,0,0,-1,		  //直立FF 55 5E 58 54 5F 53 FE
									0,0,0,-16,-58,-36,					//FF 55 5E 58 44 25 30 FE
									-15,0,14,-20,-58,-36,				//FF 46 5E 66 40 25 30 FE
									-15,0,19,-16,-58,-47,				//FF 46 5E 6B 44 25 25 FE
									-15,0,19,-4,-29,-28,    //落脚FF 46 5E 6B 50 42 38 FE
									2,58,47,0,0,-1,					    //FF 57 98 87 54 5F 53 FE
									26,54,34,0,0,-1,						  //FF 6F 94 7A 54 5F 53 FE
									27,50,27,-11,0,12,					//FF 70 90 73 49 5F 60 FE
									6,26,23,-15,0,12				//落脚FF 5B 78 6F 45 5F 60 FE
													};
	Servo_Control_break(angle[1], angle[0], 20);
	Servo_Control_break(angle[2], angle[1], 15);
	Servo_Control_break(angle[3], angle[2], 15);
	Servo_Control_break(angle[4], angle[3], 15);
	Servo_Control_break(angle[5], angle[4], 20);
	Servo_Control_break(angle[6], angle[5], 15);
	Servo_Control_break(angle[7], angle[6], 15);
	Servo_Control_break(angle[8], angle[7], 15);
}

void Servo_Walk(void)
{
	int8_t angle[step_walk][6] = {6,26,23,-15,0,12,				//落脚FF 5B 78 6F 45 5F 60 FE
													0,0,0,-26,-42,-14,						//FF 55 5E 58 3A 35 46 FE
													0,0,0,-16,-58,-36,					//FF 55 5E 58 44 25 30 FE
													-15,0,14,-20,-58,-36,				//FF 46 5E 66 40 25 30 FE
													//-15,0,19,-16,-58,-47,				//FF 46 5E 6B 44 25 25 FE
													//-15,0,19,-4,-29,-28,    //落脚FF 46 5E 6B 50 42 38 FE
													-18,0,19,-20,-61,-52,				//FF 43 5E 6B 40 22 2A FE
													-14,0,20,-3,-26,-25,    //落脚FF 47 5E 6C 51 45 3B FE
													2,58,47,0,0,-1,					    //FF 57 98 87 54 5F 53 FE
													26,54,34,0,0,-1,						  //FF 6F 94 7A 54 5F 53 FE
													27,50,27,-11,0,12,					//FF 70 90 73 49 5F 60 FE
													6,26,23,-15,0,12				//落脚FF 5B 78 6F 45 5F 60 FE
													};
	Servo_Control_break(angle[1], angle[0], 20);
	Servo_Control_break(angle[2], angle[1], 15);
	Servo_Control_break(angle[3], angle[2], 15);
	Servo_Control_break(angle[4], angle[3], 15);
	Servo_Control_break(angle[5], angle[4], 15);
	Servo_Control_break(angle[6], angle[5], 20);
	Servo_Control_break(angle[7], angle[6], 15);
	Servo_Control_break(angle[8], angle[7], 15);
	Servo_Control_break(angle[9], angle[8], 15);
}

#define step_walk_three 17
//#define step_walk_three 2

void Servo_Walk_Three(void)
{
	int8_t angle[22][6] = {0X6A,0X5A,0X5A,0X6A,0X5A,0X5A,									//直立
										0x6A,0x50,0x4B,0x6A,0x64,0x69,						//1准备
										0x6A,0x50,0x4B,0x6A,0x60,0x64,						//2抬右脚
										0x50,0x4C,0x65,0x6A,0x5A,0x5A,						//3抬右脚
										0X57,0X4D,0X62,0X61,0X5D,0X6B,						//4迈右脚/////////
										0X57,0X4D,0X62,0X61,0X64,0X76,						//5落右脚
										0X6A,0X5A,0X5A,0X6A,0X70,0X74,						//6抬左脚
										0X6A,0X5A,0X5A,0X7A,0X7F,0X74,						//7迈左脚1
										0X6A,0X50,0X4B,0X7A,0X5A,0X40,						//8落左脚
										0X6A,0X45,0X38,0X7A,0X5A,0X40,						//9
										0X6A,0X40,0X35,0X6A,0X5A,0X5A,						//10迈右脚1
										0X50,0X45,0X5A,0X6A,0X5A,0X5A,						//11
										0x50,0x4C,0x65,0x6A,0x5A,0x5A,						//12迈右脚2
										0X57,0X4D,0X62,0X61,0X5D,0X6B,						//13迈右脚///////
										0X57,0X4D,0X62,0X61,0X64,0X76,						//14落右脚
										0X6A,0X5A,0X5A,0X6A,0X70,0X74,						//15抬左脚
										0X6A,0X5A,0X5A,0X7A,0X7F,0X74,						//16迈左脚1
										0X6A,0X50,0X4B,0X7A,0X5A,0X40,						//17落左脚
										0X6A,0X45,0X38,0X7A,0X5A,0X40,						//18抬右脚
										0X6A,0X40,0X35,0X6A,0X5A,0X5A,						//19迈右脚1
										0X50,0X45,0X5A,0X6A,0X5A,0X5A,						//20
										0x50,0x4C,0x63,0x6A,0x5A,0x5A						//21迈右脚2
						};
//	Servo_Control_break(angle[1], angle[0], 20);
//	Servo_Control_break(angle[2], angle[1], 10);
	Servo_Control_break(angle[3], angle[2], 20);
	Servo_Control_break(angle[4], angle[3], 20);
	Servo_Control_break(angle[5], angle[4], 15);
	Servo_Control_break(angle[6], angle[5], 15);
	Servo_Control_break(angle[7], angle[6], 15);
	Servo_Control_break(angle[8], angle[7], 15);
	Servo_Control_break(angle[9], angle[8], 15);
	Servo_Control_break(angle[10], angle[9], 20);
	Servo_Control_break(angle[11], angle[10], 20);
	Servo_Control_break(angle[12], angle[11], 15);
	Servo_Control_break(angle[13], angle[12], 15);
	Servo_Control_break(angle[14], angle[13], 20);
	Servo_Control_break(angle[15], angle[14], 15);
	Servo_Control_break(angle[16], angle[15], 20);
	Servo_Control_break(angle[17], angle[16], 15);
	Servo_Control_break(angle[18], angle[17], 15);
	Servo_Control_break(angle[19], angle[18], 20);
	Servo_Control_break(angle[20], angle[19], 20);
	Servo_Control_break(angle[21], angle[20], 15);
	while(1)
	{
	//	Servo_Control_break(angle[1], angle[0], 20);
	//	Servo_Control_break(angle[2], angle[1], 10);
	//	Servo_Control_break(angle[3], angle[2], 20);
		Servo_Control_break(angle[4], angle[3], 20);
		Servo_Control_break(angle[5], angle[4], 15);
		Servo_Control_break(angle[6], angle[5], 15);
		Servo_Control_break(angle[7], angle[6], 15);
		Servo_Control_break(angle[8], angle[7], 15);
		Servo_Control_break(angle[9], angle[8], 15);
		Servo_Control_break(angle[10], angle[9], 20);
		Servo_Control_break(angle[11], angle[10], 20);
		Servo_Control_break(angle[12], angle[11], 15);
	//	Servo_Control_break(angle[13], angle[12], 15);
//		Servo_Control_break(angle[14], angle[13], 20);
//		Servo_Control_break(angle[15], angle[14], 15);
//		Servo_Control_break(angle[16], angle[15], 20);
//		Servo_Control_break(angle[17], angle[16], 15);
//		Servo_Control_break(angle[18], angle[17], 15);
//		Servo_Control_break(angle[19], angle[18], 20);
//		Servo_Control_break(angle[20], angle[19], 20);
//		Servo_Control_break(angle[21], angle[20], 15);
	}
}
//void Servo_Walk_Three(void)
//{
//	int8_t angle[step_walk_three][6] = {0,0,0,0,0,-1,						//直立
//													0,0,0,-16,-58,-36,					//FF 55 5E 58 44 25 30 FE
//													-15,0,14,-20,-58,-36,				//FF 46 5E 66 40 25 30 FE
//													-15,0,19,-16,-58,-47,				//FF 46 5E 6B 44 25 25 FE
//													-15,0,20,-4,-29,-28,    //落脚FF 46 5E 6C 50 42 38 FE
//													2,58,47,0,0,-1,					    //FF 57 98 87 54 5F 53 FE
//													26,54,34,0,0,-1,						  //FF 6F 94 7A 54 5F 53 FE
//													27,50,27,-11,0,12,					//FF 70 90 73 49 5F 60 FE
//													6,26,23,-15,0,12,				//落脚FF 5B 78 6F 45 5F 60 FE
//													0,0,0,-26,-42,-14,						//FF 55 5E 58 3A 35 46 FE
//													0,0,0,-16,-58,-36,					//FF 55 5E 58 44 25 30 FE
//													-15,0,14,-20,-58,-36,				//FF 46 5E 66 40 25 30 FE
//													-15,0,19,-16,-58,-47,				//FF 46 5E 6B 44 25 25 FE
//													-15,0,20,-4,-29,-28,    //落脚FF 46 5E 6C 50 42 38 FE
//													2,58,47,0,0,-1,					    //FF 57 98 87 54 5F 53 FE
//													26,54,34,0,0,-1,						  //FF 6F 94 7A 54 5F 53 FE
//													0,0,0,0,0,-1
//													};
//	Servo_Control_break(angle[1], angle[0], 20);
//	Servo_Control_break(angle[2], angle[1], 15);
//	Servo_Control_break(angle[3], angle[2], 15);
//	Servo_Control_break(angle[4], angle[3], 15);
//	Servo_Control_break(angle[5], angle[4], 20);
//	Servo_Control_break(angle[6], angle[5], 15);
//	Servo_Control_break(angle[7], angle[6], 15);
//	Servo_Control_break(angle[8], angle[7], 15);
//	Servo_Control_break(angle[9], angle[8], 20);
//	Servo_Control_break(angle[10], angle[9], 15);
//	Servo_Control_break(angle[11], angle[10], 15);
//	Servo_Control_break(angle[12], angle[11], 15);
//	Servo_Control_break(angle[13], angle[12], 15);
//	Servo_Control_break(angle[14], angle[13], 20);
//	Servo_Control_break(angle[15], angle[14], 15);
//	Servo_Control_break(angle[16], angle[15], 15);
//}

#define step_Somersault 13

/*void Servo_ForwardSomersault(void)
{
	int8_t angle[step_Somersault][6] = {0,0,0,0,0,0,						//直立
													85,0,0,0,0,-80,					
													85,-85,0,0,85,-80,	
													85,-49,46,-47,43,-80,				
													0,0,-51,-47,43,-80,
													//0,42,-51,-47,43,-80,
													-85,42,-51,-47,43,-80,				
													-85,42,-51,53,0,0,
													//-85,42,-51,53,-41,0,
													-85,42,-51,53,-41,90,
													-85,85,0,0,-95,90,
													//-85,0,0,0,0,90,
													0,0,0,0,0,0
													};
	Servo_Control_break(angle[1], angle[0], 15);
	Servo_Control_break(angle[2], angle[1], 15);
	Servo_Control_break(angle[3], angle[2], 15);
	Servo_Control_break(angle[4], angle[3], 15);
	Servo_Control_break(angle[5], angle[4], 15);
	Servo_Control_break(angle[6], angle[5], 15);
	Servo_Control_break(angle[7], angle[6], 15);
	Servo_Control_break(angle[8], angle[7], 15);
	Servo_Control_break(angle[9], angle[8], 20);
	//Servo_Control_break(angle[10], angle[9], 15);
	//Servo_Control_break(angle[11], angle[10], 15);
}*/

void Servo_ForwardSomersault(void)
{
	int8_t angle[step_Somersault][6] = {0,0,0,0,0,-1,						//直立
													85,0,0,0,0,-80,					
													80,-85,0,0,85,-75,	
													78,-80,26,-28,78,-70,	//78,-80,26,-28,78,-70,				85,-50,49,-50,43,-83,
													0,-80,26,-28,78,-70,
													0,61,-39,-28,78,-70,
													-80,61,-39,-28,78,-70,				
													-80,61,-39,-28,78,0,
													-80,61,-39,37,-65,0,
													-80,61,-39,37,-65,83, //-80,61,-39,37,-65,83,      -80,43,-48,51,-44,89,
													-85,85,0,0,-88,90,
													-85,0,0,0,0,90,
													0,0,0,0,0,-1
													};
	Servo_Control_break(angle[1], angle[0], 15);
	Servo_Control_break(angle[2], angle[1], 15);
	Servo_Control_break(angle[3], angle[2], 15);
	Servo_Control_break(angle[4], angle[3], 10);
	Servo_Control_break(angle[5], angle[4], 10);
	Servo_Control_break(angle[6], angle[5], 20);
	Servo_Control_break(angle[7], angle[6], 10);
	Servo_Control_break(angle[8], angle[7], 10);
	Servo_Control_break(angle[9], angle[8], 20);
	Servo_Control_break(angle[10], angle[9], 15);
	Servo_Control_break(angle[11], angle[10], 15);
	Servo_Control_break(angle[12], angle[11], 20);
}


void Servo_BackwardSomersault(void)
{
	int8_t angle[step_Somersault][6] = {0,0,0,0,0,-1,						//直立
													-85,0,0,0,0,90,					
													-80,85,0,0,-95,85,	
													-80,61,-39,37,-65,83,				
													0,61,-39,37,-65,83,
													0,-80,26,37,-65,83,
													78,-80,26,37,-65,83,				
													78,-80,26,37,-65,0,
													78,-80,26,-28,78,0,
													78,-80,26,-28,78,-70,
													85,-85,0,0,85,-80,
													85,0,0,0,0,-80,
													0,0,0,0,0,-1
													};
	Servo_Control_break(angle[1], angle[0], 15);
	Servo_Control_break(angle[2], angle[1], 15);
	Servo_Control_break(angle[3], angle[2], 15);
	Servo_Control_break(angle[4], angle[3], 10);
	Servo_Control_break(angle[5], angle[4], 10);
	Servo_Control_break(angle[6], angle[5], 20);
	Servo_Control_break(angle[7], angle[6], 10);
	Servo_Control_break(angle[8], angle[7], 10);
	Servo_Control_break(angle[9], angle[8], 20);
	Servo_Control_break(angle[10], angle[9], 15);
	Servo_Control_break(angle[11], angle[10], 15);
	Servo_Control_break(angle[12], angle[11], 20);
}







/*
	第二套作死程序
*/
void Servo_Walk_Start_two(void)
{
	int8_t angle[step_walk][6] = {0,0,0,0,0,-1,		  //直立FF 55 5E 58 54 5F 53 FE
													0,0,0,-16,-58,-36,					//FF 55 5E 58 44 25 30 FE
													-15,0,14,-20,-58,-36,				//FF 46 5E 66 40 25 30 FE
													-15,0,19,-16,-58,-47,				//FF 46 5E 6B 44 25 25 FE
													-15,0,19,-4,-29,-28,    //落脚FF 46 5E 6B 50 42 38 FE
													2,58,47,0,0,-1,					    //FF 57 98 87 54 5F 53 FE
													26,54,34,0,0,-1,						  //FF 6F 94 7A 54 5F 53 FE
													27,50,27,-11,0,12,					//FF 70 90 73 49 5F 60 FE
													6,26,23,-15,0,12				//落脚FF 5B 78 6F 45 5F 60 FE
													};
	Servo_Control_break(angle[1], angle[0], 13);
	Servo_Control_break(angle[2], angle[1], 13);
	Servo_Control_break(angle[3], angle[2], 13);
	Servo_Control_break(angle[4], angle[3], 13);
	Servo_Control_break(angle[5], angle[4], 13);
	Servo_Control_break(angle[6], angle[5], 13);
	Servo_Control_break(angle[7], angle[6], 13);
	Servo_Control_break(angle[8], angle[7], 13);
}

void Servo_Walk_two(void)
{
	int8_t angle[step_walk][6] = {6,26,23,-15,0,12,				//落脚FF 5B 78 6F 45 5F 60 FE
													0,0,0,-26,-42,-14,						//FF 55 5E 58 3A 35 46 FE
													0,0,0,-16,-58,-36,					//FF 55 5E 58 44 25 30 FE
													-15,0,14,-20,-58,-36,				//FF 46 5E 66 40 25 30 FE
													-15,0,19,-16,-58,-47,				//FF 46 5E 6B 44 25 25 FE
													-15,0,19,-4,-29,-28,    //落脚FF 46 5E 6B 50 42 38 FE
													2,58,47,0,0,-1,					    //FF 57 98 87 54 5F 53 FE
													26,54,34,0,0,-1,						  //FF 6F 94 7A 54 5F 53 FE
													27,50,27,-11,0,12,					//FF 70 90 73 49 5F 60 FE
													6,26,23,-15,0,12				//落脚FF 5B 78 6F 45 5F 60 FE
													};
	Servo_Control_break(angle[1], angle[0], 13);
	Servo_Control_break(angle[2], angle[1], 13);
	Servo_Control_break(angle[3], angle[2], 13);
	Servo_Control_break(angle[4], angle[3], 13);
	Servo_Control_break(angle[5], angle[4], 13);
	Servo_Control_break(angle[6], angle[5], 13);
	Servo_Control_break(angle[7], angle[6], 13);
	Servo_Control_break(angle[8], angle[7], 13);
	Servo_Control_break(angle[9], angle[8], 13);
}

void Servo_Walk_Three_two(void)
{
	int8_t angle[step_walk_three][6] = {0,0,0,0,0,-1,						//直立
													0,0,0,-16,-58,-36,					//FF 55 5E 58 44 25 30 FE
													-15,0,14,-20,-58,-36,				//FF 46 5E 66 40 25 30 FE
													-15,0,19,-16,-58,-47,				//FF 46 5E 6B 44 25 25 FE
													-15,0,20,-4,-29,-28,    //落脚FF 46 5E 6C 50 42 38 FE
													2,58,47,0,0,-1,					    //FF 57 98 87 54 5F 53 FE
													26,54,34,0,0,-1,						  //FF 6F 94 7A 54 5F 53 FE
													27,50,27,-11,0,12,					//FF 70 90 73 49 5F 60 FE
													6,26,23,-15,0,12,				//落脚FF 5B 78 6F 45 5F 60 FE
													0,0,0,-26,-42,-13,						//FF 55 5E 58 3A 35 46 FE
													0,0,0,-16,-58,-35,					//FF 55 5E 58 44 25 30 FE
													-15,0,14,-20,-58,-35,				//FF 46 5E 66 40 25 30 FE
													-15,0,19,-16,-58,-47,				//FF 46 5E 6B 44 25 25 FE
													-15,0,20,-4,-29,-28,    //落脚FF 46 5E 6C 50 42 38 FE
													2,58,47,0,0,0,					    //FF 57 98 87 54 5F 53 FE
													26,54,34,0,0,0,						  //FF 6F 94 7A 54 5F 53 FE
													0,0,0,0,0,0
													};
	Servo_Control_break(angle[1], angle[0], 13);
	Servo_Control_break(angle[2], angle[1], 13);
	Servo_Control_break(angle[3], angle[2], 13);
	Servo_Control_break(angle[4], angle[3], 13);
	Servo_Control_break(angle[5], angle[4], 13);
	Servo_Control_break(angle[6], angle[5], 13);
	Servo_Control_break(angle[7], angle[6], 13);
	Servo_Control_break(angle[8], angle[7], 13);
	Servo_Control_break(angle[9], angle[8], 13);
	Servo_Control_break(angle[10], angle[9], 13);
	Servo_Control_break(angle[11], angle[10], 13);
	Servo_Control_break(angle[12], angle[11], 13);
	Servo_Control_break(angle[13], angle[12], 13);
	Servo_Control_break(angle[14], angle[13], 13);
	Servo_Control_break(angle[15], angle[14], 13);
	Servo_Control_break(angle[16], angle[15], 13);
}


void Servo_ForwardSomersault_two(void)
{
	int8_t angle[step_Somersault][6] = {0,0,0,0,0,0,						//直立
													85,0,0,0,0,-80,					
													80,-85,0,0,85,-75,	
													78,-80,26,-28,78,-70,				
													0,-80,26,-28,78,-70,
													0,61,-39,-28,78,-70,
													-80,61,-39,-28,78,-70,				
													-80,61,-39,-28,78,0,
													-80,61,-39,37,-65,0,
													-80,61,-39,37,-65,83,
													-85,85,0,0,-88,90,
													-85,0,0,0,0,90,
													0,0,0,0,0,0
													};
	Servo_Control_break(angle[1], angle[0], 15);
	Servo_Control_break(angle[2], angle[1], 15);
	Servo_Control_break(angle[3], angle[2], 15);
	Servo_Control_break(angle[4], angle[3], 10);
	Servo_Control_break(angle[5], angle[4], 10);
	Servo_Control_break(angle[6], angle[5], 20);
	Servo_Control_break(angle[7], angle[6], 10);
	Servo_Control_break(angle[8], angle[7], 10);
	Servo_Control_break(angle[9], angle[8], 20);
	Servo_Control_break(angle[10], angle[9], 15);
	Servo_Control_break(angle[11], angle[10], 15);
	Servo_Control_break(angle[12], angle[11], 20);
}


void Servo_BackwardSomersault_two(void)
{
	int8_t angle[step_Somersault][6] = {0,0,0,0,0,0,						//直立
													-85,0,0,0,0,90,					
													-80,85,0,0,-95,85,	
													-80,61,-39,37,-65,83,				
													0,61,-39,37,-65,83,
													0,-80,26,37,-65,83,
													78,-80,26,37,-65,83,				
													78,-80,26,37,-65,0,
													78,-80,26,-28,78,0,
													78,-80,26,-28,78,-70,
													85,-85,0,0,85,-80,
													85,0,0,0,0,-80,
													0,0,0,0,0,0
													};
	Servo_Control_break(angle[1], angle[0], 15);
	Servo_Control_break(angle[2], angle[1], 15);
	Servo_Control_break(angle[3], angle[2], 15);
	Servo_Control_break(angle[4], angle[3], 10);
	Servo_Control_break(angle[5], angle[4], 10);
	Servo_Control_break(angle[6], angle[5], 20);
	Servo_Control_break(angle[7], angle[6], 10);
	Servo_Control_break(angle[8], angle[7], 10);
	Servo_Control_break(angle[9], angle[8], 20);
	Servo_Control_break(angle[10], angle[9], 15);
	Servo_Control_break(angle[11], angle[10], 15);
	Servo_Control_break(angle[12], angle[11], 20);
}
