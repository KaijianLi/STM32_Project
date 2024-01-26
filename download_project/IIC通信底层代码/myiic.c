#include "myiic.h"

/******************************************************************************************************
 *【文件名称】   : myiic.c
 *【文件描述】   : iic通信驱动代码
 *【文件功能】   : 为iic通信提供接口函数
 *【主控芯片】   : STM432F407zg
 *【实验平台】   : STM32F4xx开发板
 *【编写环境】   : IAR 8.40.2
 *【编写时间】   : 2020-4-14
 *【作    者】   : 李剀(KevinLee)  CSDN: Kevin_8_Lee
 *【注:】 改用的原子的例程代码

*******************************************************************************************************/

//产生IIC起始信号
void IIC_Start(void)
{
		SDA_OUT();     													    //sda线输出
		HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_SET);	//IIC_SDA=1;
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_SET);	//IIC_SCL=1;
		HAL_Delay(4);
		/* IIC_SDA=0; START:when CLK is high,DATA change form high to low */
		HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_RESET);
		HAL_Delay(4);
		/* IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 */
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET);
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
		SDA_OUT();//sda线输出
		/* IIC_SCL=0;-------------------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET);
		/* IIC_SDA=0; STOP:when CLK is high DATA change form low to high----------*/
		HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_RESET);
		HAL_Delay(4);
		/* IIC_SCL=1;-------------------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_SET);
		/* IIC_SDA=1; 发送I2C总线结束信号------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_SET);
		HAL_Delay(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
		uint8_t ucErrTime=0;
		SDA_IN();      //SDA设置为输入  
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
		/* IIC_SCL=0; 时钟输出0---------------------------------------------------*/
		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET);
		return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
		uint8_t t;   
		SDA_OUT(); 	    
		/* IIC_SCL=0; 拉低时钟开始数据传输------------------------------------------*/
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
				HAL_Delay(2);   //对TEA5767这三个延时都是必须的
				/* IIC_SCL=1; --------------------------------------------------------*/
				HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_SET);
				HAL_Delay(2); 
				/* IIC_SCL=0; 拉低时钟开始数据传输-------------------------------------*/
				HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET);
				HAL_Delay(2);
		}	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
		unsigned char i,receive=0;
		SDA_IN();//SDA设置为输入
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
						IIC_NAck();//发送nACK
				else
						IIC_Ack(); //发送ACK   
				return receive;
}




