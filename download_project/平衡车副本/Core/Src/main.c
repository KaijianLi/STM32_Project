/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Motor.h"
#include "PID.h"
#include "OLED_IIC.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "IIC.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

#define Rx_MAX 255
#define PI 3.141592
#define MIDANGLE 4.00
#define AHEAD 'F'
#define LEFT  'L'
#define RIGHT 'R'
#define BACK  'B'

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
	float speed_left;
	float speed_right;
	float Roll;
	float Yaw;
}Motor_Struct;

Motor_Struct nowa_state;
Motor_Struct goal_state;

extern PID_SPEED PID_LeftWheel;  // 左轮速度环
extern PID_SPEED PID_RightWheel; // 右轮速度环
extern PID_BALANCE PID_Balance;

int count_left = 0,count_right = 0;      // 左右轮编码器计数值
float pos_left = 0.0,pos_right = 0;
float pitch,roll,yaw; 		    //欧拉角
extern unsigned char Rx_Buffer[Rx_MAX];  // 缓存区
extern int data_length;                  // 缓存区读取到的数据长度
extern char update_flag;                 // 更新标志位，在缓存区数据更新时有效

// 重定向函数，便于使用printf( )函数，注意需要勾选MicroLIB选项
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

// 定时器定时10ms，每10ms执行一次该函数，执行内容包括：
// 测速，PID闭环控制，并每秒钟显示一次速度
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
////	HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
//	static int i = 0;
//	i++;
//	if(htim == &htim1)
//	{
////		mpu_dmp_get_data(&pitch, &roll, &yaw);
//	  
//		calculate_Balance(&PID_Balance,nowa_state.Roll,MIDANGLE);
//		MotorRun(LEFT_WHEEL, PID_Balance.output);
////		MotorRun(RIGHT_WHEEL,PID_Balance.output);
////		nowa_state.speed_left  = count_left /390.0/0.05 * PI * 6.7;  // 单位：（cm/s）
////		nowa_state.speed_right = count_right/390.0/0.05 * PI * 6.7;       // 单位：（cm/s）
//		
////		calculate_Speed(&PID_LeftWheel ,nowa_state.speed_left, goal_state.speed_left);
////		calculate_Speed(&PID_RightWheel,nowa_state.speed_right,goal_state.speed_right);
////		MotorRun(LEFT_WHEEL,  PID_LeftWheel.Output_PWM);
////		MotorRun(RIGHT_WHEEL,PID_RightWheel.Output_PWM);
////		printf("left: now speed:%f orign set:%d now:%d\r\n",nowa_state.speed_left, \
////		                                                PID_LeftWheel.last_Set,\
////		                                                PID_LeftWheel.Output_PWM);
////		printf("right:now speed:%f orign set:%d now:%d\r\n",nowa_state.speed_right,\
////		                                                PID_RightWheel.last_Set,\
////		                                                PID_RightWheel.Output_PWM);
//		count_left  = 0;
//		count_right = 0;
//	}
//	// 每秒钟刷新一次左右轮的转速值
//	if(i == 100)
//	{
//		printf("roll:%f\r\n",nowa_state.Roll);
//		
////		printf("speed_left:%f(cm/s) speed_right:%f(cm/s)\r\n",nowa_state.speed_left,nowa_state.speed_right);
//		i = 0;
//	}
	
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim1)
	{
		if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_3)  // TIM1 - Channel3 左轮编码器
		{
			GPIO_PinState judge_left = HAL_GPIO_ReadPin(LEFT_B_GPIO_Port,LEFT_B_Pin);
			switch(judge_left)
			{
				case GPIO_PIN_SET:  // 左轮反转
					count_left--;
				break;
				
				case GPIO_PIN_RESET:  // 左轮正转
					count_left++;
				break;
			}
		}
		if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_4)  // TIM1 - Channel4 右轮编码器
		{
			GPIO_PinState judge_right = HAL_GPIO_ReadPin(RIGHT_B_GPIO_Port,RIGHT_B_Pin);
			switch(judge_right)
			{
				case GPIO_PIN_SET:  // 右轮正转
					count_right++;
				break;
				
				case GPIO_PIN_RESET:  // 右轮反转
					count_right--;
				break;
			}
		}
	}
}
/**
  * @brief  OLED显示函数，显示左右轮转速值以及X Y Z三轴角度姿态
  * @retval None
  */
void DisplaySign()
{
	OLED_ShowString(1,1,"L:");
	OLED_ShowFloatNum(1,3,nowa_state.speed_left);
	
	OLED_ShowString(2,1,"R:");
	OLED_ShowFloatNum(2,3,nowa_state.speed_right);
	
	OLED_ShowString(3,1,"X:");
	OLED_ShowString(3,9,"Y:");
	OLED_ShowString(4,1,"Z:");
}
/**
  * @brief  数据处理函数，将串口接收到的数据流分类管理，对相应参数进行赋值
  * @param  *data: 接收区数组缓存
  * @param  length:接收区有效数据长度
  * @retval None
  */
