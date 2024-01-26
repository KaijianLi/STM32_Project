#include "SR04.h"
 
float distant;      //测量距离
uint32_t measure_Buf[3] = {0};   //存放定时器计数值的数组
uint8_t  measure_Cnt = 0;    //状态标志位
uint32_t high_time;   //超声波模块返回的高电平时间
uint8_t TIM4_CH1_Edge=0;  //状态变化时，计数值
uint32_t TIM4_CH1_VAL=0;  //储存计数器的记录值
uint32_t TIM4_CH1_OVER=0; //计数器溢出的个数

//===============================================us延时函数
void delay_us(uint32_t us)//主频72M
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--)
	{
		;
	}
}

//===============================================读取距离
void SR04_GetData(void)
{
	HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
switch (measure_Cnt){
	case 0:
		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin); 
		measure_Cnt++;	//标志位 +1 进入捕获上升沿的状态
		__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);	// 捕获上升沿
		HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);	//启动输入捕获       或者: __HAL_TIM_ENABLE(&htim5);                                                                                    		
        break;
	case 3:
		high_time = measure_Buf[1] - measure_Buf[0] + TIM4_CH1_OVER  * 10000;	//高电平时间（us）
        printf("\r\n----高电平时间-%d-us----\r\n",high_time);							
		distant = (high_time * 0.034) / 2;  //单位:cm
        printf("\r\n-检测距离为-%.2f-cm-\r\n",distant);          
		measure_Cnt = 0;  //清空标志位
        __HAL_TIM_SET_COUNTER(&htim2,0);  //设置定时器CNT计数器的值为0
		break;
	default:
		break;
	}
}

//===============================================中断回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//
{
	if(TIM2 == htim->Instance)// 判断触发的中断的定时器为TIM2
	{
		switch(measure_Cnt)
		{
			case 1:
				measure_Buf[0] = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1);//获取当前的捕获值.
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);  //设置为下降沿捕获
				measure_Cnt++;                                            
				break;              
			case 2:
				measure_Buf[1] = HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1);//获取当前的捕获值.
				HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_1); //停止捕获   或者: __HAL_TIM_DISABLE(&htim5);
				measure_Cnt++;     
		}
	}
}
//===============================================中断溢出函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim2)
	{
		if(measure_Cnt == 2)	// 在等待下降沿的过程中若计数器溢出，记录溢出次数
		{
			TIM4_CH1_OVER++;  //定时器溢出值增加
		}
	}
}

