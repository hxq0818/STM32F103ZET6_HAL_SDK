#ifndef _led_H
#define _led_H


#include "system.h"


/*  LED时钟端口、引脚定义 */
#define LED1_PORT 				GPIOB   
#define LED1_PIN 				GPIO_PIN_5
#define LED1_PORT_RCC_ENABLE	__HAL_RCC_GPIOB_CLK_ENABLE()

#define LED2_PORT 				GPIOE   
#define LED2_PIN 				GPIO_PIN_5
#define LED2_PORT_RCC_ENABLE	__HAL_RCC_GPIOE_CLK_ENABLE()


#define LED1 PBout(5)  	
#define LED2 PEout(5) 

void LED_Init(void);

#endif
