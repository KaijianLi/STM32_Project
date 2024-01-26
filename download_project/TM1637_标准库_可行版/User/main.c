#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_SysTick.h"
#include "TM1637.h"
void display(int num,unsigned char order[])
{
	static int flag = 0;
	int OK = 1;
	if(num < 9999 && num >= 0) num = num;
	else if(num < 0 && num > -999) num = -num;
	else OK = 0;
	
	if(OK)
	{
		for(int i = 0;i < 4;i++)
		{
			if(i == 2)
			{	
				if(flag) order[3 - i] = tab[num%10]|0x80;
				else     order[3 - i] = tab[num%10];
			}
			else order[3 - i] = tab[num%10];
			num /= 10;
		}	
	}
	else
	{
		for(int i = 0;i < 4;i++)
			order[i] = tab[15];
	}
	TM1637_Display_INC();
    TM1637_SetBrightness( 7 );		//设置亮度等级 0---7
	delay_ms(1000);
	flag = ~flag;
}
int main( void )
{
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );
    LED_GPIO_Config();
    TM1637_Init();

//    delay_config();						//使用方法二延时的时候调用
    while ( 1 )
    {
#if 1 
//		冒号是在第二个数码管的dp引脚接着，
//		dp为最高位，
//		所以给最高位写1就可以点亮时钟的冒号。
		display(43211,disp_num);
#else			//延时精确测试代码

        LED1_ON;
        delay_us( 1 );
        LED1_OFF;
        delay_us( 1 );

#endif
    }

}

