#include "KeyBoard.h"
unsigned int ROW[4] = {GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14,GPIO_PIN_15};
unsigned int COL[4] = {GPIO_PIN_6 ,GPIO_PIN_5, GPIO_PIN_4 ,GPIO_PIN_3 };
unsigned char result[4][4] = {0};
// 键盘初始化，四行引脚全部输入低电平
void KeyBoard_Init(void)
{
    for(unsigned char row = 0;row < 4;row++)
        HAL_GPIO_WritePin(GPIOB,ROW[row],GPIO_PIN_RESET);
}

void KeyBoard_Scan(void)
{
    for(unsigned char i = 0;i < 4;i++)
    {
        KeyBoard_Init();
        HAL_GPIO_WritePin(GPIOB,ROW[i],GPIO_PIN_SET);
        for(unsigned char j = 0;j < 4;j++)
        {
            unsigned char pin_state;
            pin_state = HAL_GPIO_ReadPin(GPIOB,COL[j]);
            if(pin_state == GPIO_PIN_SET)
            {
                HAL_Delay(50);  // 按键消抖
                if(pin_state == GPIO_PIN_SET)
                    // 该按键开关被按下
                    result[i][j] = 1;
            }
            else
                result[i][j] = 0;
        }
    }

    // for(unsigned char i = 0;i < 4;i++)
    // {
    //     for(unsigned char j = 0;j < 4;j++)
    //     {
    //         if(result[i][j] == 1)
    //         {
    //             // 该按键开关被按下，需要计算引脚编号
    //             HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    //             unsigned char pin_ID = i * 4 + j + 1;
    //             OLED_ShowString(1,1,"Key:");
    //             OLED_ShowNum(1,5,pin_ID);
    //         }
    //         HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
    //     }
    // }
}
