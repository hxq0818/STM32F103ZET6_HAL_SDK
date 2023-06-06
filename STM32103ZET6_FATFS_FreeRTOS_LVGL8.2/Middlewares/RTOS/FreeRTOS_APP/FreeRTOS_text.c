#include "FreeRTOS_text.h"
#include "drive_led.h"

#include "text.h"


//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define LED1_TASK_PRIO		3
//任务堆栈大小	
#define LED1_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED1Task_Handler;
//任务函数
void led1_task(void *pvParameters);

//任务优先级
#define LVGL_TASK_PRIO		2
//任务堆栈大小	
#define LVGL_STK_SIZE 		256  
//任务句柄
TaskHandle_t LVGLTask_Handler;
//任务函数
void LVGL_task(void *pvParameters);


void freeRTOS_start_dome(void)
{
			//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}
//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
      
	
    //创建LVGL任务
    xTaskCreate((TaskFunction_t )LVGL_task,     
                (const char*    )"LVGL_task",   
                (uint16_t       )LVGL_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LVGL_TASK_PRIO,
                (TaskHandle_t*  )&LVGLTask_Handler); 	
	
    //创建LED1任务
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler); 
				
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
} 
//LED1任务函数
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
//				printf("接收的数据：%s\r\n",buf);
        vTaskDelay(500);
    }
}
//LVGL任务函数
void LVGL_task(void *pvParameters)
{
	pvParameters = pvParameters;
	
	lv_text();
    while(1)
    {
        lv_timer_handler();
    }
}

