#ifndef __OLED_IIC_H_
#define __OLED_IIC_H_

#include "main.h"

void OLED_Init(void);
void OLED_Clear(void);

void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number);
void OLED_ShowFloatNum(uint8_t x,uint8_t y,float FloatNum);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void delay_us(uint32_t us);  // hal��ʵ��΢�뼶��ʱ
#endif

