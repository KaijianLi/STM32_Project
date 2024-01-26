
#include "TM1637.h"
//#include "delay.h"
/** Write multiple bits in an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 */
void TM1637_Delay_us(unsigned  int Nus)
{
 for(;Nus>0;Nus--)
	{
	  __nop();
	 	__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
	}
}
/*===================================================================================================
���ֿ�
 unsigned char SEGData[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//1-9	
 unsigned char SEGDataDp[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef}; //��С���㣬ֻ����0x01
=====================================================================================================*/
void TM1637_DIO_OUT(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
}

void TM1637_DIO_IN(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;	
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);		
}

void TM1637_Start(void)//��ʼ����
{
	TM1637_DIO_OUT(); 
    TM1637_CLK1;
	TM1637_DIO1;
	delay_us(2);
	TM1637_DIO0;
	TM1637_CLK1;
	delay_us(2);
}

void TM1637_Ack(void)//����Ӧ���ź�
{
//	TM1637_DIO_IN(); 
    TM1637_CLK0;
    TM1637_DIO1;
	TM1637_Delay_us(5);
	while(TM1637_READ_DIO)
	{
		;
	}
	TM1637_CLK1;
	delay_us(2);
    TM1637_DIO_IN(); 
//	TM1637_CLK = 0;
//	delay_us(2);
}

void TM1637_Stop(void)//ֹͣ�ź�
{
	TM1637_DIO_OUT(); 
	TM1637_CLK0;
	delay_us(2);
	TM1637_DIO0;
	delay_us(2);
	TM1637_CLK1;
	delay_us(2);
	TM1637_DIO1;
	delay_us(2);
}
void TM1637_WriteByte(unsigned char oneByte)//д��һ���ֽ�
{
 unsigned char i;
	TM1637_DIO_OUT(); 
	for(i=0;i<8;i++)
	{
	 TM1637_CLK0;
		if(oneByte&0x01)
		{
			TM1637_DIO1;
		}
		else
		{
			TM1637_DIO0;
		}
		delay_us(3);
		oneByte=oneByte>>1;
		TM1637_CLK1;
		delay_us(3);
	}
	TM1637_Ack();
}

unsigned char TM1637_ScanKey(void)//����ɨ��
{
	unsigned char reKey,i;
	TM1637_Start();
	TM1637_WriteByte(0x42);
	TM1637_Ack();
	TM1637_DIO1;
	TM1637_DIO_IN(); 
	for(i=0;i<8;i++)
	{
	  TM1637_CLK0;
		reKey=reKey>>1;
		delay_us(30);
		TM1637_CLK1;
		if(TM1637_READ_DIO)
		{
		  reKey=reKey|0x80;
		}
		else
		{
		  reKey=reKey|0x00;
		}
		delay_us(30);
	}
    TM1637_Ack();
	TM1637_Stop();
	return(reKey);
}
void display_fix(unsigned char add,unsigned char DispData)		//ָ����ַд������
{
	TM1637_Start();          //start signal sent to TM1637 from MCU
	TM1637_WriteByte(ADD_FIX);//
	TM1637_Stop();           //
	TM1637_Start();          //
	TM1637_WriteByte(ADDR_COMMAND|add);//
	TM1637_WriteByte(DispData);//
	TM1637_Stop();            //
}
void display_char(unsigned char add,unsigned char dis_char)
{
	unsigned char point_flag,char_code;
	point_flag=dis_char&0x80;
	dis_char&=0x7f;
	if(dis_char<16)
		char_code=tab[dis_char];
	else switch(dis_char)
		{
			case 'L':char_code=0x38;break;
			case 'H':char_code=0x76;break;
			case 'U':char_code=0x3e;break;
			case 'N':char_code=0x37;break;
			case 'P':char_code=0x73;break;
			case 'J':char_code=0x0e;break;
			case 'O':char_code=0x3f;break;
			case 'o':char_code=0x5c;break;
			case 'n':char_code=0x54;break;
			case 'Y':char_code=0x6e;break;
			case ' ':char_code=0;break;
			default:char_code=dis_char;break;
		}
	display_fix(add,char_code|point_flag);
}
void TM1637_NixieTubeDisplay(void)//��ʾ����,�̶���ַģʽ
{
	TM1637_Start();
	TM1637_WriteByte(0x44);//�̶���ַ
	TM1637_Ack();
	TM1637_Stop();
	TM1637_Start();
	TM1637_WriteByte(0xC2);
	TM1637_Ack();
		

		TM1637_WriteByte(0xb0);
		TM1637_Ack();

	TM1637_Stop();
	TM1637_Start();
	TM1637_WriteByte(0x8f);
	TM1637_Ack();
	TM1637_Stop();
}

void TM1637_Init(void)//��ʼ������PA2-clk,pA3-dio
{
 int k;
// GPIO_InitTypeDef  GPIO_InitStructure; 	
// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 	
// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;				  
// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
// GPIO_Init(GPIOA, &GPIO_InitStructure);					  

//	GPIO_InitTypeDef GPIO_InitStruct;
//  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	TM1637_Start();
	TM1637_WriteByte(ADD_AUTO);
	TM1637_Stop();	
	TM1637_Start();
	TM1637_WriteByte(ADDR_COMMAND);
	for(k=0;k<6;k++)
	{
		TM1637_WriteByte(0);
	}
	TM1637_Stop();	
	TM1637_Start();
	TM1637_WriteByte(BRIGHT);
	TM1637_Stop();
}
 
void TIM1637_SHOW_ADD_DATA(unsigned char TM1637_add,unsigned char TM163_data)//ĳһλ����ʾĳ���� //�̶���ַģʽ
{
	TM1637_Start();
	TM1637_WriteByte(0x44);
	TM1637_Ack();
	TM1637_Stop();
	TM1637_Start();
	TM1637_WriteByte(TM1637_add);
	TM1637_Ack();
		
		TM1637_WriteByte(TM163_data);
		TM1637_Ack();
	
	TM1637_Stop();
	TM1637_Start();
	TM1637_WriteByte(0x8f);
	TM1637_Ack();
	TM1637_Stop();
}
//========================================================================
//�����ֱ�Ϊ�ĸ����������ʾ�����֣�ģʽΪ�Զ�+1  (0x40)
//========================================================================
void TIM1637_printf(unsigned char TM1637_add0_data,unsigned char TM1637_add1_data,unsigned char TM1637_add2_data,unsigned char TM1637_add3_data)
{	
	unsigned char i;
	TM1637_Start();
	TM1637_WriteByte(0x40);
	TM1637_Ack();
	TM1637_Stop();
	TM1637_Start();
	TM1637_WriteByte(0xC0);
	TM1637_Ack();	
	for(i=0;i<4;i++)
	{
		if(i==0){TM1637_WriteByte(TM1637_add0_data);TM1637_Ack();}
		if(i==1){TM1637_WriteByte(TM1637_add1_data);TM1637_Ack();}
		if(i==2){TM1637_WriteByte(TM1637_add2_data);TM1637_Ack();}
		if(i==3){TM1637_WriteByte(TM1637_add3_data);TM1637_Ack();}
	}
	TM1637_Stop();
	TM1637_Start();
	TM1637_WriteByte(0x8f);
	TM1637_Ack();
	TM1637_Stop();


}


