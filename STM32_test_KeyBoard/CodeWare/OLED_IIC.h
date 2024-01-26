#ifndef __OLED_IIC_H_
#define __OLED_IIC_H_

#include "main.h"

void OLED_Init(void);
void OLED_Clear(void);
void OLED_Fill(void);

void OLED_Refresh(void);
void OLED_DrawPoint(uint8_t row,uint8_t col,uint8_t mode);
void OLED_DrawCircle(uint8_t row,uint8_t col,uint8_t r);
void OLED_DrawLine(uint8_t start_row,uint8_t start_col,uint8_t end_row,uint8_t end_col);

void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number);
void OLED_ShowFloatNum(uint8_t x,uint8_t y,float FloatNum);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode);
void OLED_ShowMirrorPic(uint8_t row,uint8_t col,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode);
void delay_us(uint32_t us);  // hal锟斤拷实锟斤拷微锟诫级锟斤拷时
#endif
