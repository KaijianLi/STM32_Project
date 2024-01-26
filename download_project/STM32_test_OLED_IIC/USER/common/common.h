#ifndef  __COMMON_H__
#define  __COMMON_H__

#include "stm32f1xx_hal.h"

typedef  enum{
    FALSE = 0,
    TRUE  = !FALSE,
}BOOL;


void  Delay_ms(uint32_t );
void  Delay(volatile uint32_t );

#endif
