#ifndef __MYI2C_H__
#define __MYI2C_H__
 
#include "stm32f1xx_hal.h"
#include "main.h"
#include "stdint.h"
 
#define CPU_FREQUENCY_MHZ  72
#define MYI2C_SCL_PIN	  	I2C_SCL_Pin
#define MYI2C_SCL_PORT		I2C_SCL_GPIO_Port
#define MYI2C_SDA_PIN			I2C_SDL_Pin
#define MYI2C_SDA_PORT		I2C_SDL_GPIO_Port
 
#define SDA_OUT_LOW()         HAL_GPIO_WritePin(SDA_GPIO_Port,SDA_Pin,GPIO_PIN_RESET)
#define SDA_OUT_HIGH()        HAL_GPIO_WritePin(SDA_GPIO_Port,SDA_Pin,GPIO_PIN_SET)
#define SDA_DATA_IN()         HAL_GPIO_ReadPin(SDA_GPIO_Port,SDA_Pin)
#define SCL_OUT_LOW()         HAL_GPIO_WritePin(SCL_GPIO_Port,SCL_Pin,GPIO_PIN_RESET)
#define SCL_OUT_HIGH()        HAL_GPIO_WritePin(SCL_GPIO_Port,SCL_Pin,GPIO_PIN_SET)
#define SCL_DATA_IN()         HAL_GPIO_ReadPin(SCL_GPIO_Port,SCL_Pin)
#define SDA_Write(XX)   	  HAL_GPIO_WritePin(SDA_GPIO_Port,SDA_Pin,(XX?GPIO_PIN_SET:GPIO_PIN_RESET))
 
void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint8_t txd); 
uint8_t IIC_Read_Byte(uint8_t ack);
void IIC_NAck(void);
void IIC_Ack(void);
uint8_t IIC_Wait_Ack(void);
 
#endif
