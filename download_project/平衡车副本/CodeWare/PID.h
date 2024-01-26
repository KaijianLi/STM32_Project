#ifndef __PID_H_
#define __PID_H_
#include "Motor.h"
typedef struct
{
	float kp;
	float ki;
	float kd;
	
	int goal_state;
	float now_state;
	
	int error;
	int last_error;
	int last_last_error;
	
	int last_Set;
	int Output_PWM;
}PID_SPEED;

typedef struct
{
	float kp;
	float kd;
	
	float error;
	float last_error;
	
	int output;
}PID_BALANCE;

void PID_InitSpeed(PID_SPEED* structure,Wheel_ID ID);
void PID_InitBalance(PID_BALANCE* structure);
void calculate_Speed(PID_SPEED* structure,float speed,int goal);
void calculate_Balance(PID_BALANCE* PID,float nowAngle,float MidAngle);
void PID_Init(void);
#endif
