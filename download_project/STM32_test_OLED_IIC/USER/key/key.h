#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f1xx_hal.h"
#include "common.h"

#define    KEY_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()

#define    GPIOx_KEY_PORT                GPIOB

#define    KEY1_PIN                      GPIO_PIN_6
#define    KEY2_PIN                      GPIO_PIN_7
#define    KEY3_PIN                      GPIO_PIN_8


#define    READ_KEY1()                   HAL_GPIO_ReadPin(GPIOx_KEY_PORT, KEY1_PIN)
#define    READ_KEY2()                   HAL_GPIO_ReadPin(GPIOx_KEY_PORT, KEY2_PIN)
#define    READ_KEY3()                   HAL_GPIO_ReadPin(GPIOx_KEY_PORT, KEY3_PIN)


void KEY_GPIO_Init(void);
void KEY_Process(void);

#endif
