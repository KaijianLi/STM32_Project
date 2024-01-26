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
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED_SPI.h"
#include "bmp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
 OLED_DisPlay_On();
 OLED_Init();
 OLED_ColorTurn(0);//0正常显示，1 反色显示
 OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
 //HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_SET);
 //HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
//        OLED_ShowPicture(0,0,128,64,BMP1,1);
		OLED_Refresh();
		HAL_Delay(100);
	    int i = 0,x_pos = 0;
	  for(i = 0;i < 7;i++)  // 邓趁婷门门分数
	  { 
		  int num = i;
		  if(i >= 4) num = i - 1;
          OLED_ShowChinese(x_pos,0,num,16,1);
		  x_pos += 16;
	  }
	  OLED_ShowChar(x_pos,0,':',16,1);
	  OLED_ShowString(x_pos + 5,0,"95+",16,1);
	  OLED_Refresh();
	  HAL_Delay(100);
//		OLED_ShowChinese(  0, 0, 0, 16, 1);//中
//		OLED_ShowChinese( 18, 0, 1, 16, 1);//景
//		OLED_ShowChinese( 36, 0, 2, 16, 1);//园
//		OLED_ShowChinese( 54, 0, 4, 16, 1);//子
//		OLED_ShowChinese( 72, 0, 5, 16, 1);//技
//		OLED_ShowChar(90,0,':',16,1);//显示ASCII字符
//		OLED_ShowString(100,0,"95+",16,1);
//		OLED_ShowChinese(  0, 16, 6, 16, 1);//中
//		OLED_ShowChinese( 18, 16, 7, 16, 1);//景
//		OLED_ShowChinese( 36, 16, 8, 16, 1);//园
//		OLED_ShowChinese( 54, 16, 9, 16, 1);//子
//		OLED_ShowChar(72,16,':',16,1);//显示ASCII字符
//		OLED_ShowString(90,16,"650+",16,1);
//		OLED_ShowString(0,32,"computer excellent",12,1);
//		OLED_ShowString(20,32,"2014/05/01",16,1);
//		OLED_ShowString(0,48,"ASCII:",16,1);  
//		OLED_ShowString(63,48,"CODE:",16,1);
//	  
//		OLED_ShowChar(48,48,t,16,1);//显示ASCII字符	
//		
//		t++;
//		if(t > '~') t = ' ';
//		OLED_ShowNum(103,48,t,3,16,1);
//		OLED_Refresh();
//		HAL_Delay(500);
//		OLED_Clear();
//		OLED_ShowChinese( 0,  0, 0, 16, 1);  //16*16 华
//	    OLED_ShowChinese(16,  0, 1, 16, 1);  //24*24 中
//		OLED_ShowChinese(32,  0, 2, 16, 1);  //32*32 科
//	    OLED_ShowChinese(48,  0, 3, 16, 1);  //64*64 技
//		OLED_ShowChinese(64,  0, 4, 16, 1);  //64*64 大
//		OLED_ShowChinese(80,  0, 5, 16, 1);  //64*64 学
//		OLED_Refresh();
//	    HAL_Delay(500);
//  	    OLED_Clear();
//		OLED_ShowString(0,  0, "HUST",  8, 1);//6*8 “ABC”
//		OLED_ShowString(0,  8, "HUST", 12, 1);//6*12 “ABC”
//	    OLED_ShowString(0, 20, "HUST", 16, 1);//8*16 “ABC”
//		OLED_ShowString(0, 36, "HUST", 24, 1);//12*24 “ABC”
//	    OLED_Refresh();
//		HAL_Delay(500);
//		char *string = "I don\'t want to study!!!!!";
//		OLED_ScrollDisplay_String(0,string,strlen(string),2,1);
		//OLED_ScrollDisplay_Chinese(0,6,1,1);
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
