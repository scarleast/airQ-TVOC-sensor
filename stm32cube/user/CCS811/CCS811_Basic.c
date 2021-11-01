/**
	******************************************************************************
	* File Name          : CCS811.C
	* Description        : This file contains the common defines of the application
	******************************************************************************
	*
	* COPYRIGHT(c) 2017 Akash kapashia
       * Created by Akash kapashia
	******************************************************************************
*/
 
	/* Includes ------------------------------------------------------------------*/
#include "CCS811_Basic.h"
#include "stdio.h"

//#include "HDC_1080_TH.h"
//These are the air quality values obtained from the sensor
static unsigned int tVOC = 0;
static unsigned int CO2 = 0;
static unsigned short int adc_raw=0;
static unsigned char current_value=0;
static unsigned char appvalue=0;
static unsigned char errvalue=0;
static unsigned char mosetting=0;
static unsigned char dtvalue =0;
static float relativeHumidity=50.0,temperature=25.0;

static unsigned char readRegister(unsigned char addr);
static void writeRegister(unsigned char addr, unsigned char val);

/*
  * @brief  configureCCS811.
  * @param  NONE.
  * @retval None.
 */
char configureCCS811()
{
	HAL_Delay(100);
	ccs811_wakeup();
	HAL_Delay(500);

	//Verify the hardware ID is what we expect
	unsigned char hwID = readRegister(0x20); //Hardware ID should be 0x81
	if (hwID != 0x81){
		printf("hwID:%d, CCS811 not found. Please check wiring.\n", hwID);
		return -1; //Freeze!
	}

	if (checkForError() == 1){
		return -2;
	}

	if(appValid() == 0){
		return -3;
	}

	unsigned char    lodata[1];
	lodata[0]= CSS811_APP_START;
	HAL_I2C_Master_Transmit(&hi2c2, CCS811_ADDR, lodata, 1, 100);

	if (checkForError() == 1){
			return -4;
	}

	setDriveMode(driveMode_1sec); //Read every second

	if (checkForError() == 1){
		while (1); //Freeze!
	}
	//Set Drive Mode

	//Check for errors
	if (checkForError() == 1){
		return -5;
	}

	setEnvironmentalData(relativeHumidity, temperature);
	while(!checkDataAndUpdate());
//	if(checkDataAndUpdate()){
//	  printf("co2:%d, tvoc:%d\n", getCo2(), gettVOC());
//	}else{
//	  printf("can't read co2 and tvoc");
//	}
	HAL_Delay(2);
//	unsigned int result = getBaseline();

//	readHumidity();
//	readTemperature();
//	setEnvironmentalData(  HDC1080_Humidity/1000,  HDC1080_Temp);
	return 1;
}

		
/*
  * @brief  Updates the total voltatile organic compounds (TVOC) in parts per billion (PPB) and the CO2 value.
  * @param  NONE.
  * @retval None.
 */	
void readAlgorithmResults()
{
	 
	unsigned char data_rq[4];
	HAL_I2C_Mem_Read( &hi2c2, CCS811_ADDR, ( unsigned char )CSS811_ALG_RESULT_DATA, I2C_MEMADD_SIZE_8BIT, data_rq, 4,
															100 );

	unsigned char co2MSB = data_rq[0];
	unsigned char co2LSB = data_rq[1];
	unsigned char tvocMSB = data_rq[2];
	unsigned char tvocLSB = data_rq[3];

	/*	TVOC value, in parts per billion (ppb)

	eC02 value, in parts per million (ppm) */
	CO2 = ((unsigned int)co2MSB << 8) | co2LSB;
	tVOC = ((unsigned int)tvocMSB << 8) | tvocLSB;
}


//Checks to see if error bit is set
FlagStatus checkForError(){
	errvalue=readRegister(CSS811_ERROR_ID);
	errvalue = readRegister(CSS811_STATUS);
	return (errvalue & 1 << 0);
}

FlagStatus checkDataAndUpdate(){
	if(dataAvailable()){
		readAlgorithmResults();
		return 1;
	};
	return 0;
}



//Checks to see if DATA_READ flag is set in the status register
FlagStatus dataAvailable(){
	// dtvalue = readRegister(CSS811_ERROR_ID);
	//HAL_Delay(00);
	dtvalue = readRegister(CSS811_STATUS);
	return (dtvalue & 1 << 3);
}

