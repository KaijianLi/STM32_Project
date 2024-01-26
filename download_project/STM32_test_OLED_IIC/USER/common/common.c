#include  "common.h"

// 可以精确延迟
void  Delay_ms(uint32_t myms)
{
    uint32_t delaytime;
    delaytime = HAL_GetTick();
    while( (HAL_GetTick()-delaytime) < myms);
}

// 此延迟无法精确定时
void Delay(volatile uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

