#include "drive_time.h"
#include "lvgl.h"


TIM_HandleTypeDef TIM6_Handler;      //��ʱ����� 


//ͨ�ö�ʱ��3�жϳ�ʼ��
//per���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM6_Init(u16 per,u16 psc)
{  
    TIM6_Handler.Instance=TIM6;                          //ͨ�ö�ʱ��3
    TIM6_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM6_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM6_Handler.Init.Period=per;                        //�Զ�װ��ֵ
    TIM6_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM6_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM6_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
}


//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM6)
	{
		__HAL_RCC_TIM6_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM6_IRQn,3,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM6_IRQn);          //����ITM3�ж�   
	}
}


//��ʱ��3�жϷ�����
void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM6_Handler);
}

//static uint32_t time=0;
//�ص���������ʱ���жϷ���������
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&TIM6_Handler))
    {
       lv_tick_inc(1);
			 HAL_IncTick();
			
//			time++;
//			if(time == 1000)
//			{
//					 LED2=!LED2;        //LED1��ת
//				time = 0;
//			}
    }
}
