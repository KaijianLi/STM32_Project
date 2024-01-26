#include "OLED_IIC.h"
#include "gpio.h"
#include "OLED_FONT.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

#define OLED_SCL_OUT_HIGH()		HAL_GPIO_WritePin(OLED_SCL_GPIO_Port,OLED_SCL_Pin,GPIO_PIN_SET)
#define OLED_SCL_OUT_LOW()      HAL_GPIO_WritePin(OLED_SCL_GPIO_Port,OLED_SCL_Pin,GPIO_PIN_RESET)
#define OLED_SDA_OUT_HIGH()     HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,GPIO_PIN_SET)
#define OLED_SDA_OUT_LOW()		HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,GPIO_PIN_RESET)
#define OLED_SDA_Write(X)   	HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,(X?GPIO_PIN_SET:GPIO_PIN_RESET))

unsigned char OLED_GRAM[144][8];

// HAL libiary to achieve us delay
void delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 8000000 * us);
    while (delay--)
	{
		;
	}
}

// analog IIC pin init
void OLED_I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
	
    GPIO_InitStruct.Pin = OLED_SCL_Pin|OLED_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	OLED_SCL_OUT_HIGH();
	OLED_SDA_OUT_HIGH();
}

// start IIC communication
void OLED_I2C_Start(void)
{
	OLED_SDA_OUT_HIGH();
	OLED_SCL_OUT_HIGH();
	delay_us(1);
	OLED_SDA_OUT_LOW();
	delay_us(1);
	OLED_SCL_OUT_LOW();
	delay_us(1);
}

// stop the IIC communication
void OLED_I2C_Stop(void)
{
	OLED_SDA_OUT_LOW();
	OLED_SCL_OUT_LOW();
	
	OLED_SCL_OUT_HIGH();
	delay_us(1);
	OLED_SDA_OUT_HIGH();
	delay_us(1);
}

/**
  * @brief  use IIC to send a byte
  * @param  Byte the byte to send
  * @retval None
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		GPIO_PinState send_temp = (GPIO_PinState)(Byte&(0x80 >> i));
		OLED_SDA_Write(send_temp);
		OLED_SCL_OUT_HIGH();
		delay_us(1);
		OLED_SCL_OUT_LOW();
	}
	OLED_SCL_OUT_HIGH();	//额外的一个时钟，不处理应答信号
	delay_us(1);
	OLED_SCL_OUT_LOW();
}

/**
  * @brief  OLED write a command
  * @param  Command the command to write, a HEX data
  * @retval None
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x00);		//写命令
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLED write data
  * @param  Data the data to write,a HEX data
  * @retval None
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x40);		//写数据
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED set the position of cursor
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval None
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}

// refresh the OLED_GRAM,only for display the picture
void OLED_Refresh(void)
{
	unsigned char i,n;
	for(i = 0;i < 8;i++)
	{
		OLED_SetCursor(i,0);
	    for(n=0;n<128;n++)
	        OLED_WriteData(OLED_GRAM[n][i]);
  }
}

/**
  * @brief  OLED clear all
  * @retval None
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLED填充函数
  * @retval 无
  */
void OLED_Fill(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0xFF);
		}
	}
}

/**
  * @brief  OLED画点
  * @param  row 行坐标,范围: 0~63
  * @param  col 列坐标,范围: 0~123
  * @param  mode 
		@arg 1:填充
		@arg 0:清除
  * @retval 无
  */
void OLED_DrawPoint(uint8_t row,uint8_t col,uint8_t mode)
{
	uint8_t i,m,n;
	i = row/8;
	m = row%8;
	n = 1 << m;
	if(mode == 1)	
		OLED_GRAM[col][i] |= n;
	else
		OLED_GRAM[col][i]  &= ~n;
}

/**
  * @brief  OLED画直线
  * @param  start_row 起点行坐标
  * @param  start_col 起点列坐标
  * @param  end_row   终点行坐标
  * @param  end_col   终点列坐标
  * @retval 无
  */
void OLED_DrawLine(uint8_t start_row,uint8_t start_col,uint8_t end_row,uint8_t end_col)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_row,delta_col,distance;
	int incrow,inccol,uRow,uCol;
	
	
	delta_row = end_row - start_row; //计算坐标增量 
	delta_col = end_col - start_col;
	
	uRow = start_row;//画线起点坐标
	uCol = start_col;
	
	if(delta_col > 0)
		inccol = 1; //设置单步方向 
	else if(delta_col == 0)
		inccol = 0;//垂直线 
	else 
    {
		inccol = -1;
		delta_col = -delta_col;
	}
	
	if(delta_row > 0)
		incrow = 1;
	else if(delta_row == 0)
		incrow = 0;//水平线 
	else 
    {
		incrow = -1;
		delta_row = -delta_row;
	}
	
	if(delta_col > delta_row)
		distance = delta_col; //选取基本增量坐标轴 
	else 
		distance = delta_row;
	
	for(t=0;t<distance+1;t++)
	{
		OLED_DrawPoint(uRow,uCol,1);//画点
		xerr+=delta_col;
		yerr+=delta_row;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=inccol;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incrow;
		}
	}
}

