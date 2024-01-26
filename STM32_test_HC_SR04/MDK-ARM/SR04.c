#include "SR04.h"
 
float distant;      //��������
uint32_t measure_Buf[3] = {0};   //��Ŷ�ʱ������ֵ������
uint8_t  measure_Cnt = 0;    //״̬��־λ
uint32_t high_time;   //������ģ�鷵�صĸߵ�ƽʱ��
uint8_t TIM4_CH1_Edge=0;  //״̬�仯ʱ������ֵ
uint32_t TIM4_CH1_VAL=0;  //����������ļ�¼ֵ
uint32_t TIM4_CH1_OVER=0; //����������ĸ���

//===============================================us��ʱ����
void delay_us(uint32_t us)//��Ƶ72M
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--)
	{
		;
	}
}

//===============================================��ȡ����
void SR04_GetData(void)
{
	HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
switch (measure_Cnt){
	case 0:
		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin); 
		measure_Cnt++;	//��־λ +1 ���벶�������ص�״̬
		__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);	// ����������
		HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);	//�������벶��       ����: __HAL_TIM_ENABLE(&htim5);                                                                                    		
        break;
	case 3:
		high_time = measure_Buf[1] - measure_Buf[0] + TIM4_CH1_OVER  * 10000;	//�ߵ�ƽʱ�䣨us��
        printf("\r\n----�ߵ�ƽʱ��-%d-us----\r\n",high_time);							
		distant = (high_time * 0.034) / 2;  //��λ:cm
        printf("\r\n-������Ϊ-%.2f-cm-\r\n",distant);          
		measure_Cnt = 0;  //��ձ�־λ
        __HAL_TIM_SET_COUNTER(&htim2,0);  //���ö�ʱ��CNT��������ֵΪ0
		break;
	default:
		break;
	}
}

//===============================================�жϻص�����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//
{
	if(TIM2 == htim->Instance)// �жϴ������жϵĶ�ʱ��ΪTIM2
	{
		switch(measure_Cnt)
		{
			case 1:
				measure_Buf[0] = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1);//��ȡ��ǰ�Ĳ���ֵ.
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);  //����Ϊ�½��ز���
				measure_Cnt++;                                            
				break;              
			case 2:
				measure_Buf[1] = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1);//��ȡ��ǰ�Ĳ���ֵ.
				HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_1); //ֹͣ����   ����: __HAL_TIM_DISABLE(&htim5);
				measure_Cnt++;     
		}
	}
}
//===============================================�ж��������
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim2)
	{
		if(measure_Cnt == 2)	// �ڵȴ��½��صĹ��������������������¼�������
		{
			TIM4_CH1_OVER++;  //��ʱ�����ֵ����
		}
	}
}