//Checks to see if APP_VALID flag is set in the status register
FlagStatus appValid(){
	appvalue = readRegister(CSS811_STATUS);
	return (appvalue & (1 << 4));
}

//Enable the nINT signal
void enableInterrupts(void){
	unsigned char setting = readRegister(CSS811_MEAS_MODE); //Read what's currently there
	setting |= (1 << 3); //Set INTERRUPT bit
	writeRegister(CSS811_MEAS_MODE, setting);
}

//Disable the nINT signal
void disableInterrupts(void){
	unsigned char setting = readRegister(CSS811_MEAS_MODE); //Read what's currently there
	setting &= ~(1 << 3); //Clear INTERRUPT bit
	writeRegister(CSS811_MEAS_MODE, setting);
}

/*
  * @brief  //Mode 0 = Idle
	    //Mode 1 = read every 1s
	    //Mode 2 = every 10s
	    //Mode 3 = every 60s
	    //Mode 4 = RAW mode.
  * @param  MODE.
  * @retval None.
 */
void setDriveMode(unsigned char mode){
	if (mode > 4) mode = 4; //Error correction

	mosetting = readRegister(CSS811_MEAS_MODE); //Read what's currently there

	mosetting &=~(7<<4); //Clear DRIVE_MODE bits
	mosetting |= (mode << 4); //Mask in mode
	mosetting |= (1 << 3); //interrupt in mode

	writeRegister(CSS811_MEAS_MODE, mosetting);
	mosetting = readRegister(CSS811_MEAS_MODE); //Read what's currently there
	 
}

 
 /*
  * @brief //Given a temp and humidity, write this data to the CSS811 for better compensation
	 //This function expects the humidity and temp to come in as floats
  * @param  relativeHumidity HUMIDITY.
  * @param  temperature TEMPERATURE.
  * @retval None.
 */
void setEnvironmentalData(float relativeHumidity, float temperature){
	int rH = relativeHumidity * 1000; //42.348 becomes 42348
	int temp = temperature * 1000; //23.2 becomes 23200

	unsigned char envData[4];

	//Split value into 7-bit integer and 9-bit fractional
	envData[0] = ((rH % 1000) / 100) > 7 ? (rH / 1000 + 1) << 1 : (rH / 1000) << 1;
	envData[1] = 0; //CCS811 only supports increments of 0.5 so bits 7-0 will always be zero
	if (((rH % 1000) / 100) > 2 && (((rH % 1000) / 100) < 8)){
		envData[0] |= 1; //Set 9th bit of fractional to indicate 0.5%
	}

	temp += 25000; //Add the 25C offset
	//Split value into 7-bit integer and 9-bit fractional
	envData[2] = ((temp % 1000) / 100) > 7 ? (temp / 1000 + 1) << 1 : (temp / 1000) << 1;
	envData[3] = 0;
	if (((temp % 1000) / 100) > 2 && (((temp % 1000) / 100) < 8)){
	envData[2] |= 1;  //Set 9th bit of fractional to indicate 0.5C
	}

	HAL_I2C_Mem_Write( &hi2c2, CCS811_ADDR, ( unsigned char )CSS811_ENV_DATA, I2C_MEMADD_SIZE_8BIT, envData, 4,
							100 );
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){
	}
	while (HAL_I2C_IsDeviceReady(&hi2c2, CCS811_ADDR, 10, 300) == HAL_TIMEOUT);
	while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){
	}

}


 /*
  * @brief  reading_Multiple_Register
  * @param  addr ADDRESS.
  * @param  val VALUE.
  * @param  size SIZE.
  * @retval None.
 */
void read_Mul_Register(unsigned char addr, unsigned char * val,unsigned char size){

HAL_I2C_Mem_Read( &hi2c2, CCS811_ADDR, ( unsigned char )addr, I2C_MEMADD_SIZE_8BIT, val, size,100 );

}

 /*
  * @brief  softRest
  * @param  NONE.
  * @retval None.
 */
