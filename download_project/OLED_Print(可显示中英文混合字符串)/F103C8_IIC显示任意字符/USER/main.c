#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "usart.h"

int main(void)
{	
	char *s = "哈h";
	int i;
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	
	uart_init(115200);
	OLED_Init();//初始化OLED  
	OLED_Clear(); 
	while(1) 
	{		
		OLED_Print(15, 1, "Temp-温度:26℃");
		OLED_Print(30, 4, "哈喽..!?");
		delay_ms(1000);
		
	}	  
}
	

