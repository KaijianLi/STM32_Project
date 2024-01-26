#include "Motor.h"
#include "main.h"
#include "tim.h"

void Motor_Init()
{
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	
    __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);
	HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET);
	
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,0);
	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
}
/**
  * @brief  �����������
  * @param  wheel: �������������������ֻ�������
		  @arg LEFT_WHEEL
		  @arg RIGHT_WHEEL
		  @arg BOTH
  * @param  speed: ��������ת��������ֵ��С�����������ΧΪ[-1000,1000]
  * @retval None
  */
void MotorRun(Wheel_ID ID,int speed)
{
	char direct = 0;
	if(speed >= 0)
	{
		direct = 1;
		if(speed >= ADD_LOAD) 
			speed = ADD_LOAD;
	}
	else
	{
		direct = 0;
		if(speed <= -ADD_LOAD)
			speed = -ADD_LOAD;
		speed = -speed;
	}
	
	switch(ID)
	{
		case LEFT_WHEEL:
			if(direct)
			{
				__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,ADD_LOAD - speed);
			    HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_SET);
			}
			else
			{
				__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,speed);
                HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
			}
		    break;
			
		case RIGHT_WHEEL:
			if(direct)
			{
				__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,speed);
			    HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET);
			}
			else
			{
				__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,ADD_LOAD - speed);
                HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_SET);
			}
		break;	
		case BOTH:
			MotorRun(LEFT_WHEEL, speed);
			MotorRun(RIGHT_WHEEL,speed);
		break;
	}
}
