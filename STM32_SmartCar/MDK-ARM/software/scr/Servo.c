#include "Servo.h"
void SetAngle(int angle)
{
	float PWM = 0.0;
	PWM = 50/45.0 * angle + 50;
	
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,PWM);
	
}

