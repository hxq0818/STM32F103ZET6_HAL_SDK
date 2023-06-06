#include "drive_time.h"
#include "lvgl.h"


TIM_HandleTypeDef TIM6_Handler;      //定时器句柄 


//通用定时器3中断初始化
//per：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
void TIM6_Init(u16 per,u16 psc)
{  
    TIM6_Handler.Instance=TIM6;                          //通用定时器3
    TIM6_Handler.Init.Prescaler=psc;                     //分频系数
    TIM6_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM6_Handler.Init.Period=per;                        //自动装载值
    TIM6_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM6_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM6_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}


//定时器底层驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM6)
	{
		__HAL_RCC_TIM6_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM6_IRQn,3,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM6_IRQn);          //开启ITM3中断   
	}
}


//定时器3中断服务函数
void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM6_Handler);
}

//static uint32_t time=0;
//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&TIM6_Handler))
    {
       lv_tick_inc(1);
			 HAL_IncTick();
			
//			time++;
//			if(time == 1000)
//			{
//					 LED2=!LED2;        //LED1反转
//				time = 0;
//			}
    }
}
