#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "usart.h"

int main(void)
{	
	char *s = "��h";
	int i;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	
	uart_init(115200);
	OLED_Init();//��ʼ��OLED  
	OLED_Clear(); 
	while(1) 
	{		
		OLED_Print(15, 1, "Temp-�¶�:26��");
		OLED_Print(30, 4, "���..!?");
		delay_ms(1000);
		
	}	  
}
	

