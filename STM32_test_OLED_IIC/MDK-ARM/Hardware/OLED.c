#include "main.h"
#include "OLEDFONT.h"
#include "i2c.h"
#include "OLED.h"

void WriteCmd(unsigned char I2C_Command)//写命??????????
{
	HAL_I2C_Mem_Write(&hi2c1,OLED0561_ADD,COM,I2C_MEMADD_SIZE_8BIT,&I2C_Command,1,100);
}

void WriteDat(unsigned char I2C_Data)//写数??????????
{
	HAL_I2C_Mem_Write(&hi2c1,OLED0561_ADD,DAT,I2C_MEMADD_SIZE_8BIT,&I2C_Data,1,100);
}

void OLED_Init(void)
{
	HAL_Delay(100); //这里的延时很重要

	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐??????????
{
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			WriteDat(fill_Data);
	}
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷??????????
	WriteCmd(0X14);  //??????????启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷??????????
	WriteCmd(0X10);  //关闭电荷??????????
	WriteCmd(0XAE);  //OLED休眠
}

/*
 * x,y初始坐标
 * num?????显示的uint32型数
 * lenth?????要显示的数字长度，从低位向高位数
 * size暂未制作
 * */
void OLED_ShowLagData(uint8_t x,uint8_t y,uint32_t num,uint8_t lenth,uint8_t size)
{
	uint8_t j=lenth,i=0,c=0;
	uint8_t GetData[10];
	uint32_t CopeData=num;
	for(i=0;i<lenth;i++)
	{
		GetData[i]=CopeData%10;
		CopeData=CopeData/10;
	}
	while(j>0)
	{
		c=GetData[j-1]+16;
		if(x> 126)//换行
		{
			x = 0;
			y++;
		}
			OLED_SetPos(x,y);//setInte x y

		for(i=0;i<6;i++)
			WriteDat(F6x8[c][i]);

					//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
					//HAL_Delay(100);
		x=x+6;
		j--;
	}
}


// Parameters     : x,y -- 起始点坐??????????(x:0~127, y:0~7); ch[] -- 要显示的字符??????????; TextSize -- 字符大小(1:6*8 ; 2:8*16) lenth是字符串字数
// Description    : 显示codetab.h中的ASCII字符,??????????6*8??????????8*16可???择
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[],unsigned char lenth, unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(j<lenth)
			{
				c = ch[j] - 32;
				if(x> 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					{WriteDat(F6x8[c][i]);}
				x=x+6;
				j++;
			}
		}break;
		case 2:
		{
			while(j<lenth)
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

// Parameters     : x,y -- 起始点坐??????????(x:0~127, y:0~7); N:汉字??????????.h中的索引
// Description    : 显示ASCII_8x16.h中的汉字,16*16点阵
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}


// Parameters     : x0,y0 -- 起始点坐??????????(x0:0~127, y0:0~7); x1,y1 -- 起点对角??????????(结束??????????)的坐??????????(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}

void OLED_ShowChar(uint8_t x,uint8_t y,uint32_t chr,uint8_t Char_Size)
{
	unsigned char c=0,i=0;
		c=chr-' ';//得到偏移后的
		if(x>128-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_SetPos(x,y);
			for(i=0;i<8;i++)
			WriteDat(F8X16[c*16+i]);
			OLED_SetPos(x,y+1);
			for(i=0;i<8;i++)
			WriteDat(F8X16[c*16+i+8]);
			}
			else {
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
				WriteDat(F6x8[c][i]);

			}
}
unsigned int oled_pow(unsigned int m,unsigned int n)
{
	unsigned int result=1;
	while(n--)result*=m;
	return result;
}
