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
	OLED_SCL_OUT_HIGH();	//�����һ��ʱ�ӣ�������Ӧ���ź�
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
	OLED_I2C_SendByte(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte(0x00);		//д����
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
	OLED_I2C_SendByte(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte(0x40);		//д����
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED set the position of cursor
  * @param  Y �����Ͻ�Ϊԭ�㣬���·�������꣬��Χ��0~7
  * @param  X �����Ͻ�Ϊԭ�㣬���ҷ�������꣬��Χ��0~127
  * @retval None
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//����Yλ��
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//����Xλ�ø�4λ
	OLED_WriteCommand(0x00 | (X & 0x0F));			//����Xλ�õ�4λ
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
  * @brief  OLED��亯��
  * @retval ��
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
  * @brief  OLED����
  * @param  row ������,��Χ: 0~63
  * @param  col ������,��Χ: 0~123
  * @param  mode 
		@arg 1:���
		@arg 0:���
  * @retval ��
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
  * @brief  OLED��ֱ��
  * @param  start_row ���������
  * @param  start_col ���������
  * @param  end_row   �յ�������
  * @param  end_col   �յ�������
  * @retval ��
  */
void OLED_DrawLine(uint8_t start_row,uint8_t start_col,uint8_t end_row,uint8_t end_col)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_row,delta_col,distance;
	int incrow,inccol,uRow,uCol;
	
	
	delta_row = end_row - start_row; //������������ 
	delta_col = end_col - start_col;
	
	uRow = start_row;//�����������
	uCol = start_col;
	
	if(delta_col > 0)
		inccol = 1; //���õ������� 
	else if(delta_col == 0)
		inccol = 0;//��ֱ�� 
	else 
    {
		inccol = -1;
		delta_col = -delta_col;
	}
	
	if(delta_row > 0)
		incrow = 1;
	else if(delta_row == 0)
		incrow = 0;//ˮƽ�� 
	else 
    {
		incrow = -1;
		delta_row = -delta_row;
	}
	
	if(delta_col > delta_row)
		distance = delta_col; //ѡȡ�������������� 
	else 
		distance = delta_row;
	
	for(t=0;t<distance+1;t++)
	{
		OLED_DrawPoint(uRow,uCol,1);//����
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
  * @brief  OLED��Բ����
  * @param  row Բ�ĵ������꣬��Χ 0~63
  * @param  col Բ�ĵ������꣬��Χ 0~127
  * @param  r   Բ��ֱ��
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
  * @brief  OLED��ʾһ���ַ�
  * @param  Line   ��λ�ã���Χ��1~4
  * @param  Column ��λ�ã���Χ��1~16
  * @param  Char Ҫ��ʾ��һ���ַ�����Χ��ASCII�ɼ��ַ�
  * @retval ��
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//���ù��λ�����ϰ벿��
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//��ʾ�ϰ벿������
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//���ù��λ�����°벿��
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//��ʾ�°벿������
	}
}

/**
  * @brief  OLED��ʾͼƬ
  * @param  row ���Ͻ���ʼ��������
  * @param  col ���Ͻ���ʼ��������
  * @param  sizex ͼƬ���
  * @param  sizey ͼƬ�߶�
  * @param  BMP   ����ͼƬ������
  * @param  mode
		@arg 0����ɫ��ʾ
		@arg 1��������ʾ
  * @retval ��
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
  * @brief  OLED��ʾ����ͼƬ
  * @param  row ���Ͻ���ʼ��������
  * @param  col ���Ͻ���ʼ��������
  * @param  sizex ͼƬ���
  * @param  sizey ͼƬ�߶�
  * @param  BMP   ����ͼƬ������
  * @param  mode
		@arg 0����ɫ��ʾ
		@arg 1��������ʾ
  * @retval ��
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
  * @brief  OLED��ʾ�ַ���
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  String Ҫ��ʾ���ַ�������Χ��ASCII�ɼ��ַ�
  * @retval ��
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
  * @brief  OLED�η�����
  * @retval ����ֵ����X��Y�η�
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
  * @brief  OLED��ʾ���֣�ʮ���ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~4294967295
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
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
  * @brief  OLED��ʾ������
  * @param  Line   ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�ĸ�����
  * @retval ��
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
	OLED_ShowString(Line,Column,Data);    //call OLED String display function��display on the OLED
	memset(Data,0,sizeof(Data));
}

/**
  * @brief  OLED��ʾ���֣�ʮ���ƣ�����������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��-2147483648~2147483647
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
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
  * @brief  OLED��ʾ���֣�ʮ�����ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~0xFFFFFFFF
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~8
  * @retval ��
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
  * @brief  OLED��ʾ���֣������ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~1111 1111 1111 1111
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~16
  * @retval ��
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
  * @brief  OLED��ʼ��
  * @param  ��
  * @retval ��
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//�ϵ���ʱ
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//�˿ڳ�ʼ��
	
	OLED_WriteCommand(0xAE);	//�ر���ʾ
	
	OLED_WriteCommand(0xD5);	//������ʾʱ�ӷ�Ƶ��/����Ƶ��
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//���ö�·������
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//������ʾƫ��
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//������ʾ��ʼ��
	
	OLED_WriteCommand(0xA1);	//�������ҷ���0xA1���� 0xA0���ҷ���
	
	OLED_WriteCommand(0xC8);	//�������·���0xC8���� 0xC0���·���

	OLED_WriteCommand(0xDA);	//����COM����Ӳ������
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//���öԱȶȿ���
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//����Ԥ�������
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//����VCOMHȡ��ѡ�񼶱�
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//����������ʾ��/�ر�

	OLED_WriteCommand(0xA6);	//��������/��ת��ʾ

	OLED_WriteCommand(0x8D);	//���ó���
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//������ʾ
		
	OLED_Clear();				//OLED����
}
