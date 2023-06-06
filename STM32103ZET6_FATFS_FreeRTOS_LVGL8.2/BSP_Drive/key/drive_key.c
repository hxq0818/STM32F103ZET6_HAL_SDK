#include "drive_key.h"
#include "SysTick.h"

/*******************************************************************************
* 函 数 名         : KEY_Init
* 函数功能		   : 按键初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	__HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();           //开启GPIOE时钟
	
	GPIO_InitStructure.Pin=KEY0_PIN|KEY1_PIN|KEY2_PIN; 
    GPIO_InitStructure.Mode=GPIO_MODE_INPUT;  //输入模式
    GPIO_InitStructure.Pull=GPIO_PULLUP;          //上拉
    HAL_GPIO_Init(KEY_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin=KEY_UP_PIN; 
    GPIO_InitStructure.Pull=GPIO_PULLDOWN;          //下拉
    HAL_GPIO_Init(KEY_UP_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名         : KEY_Scan
* 函数功能		   : 按键扫描检测
* 输    入         : mode=0:单次按下按键
					 mode=1：连续按下按键
* 输    出         : 0：未有按键按下
					 KEY_UP_PRESS：KEY_UP键按下
					 KEY0_PRESS：KEY0键按下
					 KEY1_PRESS：KEY1键按下
					 KEY2_PRESS：KEY2键按下
*******************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	
	if(mode==1) //连续按键按下
		key=1;
	if(key==1&&(KEY_UP==1||KEY0==0||KEY1==0||KEY2==0)) //任意一个按键按下
	{
		delay_ms(10);  //消抖
		key=0;
		if(KEY_UP==1)
			return KEY_UP_PRESS; 
		else if(KEY0==0)
			return KEY0_PRESS; 
		else if(KEY1==0)
			return KEY1_PRESS; 
		else if(KEY2==0)
			return KEY2_PRESS; 
	}
	else if(KEY_UP==0&&KEY0==1&&KEY1==1&&KEY2==1)    //无按键按下
		key=1;
	return 0;
}
