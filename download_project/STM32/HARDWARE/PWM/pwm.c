#include "pwm.h"

void PWM_Init_TIM2(u16 psc,u16 arr)
{
	GPIO_InitTypeDef GPIO_InitPwm;        //GPIO��ʼ��
	TIM_TimeBaseInitTypeDef TIM_PwmInit;  //��ʱ����ʼ��
	TIM_OCInitTypeDef TIM_OcPwminit;      //ָ����ʱ�����ͨ����ʼ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //����GPIOʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //����i/o����ӳ��

	GPIO_InitPwm.GPIO_Mode  = GPIO_Mode_AF_PP; //���긴�����
	GPIO_InitPwm.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitPwm.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitPwm);

	TIM_PwmInit.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_PwmInit.TIM_Period = arr;
	TIM_PwmInit.TIM_Prescaler = psc;
	TIM_PwmInit.TIM_ClockDivision = TIM_CKD_DIV1; //1��Ƶ
	TIM_TimeBaseInit(TIM2, &TIM_PwmInit);

	TIM_OcPwminit.TIM_OCMode      = TIM_OCMode_PWM1 ;       //�Ƚ����ģʽpwm1
	TIM_OcPwminit.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ�� 
	TIM_OcPwminit.TIM_OCNPolarity = TIM_OCNPolarity_Low;    //����������ԡ��͵�ƽ��Ч

	TIM_OC3Init(TIM2, &TIM_OcPwminit);            //��ʼ��ͨ��
	TIM_OC4Init(TIM2, &TIM_OcPwminit);            //��ʼ��ͨ��

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //���Ʋ�����������Ч���Ƕ�ʱ��������һ�θ����¼�ʱ�����µ�
														//Enable:��һ�θ����¼�ʱ������
														//Disable:������Ч
	TIM_Cmd(TIM2, ENABLE);                      //ʹ��TIM3������

}



