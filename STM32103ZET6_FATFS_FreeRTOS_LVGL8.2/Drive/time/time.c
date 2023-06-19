#include "time.h"
#include "led.h"
#include "lvgl.h"

/*******************************************************************************
* 函 数 名         : TIM6_Init
* 函数功能		   : TIM6初始化函数
* 输    入         : per:重装载值
					 psc:分频系数
* 输    出         : 无
*******************************************************************************/
void TIM6_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//使能TIM4时钟
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //开启定时器中断
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;//定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM6,ENABLE); //使能定时器	
}
uint32_t time=0;
/*******************************************************************************
* 函 数 名         : TIM6_IRQHandler
* 函数功能		   : TIM6中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update))
	{
		lv_tick_inc(1);
//		time++;
//		if(time == 1000){
//			time = 0;
//			LED2=!LED2;	
//		}
	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);	
}


