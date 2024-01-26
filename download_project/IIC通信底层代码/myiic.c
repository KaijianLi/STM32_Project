#include "myiic.h"

/******************************************************************************************************
 *���ļ����ơ�   : myiic.c
 *���ļ�������   : iicͨ����������
 *���ļ����ܡ�   : Ϊiicͨ���ṩ�ӿں���
 *������оƬ��   : STM432F407zg
 *��ʵ��ƽ̨��   : STM32F4xx������
 *����д������   : IAR 8.40.2
 *����дʱ�䡿   : 2020-4-14
 *����    �ߡ�   : ����(KevinLee)  CSDN: Kevin_8_Lee
 *��ע:�� ���õ�ԭ�ӵ����̴���

*******************************************************************************************************/

//����IIC��ʼ�ź�
void IIC_Start(void)
{
		SDA_OUT();     													    //sda�����
		HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_SET);	//IIC_SDA=1;
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_SET);	//IIC_SCL=1;
		HAL_Delay(4);
		/* IIC_SDA=0; START:when CLK is high,DATA change form high to low */
		HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_RESET);
		HAL_Delay(4);
		/* IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� */
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET);
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
		SDA_OUT();//sda�����
		/* IIC_SCL=0;-------------------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET);
		/* IIC_SDA=0; STOP:when CLK is high DATA change form low to high----------*/
		HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_RESET);
		HAL_Delay(4);
		/* IIC_SCL=1;-------------------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_SET);
		/* IIC_SDA=1; ����I2C���߽����ź�------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_SET);
		HAL_Delay(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
		uint8_t ucErrTime=0;
		SDA_IN();      //SDA����Ϊ����  
		/* IIC_SDA=1;-------------------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_SET);
		HAL_Delay(1);	   
		/* IIC_SCL=1;-------------------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_SET);
		HAL_Delay(1);	 
		while ( HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port, IIC_SDA_Pin) == GPIO_PIN_SET )
		{
				ucErrTime++;
				if (ucErrTime > 250)
				{
						IIC_Stop();
						return 1;
				}
		}
		/* IIC_SCL=0; ʱ�����0---------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET);
		return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
		/* IIC_SCL=0;-------------------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET);
		SDA_OUT();
		/* IIC_SDA=0;-------------------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_RESET);
		HAL_Delay(2);
		/* IIC_SCL=1;-------------------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_SET);
		HAL_Delay(2);
		/* IIC_SCL=0;-------------------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET);
}
//������ACKӦ��		    
void IIC_NAck(void)
{
		/* IIC_SCL=0;-------------------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET);
		SDA_OUT();
		/* IIC_SDA=1;-------------------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_SET);
		HAL_Delay(2);
		/* IIC_SCL=1;-------------------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_SET);
		HAL_Delay(2);
		/* IIC_SCL=0;-------------------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET);
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{                        
		uint8_t t;   
		SDA_OUT(); 	    
		/* IIC_SCL=0; ����ʱ�ӿ�ʼ���ݴ���------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET);
		for (t = 0; t < 8; t++)
		{
				//IIC_SDA = ( txd & 0x80 ) >> 7;
				if ((txd & 0x80) >> 7)
				{
						HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_SET);
				}
				else
				{
						HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_RESET);
				}
				txd<<=1; 	  
				HAL_Delay(2);   //��TEA5767��������ʱ���Ǳ����
				/* IIC_SCL=1; --------------------------------------------------------*/
				HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_SET);
				HAL_Delay(2); 
				/* IIC_SCL=0; ����ʱ�ӿ�ʼ���ݴ���-------------------------------------*/
				HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET);
				HAL_Delay(2);
		}	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
		unsigned char i,receive=0;
		SDA_IN();//SDA����Ϊ����
		for(i=0;i<8;i++ )
		{
				/* IIC_SCL=0;---------------------------------------------------------*/
				HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET);
				HAL_Delay(2);
				/* IIC_SCL=1;---------------------------------------------------------*/
				HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_SET);
				receive<<=1;
				if (HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port, IIC_SDA_Pin) == GPIO_PIN_SET)
						receive++;   
				HAL_Delay(1); 
				}					 
				if (!ack)
						IIC_NAck();//����nACK
				else
						IIC_Ack(); //����ACK   
				return receive;
}




