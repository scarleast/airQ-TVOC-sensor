/////////////////////////////////////////////////////////////////
//
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// STM32F103C8T6核心板	   
// by Pang
// 修改日期:2019/01/02
// 版本：V1.0
// 版权所有，盗版必究。
// All rights reserved
//
////////////////////////////////////////////////////////////////
#ifndef CCS811_h
#define CCS811_h
#include "delay.h"
#include "CCS811_IIC.h"
#include "usart.h"

#define boolean 				u8
#define byte 					u8
#define uint8_t 				u8
typedef struct {
u16 eco2;
u16 tvoc;
u8 status;
u8 device_id;
u8 error_id;
u16 raw_data;
} ccs811_measurement_t;

#define  CCS811_nWAKE_ON()    {PBout(15) = 0;}//delay_us(1);
#define  CCS811_nWAKE_OFF()   {PBout(15) = 1;}//delay_us(1);
#define CCS811_RST_OUT      PBout(13)
#define CCS811_INT_IN       PBin(14)

// Registers for CCS811
#define CCS811_Add      		0x5B<<1
#define STATUS_REG      		0x00
#define MEAS_MODE_REG   		0x01
#define ALG_RESULT_DATA 		0x02
#define ENV_DATA        		0x05
#define NTC_REG         		0x06
#define THRESHOLDS      		0x10
#define BASELINE 			    0x11
#define HW_ID_REG 			    0x20
#define ERROR_ID_REG 		    0xE0
#define APP_START_REG 			0xF4
#define SW_RESET 						0xFF
#define CCS_811_ADDRESS 		0x5A
#define GPIO_WAKE				 		0x5
#define DRIVE_MODE_IDLE 		0x0   // 不测量
#define DRIVE_MODE_1SEC 		0x10  // 1秒间隔测量
#define DRIVE_MODE_10SEC		0x20  // 10s间隔测量
#define DRIVE_MODE_60SEC 		0x30  // 每分钟测量
#define INTERRUPT_DRIVEN 		0x8   // 强力测量250ms
#define THRESHOLDS_ENABLED 	0x4

void CCS811_Reset(void);
int CCS811_Init(void);           // constructor
int CCS811_GetData(void);
uint8_t CCS811_ChangeMeasureMode(uint8_t mode);

#endif
