/**
	******************************************************************************
	* File Name          : CCS811.h
	* Description        : This file contains the common defines of the application
	******************************************************************************
	*
	* COPYRIGHT(c) 2017 Akash kapashia
       * Created by Akash kapashia
	******************************************************************************
*/
 
       /* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __CCS811_BASIC_H
#define __CCS811_BASIC_H
#include "i2c.h"

#define CCS811_ADDR 0x5A<<1 //7-bit unshifted default I2C Address
#define CCS811_ADDR_W 0xB4
#define CCS811_ADDR_R 0xB5

#define driveMode_Idle 0
#define driveMode_1sec 1
#define driveMode_10sec 2
#define driveMode_60sec 3
#define driveMode_RAW 4

#define CSS811_STATUS 0x00
#define CSS811_MEAS_MODE 0x01
#define CSS811_ALG_RESULT_DATA 0x02
#define CSS811_RAW_DATA 0x03
#define CSS811_ENV_DATA 0x05
#define CSS811_NTC 0x06
#define CSS811_THRESHOLDS 0x10
#define CSS811_BASELINE 0x11
#define CSS811_HW_ID 0x20
#define CSS811_HW_VERSION 0x21
#define CSS811_FW_BOOT_VERSION 0x23
#define CSS811_FW_APP_VERSION 0x24
#define CSS811_ERROR_ID 0xE0
#define CSS811_APP_START 0xF4
#define CSS811_SW_RESET 0xFF

void readAlgorithmResults(void);
char configureCCS811(void);
FlagStatus checkForError(void);
unsigned int getBaseline(void);
FlagStatus dataAvailable(void);
FlagStatus appValid(void);
FlagStatus checkDataAndUpdate();
void enableInterrupts(void);
void disableInterrupts(void);
void setDriveMode(uint8_t mode);
void setEnvironmentalData(float relativeHumidity, float temperature);
void Init_I2C_CCS811(void);
void softRest(void);
void ccs811_sleep(void);
unsigned int get_Sensor_Resistance(void);
void restore_Baseline(void);
unsigned int getCo2();
unsigned int gettVOC();
void ccs811_wakeup(void);


#endif