void HandleData(uint8_t *data,int length)
{
	if(data[0] == 'P')
	{
		if(data[1] == '+')
			PID_Balance.kp += 5;
		else if(data[1] == '-')
			PID_Balance.kp -= 5;
		printf("Kp -> %f\r\n",PID_Balance.kp);
		memset(Rx_Buffer,0,sizeof(Rx_Buffer));
	}
	else if(data[0] == 'D')
	{
		if(data[1] == '+')
			PID_Balance.kd += 2.0;
		else if(data[1] == '-')
			PID_Balance.kd -= 2.0;
		printf("Kd -> %f\r\n",PID_Balance.kd);
		memset(Rx_Buffer,0,sizeof(Rx_Buffer));
	}

	int speed_set = 0;
	/*
	速度数据：[-120，120]，单位 (cm/s)
	*/
	// 判断数据是否为速度指令 [-120,120]
	// 是否为负数 
	if(data[0] == '-' && length <= 4 && length > 0)
	{
		speed_set = 0;
		for(int i = 1;i < length;i++)
			speed_set = speed_set * 10 + data[i] - '0';
		speed_set = -speed_set;
		goal_state.speed_left  = speed_set;
		goal_state.speed_right = speed_set;
		memset(Rx_Buffer,0,sizeof(Rx_Buffer));  // 在设置完参数之后清空缓存区，避免数据覆盖
	}
	// 是否为正数
	else if(data[0] >= '0' && data[0] <= '9' && length <= 3 && length > 0)
	{
		speed_set = 0;
		for(int i = 0;i < length;i++)
			speed_set = speed_set * 10 + data[i] - '0';
		goal_state.speed_left  = speed_set;
		goal_state.speed_right = speed_set;
		memset(Rx_Buffer,0,sizeof(Rx_Buffer));
	}
	// 判断是否为方向指令
	else if(data[0] >= 'A' && data[0] <= 'Z' && length == 1)
	{
		switch(data[0])
		{
			case AHEAD:
			case LEFT:
			case RIGHT:
			case BACK:
				break;
			default:
				printf("Error Char message\r\n");
				break;
		}
		memset(Rx_Buffer,0,sizeof(Rx_Buffer));
	}
	if(update_flag == 1)
	{
		printf("speed_set:%d\r\n",speed_set);
		update_flag = 0;
	}
}
#ifdef RxIT
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART3)   //判断发生接收中断的串口
	{
		int PWM = 0;
		if((Rx_Buffer[1] - '0') > 1)
			Rx_Buffer[1] = '1';
		
		if(Rx_Buffer[0] == '+')
			PWM = 1000 * (Rx_Buffer[1] - '0') + 100 * (Rx_Buffer[2] - '0') + 10 * (Rx_Buffer[3] - '0') + (Rx_Buffer[4] - '0');
		else if(Rx_Buffer[0] == '-')
		{
			PWM = 1000 * (Rx_Buffer[1] - '0') + 100 * (Rx_Buffer[2] - '0') + 10 * (Rx_Buffer[3] - '0') + (Rx_Buffer[4] - '0');
			PWM = -PWM;
		}	
		else
			PWM = 0;
		MotorRun(LEFT_WHEEL, PWM);
		MotorRun(RIGHT_WHEEL,PWM);
		printf("received -> %s\r\n",Rx_Buffer);
		printf("has setted the PWM:%d\r\n",PWM);
	}
	HAL_UART_Receive_IT(&huart3, (uint8_t *)Rx_Buffer,5);
}
#endif
/**
  * @brief  开启输入捕获，用于捕获固定时间内正交编码器上升沿次数
  * @retval None
  */
void Start_InputCapture()
{
	HAL_TIM_Base_Start_IT(&htim1);	//打开定时器中断
    HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_3);  //打开输入捕获
    HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_4);  //打开输入捕获
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
//    short aacx,aacy,aacz;			//加速度传感器原始数据
//    short gyrox,gyroy,gyroz;		//陀螺仪原始数据
//  float temp;					    //温度
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM4_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	// 开启左右轮编码器的输入捕获
	Start_InputCapture();
	printf("1111111111\r\n");
    Motor_Init();  // 电机初始化，速度置零
	printf("2222222222\r\n");
//	OLED_Init();   //  OLED屏幕初始化
	printf("3333333333\r\n");
	PID_Init();
	printf("4444444444\r\n");
	MPU_Init();			//MPU6050初始化
	printf("5555555555\r\n");
    mpu_dmp_init();		//dmp初始化
	printf("6666666666\r\n");
	OLED_ShowString(2,4,"Succeed!");
	OLED_Refresh();
	HAL_Delay(100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  printf("3333333333\r\n");
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
	  printf("HelloWorld\r\n");
	  HAL_Delay(1000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
