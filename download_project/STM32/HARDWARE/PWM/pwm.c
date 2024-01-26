#include "pwm.h"

void PWM_Init_TIM2(u16 psc,u16 arr)
{
	GPIO_InitTypeDef GPIO_InitPwm;        //GPIO初始化
	TIM_TimeBaseInitTypeDef TIM_PwmInit;  //定时器初始化
	TIM_OCInitTypeDef TIM_OcPwminit;      //指定定时器输出通道初始化
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //配置GPIO时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //开启i/o口重映射

	GPIO_InitPwm.GPIO_Mode  = GPIO_Mode_AF_PP; //推完复用输出
	GPIO_InitPwm.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitPwm.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitPwm);

	TIM_PwmInit.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_PwmInit.TIM_Period = arr;
	TIM_PwmInit.TIM_Prescaler = psc;
	TIM_PwmInit.TIM_ClockDivision = TIM_CKD_DIV1; //1分频
	TIM_TimeBaseInit(TIM2, &TIM_PwmInit);

	TIM_OcPwminit.TIM_OCMode      = TIM_OCMode_PWM1 ;       //比较输出模式pwm1
	TIM_OcPwminit.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能 
	TIM_OcPwminit.TIM_OCNPolarity = TIM_OCNPolarity_Low;    //互补输出极性、低电平有效

	TIM_OC3Init(TIM2, &TIM_OcPwminit);            //初始化通道
	TIM_OC4Init(TIM2, &TIM_OcPwminit);            //初始化通道

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //控制波形是立即生效还是定时器发生下一次更新事件时被更新的
														//Enable:下一次更新事件时被更新
														//Disable:立即生效
	TIM_Cmd(TIM2, ENABLE);                      //使能TIM3的外设

}



