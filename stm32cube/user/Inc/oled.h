#ifndef __OLED_H
#define __OLED_H

#include "i2c.h"

/* OLED�����ú��� */
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void OLED_On(void);

/* OLED���ܺ��� */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y, char *chr,uint8_t size);	 
void OLED_Show_Float_Num(unsigned int x,unsigned int y,double num, uint8_t size);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);

/* OLED��ʼ�� */
void OLED_Init(void);

#endif  
