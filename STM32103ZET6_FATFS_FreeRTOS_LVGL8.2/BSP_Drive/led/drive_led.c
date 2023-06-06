#include "drive_led.h"

/*******************************************************************************
* 函 数 名         : LED_Init
* 函数功能		   : LED初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	LED1_PORT_RCC_ENABLE;
	LED2_PORT_RCC_ENABLE;
	
	GPIO_InitStructure.Pin=LED1_PIN; 
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_InitStructure.Pull=GPIO_PULLUP;          //上拉
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
    HAL_GPIO_Init(LED1_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin=LED2_PIN; 
    HAL_GPIO_Init(LED2_PORT,&GPIO_InitStructure);
	
	HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,GPIO_PIN_SET);	
    HAL_GPIO_WritePin(LED2_PORT,LED2_PIN,GPIO_PIN_SET);	
} 
