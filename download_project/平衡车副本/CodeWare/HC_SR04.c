#include "HC_SR04.h"

void SR04_Init()
{
	// �򿪶�ʱ�����벶�񣬶�ʱ�������ж��Ѿ���ǰ��ĵ����ʼ���п���
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);  //�����벶��
	
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);
	
	GPIO_InitStruct.Pin = TRIG_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
    HAL_GPIO_Init(TRIG_GPIO_Port, &GPIO_InitStruct);
}