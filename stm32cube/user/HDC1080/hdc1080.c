#include "hdc1080.h"
#include "i2c.h"

static double temperature = 0;
static double humidity = 0;

void hdc1080_init(void)
{
	/* Temperature and Humidity are acquired in sequence, Temperature first
	 * Default:   Temperature resolution = 14 bit,
	 *            Humidity resolution = 14 bit
	 */

	/* Set the acquisition mode to measure both temperature and humidity by setting Bit[12] to 1 */
	uint16_t config_reg_value=0x1000;
	uint8_t data_send[2];

	config_reg_value |= (1<<10); //11 bit
	config_reg_value|= (1<<8);


	data_send[0]= (config_reg_value>>8);
	data_send[1]= (config_reg_value&0x00ff);

	HAL_I2C_Mem_Write(&hi2c2,HDC_1080_ADD<<1,Configuration_register_add,I2C_MEMADD_SIZE_8BIT,data_send,2,1000);
	hdc1080_start_measurement();
}


uint8_t hdc1080_start_measurement(void)
{
	uint8_t receive_data[4];
	uint16_t temp_x,humi_x;
	uint8_t send_data = Temperature_register_add;

	HAL_I2C_Master_Transmit(&hi2c2,HDC_1080_ADD<<1,&send_data,1,1000);

	/* Delay here 15ms for conversion compelete.
	 * Note: datasheet say maximum is 7ms, but when delay=7ms, the read value is not correct
	 */
	HAL_Delay(15);

	/* Read temperature and humidity */
	HAL_I2C_Master_Receive(&hi2c2,HDC_1080_ADD<<1,receive_data,4,1000);

	temp_x =((receive_data[0]<<8)|receive_data[1]);
	humi_x =((receive_data[2]<<8)|receive_data[3]);

	temperature=((temp_x/65536.0)*165.0)-40.0;
	humidity=(uint8_t)((humi_x/65536.0)*100.0);

	return 0;

}

double gettemperature(void){
	return temperature;
}

double gethumidity(void){
	return humidity;
}
