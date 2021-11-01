#include "stm32f10x.h"
#include "init.h"
#include "delay.h"
#include "oled.h"
#include "CCS811.h"
//#include "IIC.h"
#include "CCS811_IIC.h"

int mode = 0;

extern ccs811_measurement_t CCS;

void DisplayOnOled(void);
void DisplayWait(uint8_t i);
void DisplayFrame(void);

int main()
{
    __IO int j;
    __IO int CCS811_FailureTimes = 0;
    __IO int CCS881_ReadDataResult = 0;
    init();
    OLED_Init();
    delay_ms(300);
    if (CCS811_Init())
    {
        CCS811_Reset();
        if (CCS811_Init())
        {
            LED1 = 0;
        }
        else
        {
            LED1 = 1;
        }
    }
    else
    {
        LED1 = 1;
    }
    LED3 = 0;
    DisplayFrame();
    delay_ms(100);
    while(1)
    {
        if (mode == 0)      /* 传感器初始化过程 */
        {
            LED2 = !LED2;
            delay_ms(100);
            DisplayWait(j++);
            if (j == 6)
            {
                j = 0;
            }
            CCS881_ReadDataResult = CCS811_GetData();

            if (CCS881_ReadDataResult == 0)
            {
                if ( CCS.eco2 > 0 && CCS.eco2 < 60000)
                {
                    mode = 1;
                    LED1 = 1;
                    LED2 = 0;
                    CCS811_FailureTimes = 0;
                }
            }
            else if (CCS881_ReadDataResult == -1 || CCS881_ReadDataResult == -2)
            {
                CCS811_FailureTimes++;
                if (CCS811_FailureTimes > 50)
                {
                    CCS811_FailureTimes = 0;
                    CCS811_Reset();
                    if (CCS811_Init())
                    {
                        LED1 = 0;
                    }
                    else
                    {
                        LED1 = 1;
                    }
                }
            }
            else if (CCS881_ReadDataResult == -3)
            {
                CCS811_ChangeMeasureMode(0x18);
            }
        }
        else if (mode == 1)
        {
            CCS881_ReadDataResult = CCS811_GetData();
            if (CCS881_ReadDataResult == 0)         /* 读取传感器数据成功 */
            {
                DisplayOnOled();
                CCS811_FailureTimes = 0;
            }
            else if (CCS881_ReadDataResult == -1)   /* 传感器异常 */
            {
                CCS811_FailureTimes++;
                if (CCS811_FailureTimes>10)
                {
                    CCS811_Reset(); 
                    if (CCS811_Init())
                    {
                        LED1 = 0;
                    }
                    else
                    {
                        LED1 = 1;
                    }
                    mode = 0;
                    LED1 = 0;
                    CCS811_FailureTimes = 0;
                }
            }
            else if (CCS881_ReadDataResult == -2)   /* 传感器状态异常 */
            {
                CCS811_FailureTimes++;
                if (CCS811_FailureTimes>10)
                {
                    CCS811_Reset(); 
                    if (CCS811_Init())
                    {
                        LED1 = 0;
                    }
                    else
                    {
                        LED1 = 1;
                    }
                    mode = 0;
                    LED1 = 0;
                    CCS811_FailureTimes = 0;
                }
            }
            delay_ms(100);
        }
    }
}

void DisplayFrame(void)
{
    OLED_ShowString(0, 0, "eCO", 16);
    OLED_ShowString(0, 2, "TVOC:", 16);
    OLED_ShowChar(24, 1, '2', 8);
    OLED_ShowChar(32, 0, ':', 16);
    OLED_ShowString(80, 0, "(ppm) ", 16);
    OLED_ShowString(80, 2, "(ppb) ", 16);
}

void DisplayOnOled()
{
    OLED_ShowNum(40, 0, CCS.eco2, 5, 16);
    OLED_ShowNum(40, 2, CCS.tvoc, 5, 16);
}

void DisplayWait(uint8_t i)
{
    i= i%6;
    int16_t temp;
    for (temp = i; temp < 5; temp++)
    {
        OLED_ShowChar(40 + (temp << 3), 0, ' ', 16);
        OLED_ShowChar(40 + (temp << 3), 2, ' ', 16);
    }
    for (temp = i-1; temp > -1; temp--)
    {
        OLED_ShowChar(40 + (temp << 3), 0, '.', 16);
        OLED_ShowChar(40 + (temp << 3), 2, '.', 16);
    }
}
