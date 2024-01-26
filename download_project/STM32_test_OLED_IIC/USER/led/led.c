#include "led.h"
#include  "common.h"

void LED_GPIO_init(void)
{
 GPIO_InitTypeDef  GPIO_InitStruct;
 LED_GPIO_CLK_ENABLE();
 GPIO_InitStruct.Pin  = LED_PIN;
 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
 HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
 LED_OFF();
}

void LED_set(void)
{
 LED_ON();
 Delay_ms(1000);
 LED_OFF();
 Delay_ms(1000);
}
