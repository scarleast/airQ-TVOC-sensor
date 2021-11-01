#include "oled.h"
#include "oledfont.h"
#include "stdio.h"

static void OLED_Write_Cmd(uint8_t cmd)
{
	uint8_t buf[2];
	buf[0] = 0x00;	//control byte
	buf[1] = cmd;
	
	//ʹ��HAL���APIʵ��
	HAL_I2C_Master_Transmit(&hi2c2, 0x78, buf, 2, 0xFFFF);
}
/**
 * @brief	OLEDд������
 * @param cmd - ��д������
 * @note	��ֲʱ����ʹ���Լ��ĵײ�APIʵ�� 
*/
static void OLED_Write_Dat(uint8_t dat)
{
	uint8_t buf[2];
	buf[0] = 0x40; //control byte
	buf[1] = dat;
	
	//ʹ��HAL���APIʵ��
	HAL_I2C_Master_Transmit(&hi2c2, 0x78, buf, 2, 0xFFFF);
}

/**
 * @brief	OLED������ʾλ��
 * @param x - X����λ��
 * @param y - Y����λ��
*/
void OLED_Set_Pos(uint8_t x, uint8_t y)
{ 	
	OLED_Write_Cmd(0xb0+y);
	OLED_Write_Cmd(((x&0xf0)>>4)|0x10);
	OLED_Write_Cmd((x&0x0f)|0x01);
}   	  
/**
 * @brief	OLED������ʾ
*/ 
void OLED_Display_On(void)
{
	OLED_Write_Cmd(0X8D);  //SET DCDC����
	OLED_Write_Cmd(0X14);  //DCDC ON
	OLED_Write_Cmd(0XAF);  //DISPLAY ON
}
/**
 * @brief	OLED�ر���ʾ
*/   
void OLED_Display_Off(void)
{
	OLED_Write_Cmd(0X8D);  //SET DCDC����
	OLED_Write_Cmd(0X10);  //DCDC OFF
	OLED_Write_Cmd(0XAE);  //DISPLAY OFF
}		   			 
/**
 * @brief	OLED��������������֮����ĻȫΪ��ɫ��
*/ 
void OLED_Clear(void)
{  
	uint8_t i,n;
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Cmd(0xb0+i);    //����ҳ��ַ��0~7��
		OLED_Write_Cmd(0x00);      //������ʾλ�á��е͵�ַ
		OLED_Write_Cmd(0x10);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
		{
			OLED_Write_Dat(0);
		}			
	}
}
/**
 * @brief	OLED��ʾȫ�����������ص�ȫ����
*/
void OLED_On(void)
{  
	uint8_t i,n;
	for(i=0;i<8;i++)
	{  
		OLED_Write_Cmd(0xb0+i);    //����ҳ��ַ��0~7��
		OLED_Write_Cmd(0x00);      //������ʾλ�á��е͵�ַ
		OLED_Write_Cmd(0x10);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
		{
			OLED_Write_Dat(1);
		}			
	}
}
/**
 * @brief	��ָ��λ����ʾһ��ASCII�ַ�
 * @param x - 0 - 127
 * @param y - 0 - 7
 * @param chr  - ����ʾ��ASCII�ַ�
 * @param size - ASCII�ַ���С
 * 				�ַ���С��12(6*8)/16(8*16)���ִ�С
*/
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size)
{   
	uint8_t c=0,i=0;
	
	c = chr-' ';	
	if(x > 128-1)
	{
		x=0;
		y++;
	}
	
	if(size ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
		{
			OLED_Write_Dat(F8X16[c*16+i]);
		}
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		{
			OLED_Write_Dat(F8X16[c*16+i+8]);
		}
	}
	else
	{	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		{
			OLED_Write_Dat(F6x8[c][i]);
		}
	}
}
/**
 * @brief	OLED ר��pow����
 * @param m - ����
 * @param n - ָ��
*/
static uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)result*=m;    
	return result;
}	
/**
 * @brief	��ָ��λ����ʾһ������
 * @param x - 0 - 127
 * @param y - 0 - 7
 * @param num - ����ʾ������(0-4294967295)
 * @param	len - ���ֵ�λ�� 
 * @param size - ASCII�ַ���С
 * 				�ַ���С��12(6*8)/16(8*16)���ִ�С
*/
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size); 
	}
} 
/**
 * @brief	��ָ��λ����ʾһ���ַ���
 * @param x - 0 - 127
 * @param y - 0 - 7
 * @param chr - ����ʾ���ַ���ָ��
 * @param size - ASCII�ַ���С
 * 				�ַ���С��12(6*8)/16(8*16)���ִ�С
*/
void OLED_ShowString(uint8_t x,uint8_t y,char *chr,uint8_t size)
{
	uint8_t j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],size);
			x+=8;
			if(x>120){x=0;y+=2;}
			j++;
	}
}


