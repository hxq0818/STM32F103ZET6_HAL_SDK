#include "FreeRTOS_text.h"
#include "drive_led.h"

#include "text.h"


//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define LED1_TASK_PRIO		3
//�����ջ��С	
#define LED1_STK_SIZE 		50  
//������
TaskHandle_t LED1Task_Handler;
//������
void led1_task(void *pvParameters);

//�������ȼ�
#define LVGL_TASK_PRIO		2
//�����ջ��С	
#define LVGL_STK_SIZE 		256  
//������
TaskHandle_t LVGLTask_Handler;
//������
void LVGL_task(void *pvParameters);


void freeRTOS_start_dome(void)
{
			//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}
//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
      
	
    //����LVGL����
    xTaskCreate((TaskFunction_t )LVGL_task,     
                (const char*    )"LVGL_task",   
                (uint16_t       )LVGL_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LVGL_TASK_PRIO,
                (TaskHandle_t*  )&LVGLTask_Handler); 	
	
    //����LED1����
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler); 
				
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
} 
//LED1������
void led1_task(void *pvParameters)
{
//	u8 buf[30];
	pvParameters = pvParameters;
	
    while(1)
    {
        LED1=0;
				
        vTaskDelay(500);
        LED1=1;
//				EN25QXX_Read(buf,0,TEXT_LEN);
//				printf("���յ����ݣ�%s\r\n",buf);
        vTaskDelay(500);
    }
}
//LVGL������
void LVGL_task(void *pvParameters)
{
	pvParameters = pvParameters;
	
	lv_text();
    while(1)
    {
        lv_timer_handler();
    }
}

