#include "system.h"



//时钟系统配置函数
//PLL:选择的倍频数，RCC_PLL_MUL2~RCC_PLL_MUL16
//返回值:0,成功;1,失败
void SystemClock_Init(u32 PLL)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure; 
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
    
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    	//时钟源为HSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      	//打开HSE
	RCC_OscInitStructure.HSEPredivValue=RCC_HSE_PREDIV_DIV1;		//HSE预分频
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;					//打开PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;			//PLL时钟源选择HSE
    RCC_OscInitStructure.PLL.PLLMUL=PLL; 							//主PLL倍频因子
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//初始化
	
    if(ret!=HAL_OK) while(1);
    
    //选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;		//设置系统时钟时钟源为PLL
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;				//AHB分频系数为1
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV2; 				//APB1分频系数为2
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV1; 				//APB2分频系数为1
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_2);	//同时设置FLASH延时周期为2WS，也就是3个CPU周期。
		
    if(ret!=HAL_OK) while(1);
}

//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}

