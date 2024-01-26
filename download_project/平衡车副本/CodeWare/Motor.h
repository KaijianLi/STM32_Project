#ifndef __MOTOR_H_
#define __MOTOR_H_
#define ADD_LOAD 1000
/*
	�������������������������ɣ�һ��������������źŴ�����ת�뷴ת��
	��һ���������PWM������ɵ���ĵ��١�
	�����������0��������ת��
	�����������1��������ת����ʱ��Ҫ����ԭ���ʷ�ת����Ƚ�ֵӦ�����Զ���װ��ֵ���
*/
typedef enum
{
	LEFT_WHEEL  = 0u,
	RIGHT_WHEEL = 1,
	BOTH
}Wheel_ID;

void Motor_Init(void);  // �������PWM���ź���������ȫ������
void MotorRun(Wheel_ID ID,int speed); 

#endif
