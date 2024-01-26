#ifndef TM1637_H
#define TM1637_H

//#include "sys.h"
#include "stm32f1xx_hal.h"
//#define TM1637_CLK           PAout(2)  
//#define TM1637_DIO           PAout(3)  
//#define TM1637_READ_DIO      PAin(3) 
#define TM1637_CLK1  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET)
#define TM1637_CLK0  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET)
#define TM1637_DIO1  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET)
#define TM1637_DIO0  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET)
#define TM1637_READ_DIO HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)
//IO·½ÏòÉèÖÃ
//#define TM1637_DIO_IN()     {GPIOA->CRL&=0XFFFF0FFF;GPIOA->CRL|=8<<12;}
//#define TM1637_DIO_OUT()    {GPIOA->CRL&=0XFFFF0FFF;GPIOA->CRL|=3<<12;}

#define	ADD_AUTO		0X40
#define	ADD_FIX			0x44
#define	DISP_BRIGHT		0x80
#define	ADDR_COMMAND	0XC0
#define	READ_COMMAND	0x42
#define	BRIGHT			0X8F
//-------------------------key code----------------------
#define	S1				0X8F
#define	S2				0X4F
#define	S3				0X0F
#define	S4				0X6F
#define	S5				0XEF
#define	S6				0XAF
extern void delay_us(uint32_t nTimer);
static unsigned char key_code[]={S1,S2,S3,S4,S5,S6};
static unsigned char tab[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
//										{0x3F,0x30,0x5B,0x79,0x74,0x6D,0x6F,0x38,0x7F,0x7D,0x7E,0x67,0x0F,0x73,0x4F,0x4E}};									
 void display_char(unsigned char add,unsigned char dis_char);
//void TM1637_Delay_us(unsigned  int Nus); 
//void TM1637_Start(void);
//void TM1637_Ack(void);
//void TM1637_Stop(void);
//void TM1637_WriteByte(unsigned char oneByte);
//unsigned char TM1637_ScanKey(void);
//void TM1637_NixieTubeDisplay(void);

void TM1637_Init(void);
//void TM1637LED_Init(void);
//void TIM1637_SHOW_ADD_DATA(unsigned char TM1637_add,unsigned char TM163_data);
//void TIM1637_printf(unsigned char TM1637_add0_data,unsigned char TM1637_add1_data,unsigned char TM1637_add2_data,unsigned char TM1637_add3_data);
 
#endif
