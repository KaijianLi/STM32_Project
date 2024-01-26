#ifndef __MOTOR_H_
#define __MOTOR_H_
#define ADD_LOAD 1000
/*
	电机的驱动依靠两个引脚来完成，一个引脚输出数字信号代表正转与反转，
	另一个引脚输出PWM方波完成电机的调速。
	数字引脚输出0，则电机正转，
	数字引脚输出1，则电机反转，此时若要保持原速率反转，则比较值应该与自动重装载值做差；
*/
typedef enum
{
	LEFT_WHEEL  = 0u,
	RIGHT_WHEEL = 1,
	BOTH
}Wheel_ID;

void Motor_Init(void);  // 将电机的PWM引脚和数字引脚全部置零
void MotorRun(Wheel_ID ID,int speed); 

#endif
