#include "CCS811.h"
#include "OLED.h"
u8 BUF[12];
u8 Information[10];
u8 MeasureMode, Status, Error_ID;
u8 SetMeasureMode = 0;
u8 FlagGetId = 1;
u8 n         = 4;                                        // 3次读取ID都对则说明没问题
u8 temp      = 0x5a;
ccs811_measurement_t CCS;

#define STEP_DELAY 10

void CCS811_Reset(void)
{
    CCS811_RST_OUT = 0;
    delay_ms(300);
    CCS811_RST_OUT = 1;
    delay_ms(100);
}
int CCS811_Init()
{
	u8 idCount = 0;
    u8 FailureTimes = 0;
	CCS811_nWAKE_ON();
	delay_ms(STEP_DELAY);
	
	while(idCount < n)
	{
		CCS811_ReadI2C(CCS811_Add, 0x20, Information, 1);
		if(Information[0] == 0x81)
		{
			idCount++;
		}
		else
		{
            FailureTimes++;
            if (FailureTimes > 200)
            {
                return -1;  /* ID 读取失败，建议复位模块 */
            }
            
		}
		delay_ms(STEP_DELAY);
	}
	delay_ms(STEP_DELAY);
	CCS811_ReadI2C(CCS811_Add, 0x23, &Information[1], 2);	  //FW_Boot_Version
	delay_ms(STEP_DELAY);
	CCS811_ReadI2C(CCS811_Add, 0x24, &Information[3], 2); 	//FW_App_Version
	delay_ms(STEP_DELAY);
	while (CCS811_ReadI2C(CCS811_Add, 0x00, &Status, 1) != 1)
    {
        delay_ms(STEP_DELAY);
        FailureTimes++;
        if (FailureTimes > 200)
        {
            CCS811_nWAKE_OFF();
            return -2;  /* 读取状态失败，建议复位模块 */
        }
    }
	delay_ms(STEP_DELAY);
	
	if(Status & 0x10)
	{
		while(!(Status & 0x80))
		{
            FailureTimes = 0;
			while (CCS811_WriteI2C_byte(CCS811_Add, 0xF3, 0xF0) != 1)
            {
                delay_ms(STEP_DELAY);
                FailureTimes++;
                if (FailureTimes > 200)
                {
                    CCS811_nWAKE_OFF();
                    return -3;  /* 应用程序校验失败 */
                }
            }
            FailureTimes = 0;
			while (CCS811_MWriteI2C_byte(CCS811_Add, 0xF4, &temp, 0) != 1)
            {
                delay_ms(STEP_DELAY);
                FailureTimes++;
                if (FailureTimes > 200)
                {
                    CCS811_nWAKE_OFF();
                    return -4;  /* 切换到应用程序失败 */
                }
            }
			delay_ms(STEP_DELAY);
			while (CCS811_ReadI2C(CCS811_Add, 0x00, &Status, 1) != 1)
            {
                delay_ms(STEP_DELAY);
                FailureTimes++;
                if (FailureTimes > 200)
                {
                    CCS811_nWAKE_OFF();
                    return -2;  /* 读取状态失败，建议复位模块 */
                }
            }
			delay_ms(STEP_DELAY);

		}
	}
    #if 0
	delay_ms(STEP_DELAY);
	CCS811_ReadI2C(CCS811_Add, 0x01, &MeasureMode, 1);
	delay_ms(STEP_DELAY);                           // get measure mode
	//if measure mode incorrect,and reset the measure mode.
	while(MeasureMode != 0x18)
	{
		CCS811_WriteI2C_byte(CCS811_Add, 0x01, 0x18); // Write Measure Mode Register,sensor measurement every second,no interrupt
		delay_ms(STEP_DELAY);
		CCS811_ReadI2C(CCS811_Add, 0x01, &MeasureMode, 1);
		delay_ms(STEP_DELAY);
	}
    #endif
	delay_ms(STEP_DELAY);
	while (CCS811_ReadI2C(CCS811_Add, 0x00, &Status, 1) != 1)
    {
        delay_ms(STEP_DELAY);
        FailureTimes++;
        if (FailureTimes > 200)
        {
            CCS811_nWAKE_OFF();
            return -2;  /* 读取状态失败，建议复位模块 */
        }
    }
	delay_ms(STEP_DELAY);
	while (CCS811_ReadI2C(CCS811_Add, 0x01, &MeasureMode, 1) != 1)
    {
        delay_ms(STEP_DELAY);
        FailureTimes++;
        if (FailureTimes > 200)
        {
            CCS811_nWAKE_OFF();
            return -5;  /* 读取测试模式失败 */
        }
    }
	delay_ms(STEP_DELAY);
	CCS811_nWAKE_OFF();
    return 0;
}

