#ifndef __CSB_H__
#define __CSB_H__	


#define TRIG_RCC		RCC_APB2Periph_GPIOA
#define ECHO_RCC		RCC_APB2Periph_GPIOA

#define TRIG_PIN		GPIO_Pin_15
#define ECHO_PIN		GPIO_Pin_12

#define TRIG_PORT		GPIOA
#define ECHO_PORT		GPIOA

void Ultrasonic_GPIO_Init(void);
void app_ultrasonic_mode(void);
extern float bsp_getUltrasonicDistance(void);
extern void bsp_Ultrasonic_Timer2_Init(void);



#endif
