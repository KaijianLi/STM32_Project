#include "main.h"
#include "PID.h"
#include "stdio.h"

PID_SPEED PID_LeftWheel;  // �����ٶȻ�
PID_SPEED PID_RightWheel; // �����ٶȻ�
PID_BALANCE PID_Balance;    // ֱ����
PID_BALANCE PID_Turn;       // ת��
/**
  * @brief  PID�ṹ���ʼ�����������������ֵĵ����������ȫ��ͬ�����Զ���ͬ����PWMֵ��
			�����ֵ���Ӧ����ȫ��ͬ�����������ֵ�PID���໥������
  * @param  structure: �ṹ���ַ
			@arg &PID_LeftWheel   ��ʼ������PID
			@arg &PID_RightWheel  ��ʼ������PID
  * @param  ID: ���������ֵı��
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
	// ��ʼ���ٶȻ�
	PID_InitSpeed(&PID_LeftWheel,LEFT_WHEEL);
	PID_InitSpeed(&PID_RightWheel,RIGHT_WHEEL);
	
	PID_InitBalance(&PID_Balance);
}

void calculate_Balance(PID_BALANCE* PID,float nowAngle,float MidAngle)
{
	// ���ȶԽǶȽ����޷�
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
���ȶ�ȡ��ǰ�����ת��ֵ���������ǰ���ٶ�ֵ��Ȼ����뵽�ٶȻ��У�
ͨ������ʽPID�㷨�������Ҫ���ӵ�PWM��ֵ��Ȼ�������һ��Ӧ�����õ�PWMֵ
*/
/**
  * @brief  PID���㺯��
  * @param  PID: �������������������ֻ�������
  * @param  now_speed�������ǰת��
  * @param  goal��Ŀ��ת�٣���ΧΪ[-110,110]
  * @param  ID�����������ֵı��
			@arg 0������
			@arg 1������
  * @retval None
  */
void calculate_Speed(PID_SPEED* PID,float now_speed,int goal)
{
	if(goal >= 120)    // �ٶ��޷������ֵ��ò�����120cm/s����
	   goal = 120;    // ��ʱҲ��110cm/s����
	else if(goal <= -120)
	   goal = -120;
	
	int add_PWM = 0;
	PID -> now_state  = now_speed;
	PID -> goal_state = goal;
	
	// ����ʽPID����
	PID -> error = (PID -> goal_state) - now_speed;
	add_PWM = PID->kp * (PID->error - PID->last_error) \
	        + PID->ki * PID->error\
         	+ PID->kd * (PID->error + PID->last_last_error - 2 * PID->last_error);	
	
	PID -> Output_PWM = PID ->last_Set + add_PWM;
	// ����������޷�����ֹ�������⵼�»�������۹�����Ӧʱ������
	if(PID -> Output_PWM >= 1000)  
	   PID -> Output_PWM = 1000;
	else if(PID -> Output_PWM <= -1000)
	   PID -> Output_PWM = -1000;
	
	PID->last_last_error = PID->last_error;
	PID->last_error = PID->error;
	PID->last_Set = PID->Output_PWM;
}
