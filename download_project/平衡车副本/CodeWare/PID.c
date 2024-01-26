#include "main.h"
#include "PID.h"
#include "stdio.h"

PID_SPEED PID_LeftWheel;  // 左轮速度环
PID_SPEED PID_RightWheel; // 右轮速度环
PID_BALANCE PID_Balance;    // 直立环
PID_BALANCE PID_Turn;       // 转向环
/**
  * @brief  PID结构体初始化函数，由于左右轮的电机参数不完全相同，所以对于同样的PWM值，
			左右轮的响应不完全相同，所以左右轮的PID是相互独立的
  * @param  structure: 结构体地址
			@arg &PID_LeftWheel   初始化左轮PID
			@arg &PID_RightWheel  初始化右轮PID
  * @param  ID: 代表左右轮的编号
			@arg LEFT_WHEEL
			@arg RIGHT_WHEEL
  * @retval None
  */
void PID_InitSpeed(PID_SPEED* structure,Wheel_ID ID)
{
	if(ID == LEFT_WHEEL)
	{
		structure -> kp         = 20.0;
		structure -> ki         = 6.3;
		structure -> kd         = 1.0;
	}	
	else if(ID == RIGHT_WHEEL)
	{
		structure -> kp         = 20;
		structure -> ki         = 9.0;
		structure -> kd         = 1.0;
	}
	
	structure -> goal_state = 0;
	structure -> now_state  = 0;
	
	structure -> error      = 0;
	structure -> last_error = 0;
	structure -> last_last_error = 0;
	
	structure -> last_Set   = 0;
	structure -> Output_PWM = 0;
	
}

void PID_InitBalance(PID_BALANCE* structure)
{
	structure -> kp = -30;
	structure -> kd = -90;
	
	structure -> error      = 0;
	structure -> last_error = 0;
	
	structure -> output = 0;
}

void PID_Init(void)
{
	// 初始化速度环
	PID_InitSpeed(&PID_LeftWheel,LEFT_WHEEL);
	PID_InitSpeed(&PID_RightWheel,RIGHT_WHEEL);
	
	PID_InitBalance(&PID_Balance);
}

void calculate_Balance(PID_BALANCE* PID,float nowAngle,float MidAngle)
{
	// 首先对角度进行限幅
	if(nowAngle >= 90.0)
		nowAngle = 90.0;
	else if(nowAngle <= -90.0)
		nowAngle = -90.0;
	
	PID -> error  = nowAngle - MidAngle;
	PID -> output = PID->kp * (PID->error) + PID->kd * (PID->error - PID->last_error);
	
	if(PID->output >= 1000)
		PID->output = 1000;
	else if(PID->output <= -1000)
		PID->output = -1000;
	printf("Error:%f Last_Error:%f Output:%d\r\n",PID -> error,PID->last_error,PID -> output);
	PID->last_error = PID->error;
}
/*
首先读取当前轮轴的转速值，解算出当前的速度值，然后进入到速度环中，
通过增量式PID算法解算出需要增加的PWM数值，然后输出下一次应该设置的PWM值
*/
/**
  * @brief  PID计算函数
  * @param  PID: 决定驱动的轮子是左轮还是右轮
  * @param  now_speed：电机当前转速
  * @param  goal：目标转速，范围为[-110,110]
  * @param  ID：代表左右轮的编号
			@arg 0：左轮
			@arg 1：右轮
  * @retval None
  */
void calculate_Speed(PID_SPEED* PID,float now_speed,int goal)
{
	if(goal >= 120)    // 速度限幅，最大值测得不超过120cm/s左右
	   goal = 120;    // 有时也在110cm/s左右
	else if(goal <= -120)
	   goal = -120;
	
	int add_PWM = 0;
	PID -> now_state  = now_speed;
	PID -> goal_state = goal;
	
	// 增量式PID计算
	PID -> error = (PID -> goal_state) - now_speed;
	add_PWM = PID->kp * (PID->error - PID->last_error) \
	        + PID->ki * PID->error\
         	+ PID->kd * (PID->error + PID->last_last_error - 2 * PID->last_error);	
	
	PID -> Output_PWM = PID ->last_Set + add_PWM;
	// 积分项必须限幅，防止由于意外导致积分项积累过大，响应时间增加
	if(PID -> Output_PWM >= 1000)  
	   PID -> Output_PWM = 1000;
	else if(PID -> Output_PWM <= -1000)
	   PID -> Output_PWM = -1000;
	
	PID->last_last_error = PID->last_error;
	PID->last_error = PID->error;
	PID->last_Set = PID->Output_PWM;
}