void OLED_Show_Float_Num(unsigned int x,unsigned int y,double num ,uint8_t size)
{
	uint32_t B;

	char Num[] = {'\0','\0','\0','\0' ,'\0', '\0'};
	//符号位  0    0    0     .    0    0    0     0

	if(num < 0)//负数时
	{
	num = -num;//将数值转化为正数进行处理
	}

	if(num >= 100)//数值限幅，设置上限
	{
		num = 99.99;
	}

	B = (uint32_t)(num * 100);//将小数为转化为整数

	Num[4] = B % 10 + '0';
	B = B/10;
	Num[3] = B % 10 + '0';
	B = B/10;
	Num[2] = '.';
	Num[1] = B % 10 + '0';
	B = B/10;
	Num[0] = B % 10 + '0';

//	for(uint8_t i=4;i>=0;i--)
//	{
//		if(i != 2)//如果i不是小数位时，这开始拆分
//		{
//			Num[i] = B % 10 + '0';;//由于Num数组是char类型，因此加'0'，使其变为例如'1'、'23'之类的字符
//			B = B/10;//去掉最低位
//		}
//		else//当遍历到小数位时，则填充小数点
//		{
//			Num[2] = '.';
//		}
//		OLED_ShowChar(0,i*2,Num[i], 12);//该函数为中景园OLED的库
//	}

	OLED_ShowString(x,y,Num, size);//该函数为中景园OLED的库
}

//显示汉字
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{
 uint8_t t,adder=0;
 OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
 {
    	OLED_Write_Dat(Hzk[2*no][t]);
  adder+=1;
 }
 OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
 {
    	OLED_Write_Dat(Hzk[2*no+1][t]);
  adder+=1;
 }
}


/**
 * @brief	OLED��ʼ��
*/			    
void OLED_Init(void)
{ 	
	OLED_Write_Cmd(0xAE);//--display off
	OLED_Write_Cmd(0x00);//---set low column address
	OLED_Write_Cmd(0x10);//---set high column address
	OLED_Write_Cmd(0x40);//--set start line address  
	OLED_Write_Cmd(0x81); // contract control
	OLED_Write_Cmd(0xFF);//--128   
	OLED_Write_Cmd(0xA1);//set segment remap 
	OLED_Write_Cmd(0xC8);//Com scan direction
	OLED_Write_Cmd(0xA6);//--normal / reverse
	OLED_Write_Cmd(0xA8);//--set multiplex ratio(1 to 64)
	OLED_Write_Cmd(0x3F);//--1/32 duty
	OLED_Write_Cmd(0xD3);//-set display offset
	OLED_Write_Cmd(0x00);//
	OLED_Write_Cmd(0xD5);//set osc division
	OLED_Write_Cmd(0x80);
	OLED_Write_Cmd(0xD9);//Set Pre-Charge Period
	OLED_Write_Cmd(0xF1);//
	OLED_Write_Cmd(0xDA);//set com pin configuartion
	OLED_Write_Cmd(0x12);//
	OLED_Write_Cmd(0xDB);//set Vcomh
	OLED_Write_Cmd(0x40);//
	OLED_Write_Cmd(0x20);
	OLED_Write_Cmd(0x02);
	OLED_Write_Cmd(0x8D);//set charge pump enable
	OLED_Write_Cmd(0x14);//
	OLED_Write_Cmd(0xA4);
	OLED_Write_Cmd(0xA6);
	OLED_Write_Cmd(0xAF);//--turn on oled panel
	
	OLED_Clear();
	OLED_Set_Pos(0,0);
}