uint8_t CCS811_ChangeMeasureMode(uint8_t mode)
{
    uint8_t FailureTimes = 0;
    mode &= 0x7C;
    SetMeasureMode = mode;
	CCS811_nWAKE_ON();
	delay_ms(STEP_DELAY);
	while(MeasureMode != mode)
	{
        FailureTimes = 0;
        while (CCS811_WriteI2C_byte(CCS811_Add, 0x01, mode) != 1)
        {
            delay_ms(STEP_DELAY);
            FailureTimes++;
            if (FailureTimes > 200)
            {
                CCS811_nWAKE_OFF();
                return 0;
            }
        }
		delay_ms(STEP_DELAY);
		while (CCS811_ReadI2C(CCS811_Add, 0x01, &MeasureMode, 1) != 1)
        {
            delay_ms(STEP_DELAY);
            FailureTimes++;
            if (FailureTimes > 200)
            {
                CCS811_nWAKE_OFF();
                return 0;
            }
        }
		delay_ms(STEP_DELAY);
	}
	delay_ms(STEP_DELAY);
	CCS811_nWAKE_OFF();
    return MeasureMode;
}

int CCS811_GetData()
{
    uint8_t FailureTimes = 0;
	CCS811_nWAKE_ON();
	delay_ms(STEP_DELAY);
	while (CCS811_ReadI2C(CCS811_Add, 0x00, &Status, 1) != 1)
    {
        delay_ms(STEP_DELAY);
        FailureTimes++;
        if (FailureTimes > 200)
        {
            CCS811_nWAKE_OFF();
            return -1;  /* 读取状态失败，建议复位模块 */
        }
    }
    delay_ms(STEP_DELAY);
    if ((Status & 0xF7) != 0x90)
    {
        CCS811_nWAKE_OFF();
        return -2;      /* 状态错误 */
    }
    delay_ms(STEP_DELAY);
    while (CCS811_ReadI2C(CCS811_Add, 0x01, &MeasureMode, 1) != 1)
    {
        delay_ms(STEP_DELAY);
        FailureTimes++;
        if (FailureTimes > 200)
        {
            CCS811_nWAKE_OFF();
            return -1;
        }
    }
    if (MeasureMode != SetMeasureMode)
    {
            CCS811_nWAKE_OFF();
            return -3;      /* 状态错误 */
    }
	delay_ms(STEP_DELAY);
    if ((MeasureMode & 0x80) != 0)
    {
        if (CCS811_INT_IN == 0)
        {
            CCS811_ReadI2C(CCS811_Add, 0x02, BUF, 8);
        }
    }
    else if ((Status & 0x08) != 0)
    {
        CCS811_ReadI2C(CCS811_Add, 0x02, BUF, 8);
    }
    else
    {
        CCS811_nWAKE_OFF();
        return 0;
    }
	delay_ms(STEP_DELAY);
    while (CCS811_ReadI2C(CCS811_Add, 0x20, Information, 1) != 1)
    {
        delay_ms(STEP_DELAY);
        FailureTimes++;
        if (FailureTimes > 200)
        {
            CCS811_nWAKE_OFF();
            return -1;  /* 读取模块ID失败，建议复位模块 */
        }
    }
    if (Information[0] != 0x81)
    {
        CCS811_nWAKE_OFF();
        return -1;      /* 状态错误 */
    }
	CCS811_nWAKE_OFF();
	CCS.eco2 = (u16)BUF[0] * 256 + BUF[1];
	CCS.tvoc = (u16)BUF[2] * 256 + BUF[3];
	CCS.device_id  = Information[0];
	CCS.error_id   = Error_ID;
	Error_ID       = 0;
	Information[0] = 0;
    return 0;
}
