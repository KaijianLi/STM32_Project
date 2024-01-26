#include "key.h"
#include "led.h"
#include "common.h"

void KEY_GPIO_Init(void)
{ 
 GPIO_InitTypeDef  GPIO_InitStruct;
  
 KEY_GPIO_CLK_ENABLE();
  
 GPIO_InitStruct.Pin  = KEY1_PIN | KEY2_PIN | KEY3_PIN;
 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull = GPIO_PULLUP;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
 HAL_GPIO_Init(GPIOx_KEY_PORT, &GPIO_InitStruct);
}

void KEY_Process(void)
{
 if(!READ_KEY1())
 {
  LED_ON();
 }
 if(!READ_KEY2())
 {
  LED_ON();
 }
 if(!READ_KEY3())
 {
  LED_OFF();
 }
}