/**
  * @brief  OLED画圆函数
  * @param  row 圆心的行坐标，范围 0~63
  * @param  col 圆心的列坐标，范围 0~127
  * @param  r   圆的直径
  * @retval None
  */
void OLED_DrawCircle(uint8_t row,uint8_t col,uint8_t r)
{
	int a = 0, b = r;
    while(a <= (int)(r/sqrt(2)) + 1)      
    {
        OLED_DrawPoint(row + a, col - b,1);
        OLED_DrawPoint(row - a, col - b,1);
        OLED_DrawPoint(row - a, col + b,1);
        OLED_DrawPoint(row + a, col + b,1);
 
        OLED_DrawPoint(row + b, col + a,1);
        OLED_DrawPoint(row + b, col - a,1);
        OLED_DrawPoint(row - b, col - a,1);
        OLED_DrawPoint(row - b, col + a,1);
        
        a++;
        b = (int)sqrt(r * r - a * a);
    }
}

/**
  * @brief  OLED显示一个字符
  * @param  Line   行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示图片
  * @param  row 左上角起始点行坐标
  * @param  col 左上角起始点列坐标
  * @param  sizex 图片宽度
  * @param  sizey 图片高度
  * @param  BMP   保存图片的数组
  * @param  mode
		@arg 0：反色显示
		@arg 1：正常显示
  * @retval 无
  */
void OLED_ShowPicture(uint8_t row,uint8_t col,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode)
{
	unsigned int j=0;
	unsigned char i,n,temp,m;
	unsigned char x0 = col,y0 = row;
	sizey = sizey / 8 + ((sizey%8)?1:0);
	for(n=0;n<sizey;n++)
	{
	  for(i=0;i<sizex;i++)
	  {
		temp = BMP[j];
		for(m=0;m<8;m++)
		{
			if(temp&0x01) 
				OLED_DrawPoint(row,col,mode);
			else 
				OLED_DrawPoint(row,col,!mode);
			temp>>=1;
			row++;
		}
		col++;
		if((col-x0)==sizex)
		{
			col = x0;
			y0  += 8;
		}
		row = y0;
		j++;
	}
  }
}

/**
  * @brief  OLED显示镜像图片
  * @param  row 左上角起始点行坐标
  * @param  col 左上角起始点列坐标
  * @param  sizex 图片宽度
  * @param  sizey 图片高度
  * @param  BMP   保存图片的数组
  * @param  mode
		@arg 0：反色显示
		@arg 1：正常显示
  * @retval 无
  */
void OLED_ShowMirrorPic(uint8_t row,uint8_t col,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode)
{
	unsigned int j=0;
	unsigned char i,n,temp,m;
	unsigned char x0 = col,y0 = row;
	uint8_t diad = col + sizex / 2;
	sizey = sizey / 8 + ((sizey%8)?1:0);
	for(n=0;n<sizey;n++)
	{
	  for(i=0;i<sizex;i++)
	  {
		temp = BMP[j];
		for(m=0;m<8;m++)
		{
			if(temp&0x01) 
				OLED_DrawPoint(row,2 * diad - col,mode);
			else 
				OLED_DrawPoint(row,2 * diad - col,!mode);
			temp>>=1;
			row++;
		}
		col++;
		if((col-x0)==sizex)
		{
			col = x0;
			y0  += 8;
		}
		row = y0;
		j++;
	}
  }
}
/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
unsigned int OLED_Pow(uint32_t X, uint32_t Y)
{
	unsigned int Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number)
{
	char length = 0;
	int num_temp = Number;
	while(num_temp != 0)
	{
		length += 1;
		num_temp /= 10;
	}
	uint8_t i;
	for (i = 0; i < length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示浮点数
  * @param  Line   起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的浮点数
  * @retval 无
  */
void OLED_ShowFloatNum(uint8_t Line,uint8_t Column,float FloatNum)
{
	// the float number: -xxx.xx ~ xxx.xx
	char Data[10];             //Create the target array, used to store the converted character data
    sprintf(Data,"%.2f",FloatNum);    //Two decimal places after the decimal point,save in Data array
	unsigned char length = strlen(Data);
	for(uint8_t i = length;i < 10 - length;i++)
		Data[i] = ' ';
	Data[10 - length] = '\0';
	OLED_ShowString(Line,Column,Data);    //call OLED String display function，display on the OLED
	memset(Data,0,sizeof(Data));
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	
	char length = 0;
	int num_temp = Number1;
	while(num_temp != 0)
	{
		length += 1;
		num_temp /= 10;
	}
	
	for (i = 0; i < length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//端口初始化
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}