void softRest() {
	unsigned char rstCMD[5] = {CSS811_SW_RESET, 0x11,0xE5,0x72,0x8A};

	HAL_I2C_Mem_Write( &hi2c2, CCS811_ADDR, CSS811_SW_RESET, I2C_MEMADD_SIZE_8BIT, rstCMD, 5,300);
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY);
	      
}	


 /*
  * @brief  get_Sensor_Resistance
  * @param  NONE.
  * @retval RESISTANCE OF SENSOR.
 */
unsigned int get_Sensor_Resistance(){
	unsigned char sensor_Resistance_raw[2];
	unsigned int sensor_Resistance;
	
	HAL_I2C_Mem_Read( &hi2c2, CCS811_ADDR, CSS811_RAW_DATA , I2C_MEMADD_SIZE_8BIT, sensor_Resistance_raw, 2,100 );
	current_value=sensor_Resistance_raw[0]>>2;
	sensor_Resistance_raw[0]=sensor_Resistance_raw[0]&0x03;
	
	adc_raw=(sensor_Resistance_raw[0]<<8)|sensor_Resistance_raw[1];
	
	sensor_Resistance=((165*adc_raw)*10000)/(current_value*1023);
 
	return sensor_Resistance;
}

void ccs811_wakeup(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}


 /*
  * @brief  sleep
  * @param  NONE.
  * @retval NONE.
 */
void ccs811_sleep(void){
	// sets sensor to idle; measurements are disabled; lowest power mode
	writeRegister(CSS811_MEAS_MODE, 0);
}
		 
/*
  * @brief  	 //restore the baseline value
	       //Used for telling sensor what 'clean' air is
	      //You must put the sensor in clean air and record this value
  * @param  NONE.
  * @retval NONE.
 */
void restore_Baseline(){
	unsigned int  restore_Baseline;
	// 如果需要重置baseline，从EEPROM里面读取restore_Baseline并赋值
	restore_Baseline = 0;
	unsigned char res_bs[2];
	res_bs[0]=restore_Baseline>>8;
	res_bs[1]=restore_Baseline&0x000000FF;
	HAL_I2C_Mem_Write( &hi2c2, CCS811_ADDR, CSS811_BASELINE, I2C_MEMADD_SIZE_8BIT, res_bs,2,300);
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){
	}
}
 
/*
  * @brief  	//Returns the baseline value and saves into EEPROM
	       //Used for telling sensor what 'clean' air is
	      //You must put the sensor in clean air and record this value
  * @param  NONE.
  * @retval BASELINE VALUE.
 */
unsigned int getBaseline(){
	unsigned char ada[2];
//	HAL_StatusTypeDef status = HAL_OK;

	HAL_I2C_Mem_Read( &hi2c2, CCS811_ADDR, ( unsigned char )CSS811_BASELINE,1,  ada, 2,100 );
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){
	}
	while (HAL_I2C_IsDeviceReady(&hi2c2, CCS811_ADDR, 10, 300) == HAL_TIMEOUT);
	while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){
	}

	unsigned char baselineMSB = ada[0];
	unsigned char baselineLSB = ada[1];

	unsigned int baseline = ((unsigned int)baselineMSB << 8) | baselineLSB;
	return (baseline);
}

//Write a value to a spot in the CCS811
/*
* @brief  Write a value to a spot in the CCS811
* @param  addr ADDRESS.
* @param  val  VALUE.
* @retval NONE.
*/
static void writeRegister(unsigned char addr, unsigned char val){
	HAL_I2C_Mem_Write( &hi2c2, CCS811_ADDR_W, ( unsigned char )addr, I2C_MEMADD_SIZE_8BIT, &val, 1,1000);
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){
	};
	while (HAL_I2C_IsDeviceReady(&hi2c2, CCS811_ADDR, 10, 300) == HAL_TIMEOUT);
	while(HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){
	};
}

/*
 * @brief  Reads from a give location from the CSS811
 * @param  addr  ADDRESS.
 * @retval VALUE AT THE ADDRESS.
*/
static unsigned char readRegister(unsigned char addr){
	unsigned char dt;

	HAL_I2C_Mem_Read( &hi2c2, CCS811_ADDR_R, ( unsigned char )addr,I2C_MEMADD_SIZE_8BIT, &dt, 1, 0XFF );
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY);

	return dt;
}


unsigned int getCo2(){
	return (CO2);
}

unsigned int gettVOC(){
	return (tVOC);
}
