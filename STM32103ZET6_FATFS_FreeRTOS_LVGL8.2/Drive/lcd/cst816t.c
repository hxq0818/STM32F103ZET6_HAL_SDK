
/**************作者B站UP：Sangk-Hu****************/

/*****************QQ群：690367095******************/

/****关注UP后，私信或者加群获得资料或者其他资料*****/

#include "cst816t.h"
#include "i2c.h"
#include "st7789v.h"
#include "SysTick.h"
#include "usart.h"
#include <stdio.h>
//#include "capacitive_hynitron_cst816t_update.h"

uint8_t State=0;
uint16_t X_Axis=0;
uint16_t Y_Axis=0;

void CST816T_GPIOInit(void)
{

	/*创建配置结构体*/
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	GPIO_InitTypeDef CST816T_GPIO_PIN_INIT;
	
	RCC_APB2PeriphClockCmd(CST816T_RESET_RCC_GPIO,ENABLE);
	RCC_APB2PeriphClockCmd(CST816T_IRQ_RCC_GPIO,ENABLE);
	/*启用时钟，以完成GPIO和EXTI通道映射*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	CST816T_GPIO_PIN_INIT.GPIO_Mode=GPIO_Mode_Out_PP;
	CST816T_GPIO_PIN_INIT.GPIO_Pin=CST816T_RESET_GPIO_PIN;
	CST816T_GPIO_PIN_INIT.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(CST816T_RESET_GPIO , &CST816T_GPIO_PIN_INIT);
	
	CST816T_GPIO_PIN_INIT.GPIO_Mode=GPIO_Mode_IPU;
	CST816T_GPIO_PIN_INIT.GPIO_Pin=CST816T_IRQ_GPIO_PIN;
	GPIO_Init(CST816T_IRQ_GPIO , &CST816T_GPIO_PIN_INIT);
	
	//选择GPIO管脚用作外部中断线路
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//EXTI5中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=CST816T_IRQCH_PP;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =CST816T_IRQCH_SP;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器	
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;//选择EXTI通道
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//配置中断反馈模式
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//配置中断产生方式
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;//使能
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_ClearITPendingBit(EXTI_Line5);
	
	//初始化
	_CST816T_RESET_HIGH_();
}

void CST816T_SendByte(uint8_t Addr,uint8_t* Data)
{
	I2C_SOFTWARE_StartSignal();
	I2C_SOFTWARE_SendByte(0x2A);
	I2C_SOFTWARE_SendByte(Addr);
	I2C_SOFTWARE_SendByte(*Data);
	I2C_SOFTWARE_StopSignal();
}

void CST816T_ReceiveByte(uint8_t Addr,uint8_t* Data)
{
	I2C_SOFTWARE_StartSignal();
	I2C_SOFTWARE_SendByte(0x2A);
	I2C_SOFTWARE_SendByte(Addr);
	I2C_SOFTWARE_StartSignal();
	I2C_SOFTWARE_SendByte(0x2B);
	*Data=I2C_SOFTWARE_ReceiveByte(Nack);
	I2C_SOFTWARE_StopSignal();
}

void CST816_GetAction(void)
{
	uint8_t data[5];uint16_t X=0,Y=0;
	
	
	CST816T_ReceiveByte(0x01,data);
	CST816T_ReceiveByte(0x03,data+1);
	CST816T_ReceiveByte(0x04,data+2);
	CST816T_ReceiveByte(0x05,data+3);
	CST816T_ReceiveByte(0x06,data+4);
	
	State=data[0];
	X=(uint16_t)((data[1]&0x0F)<<8)|data[2];//(temp[0]&0X0F)<<4|
	Y=(uint16_t)((data[3]&0x0F)<<8)|data[4];//(temp[2]&0X0F)<<4|
	
	if(X<LCD_W&&Y<LCD_H)
	{
		X_Axis=X;
		Y_Axis=Y;
	}  
}

void CST816T_Reset(void)
{
	_CST816T_RESET_LOW_();
	Correct_DelayMs(10);
	_CST816T_RESET_HIGH_();
	Correct_DelayMs(50);
}

void CST816T_Init(void)
{
	uint8_t ChipID=0;
	uint8_t FwVersion=0;
	My_I2C_SOFTWARE_Init();
	CST816T_GPIOInit();
	CST816T_Reset( );//芯片上电初始化
	CST816T_ReceiveByte(0xa7,&ChipID);
	CST816T_ReceiveByte(0xa9,&FwVersion);
	printf("ChipID:%d\n",ChipID);
	printf("FwVersion:%d\n",FwVersion);
	CST816T_Reset();
}

void CST816T_ClrBaseDatas(void)
{
	State=0;
	X_Axis=0;
	Y_Axis=0;
}

void EXTI9_5_IRQHandler (void)
{
	
	if(EXTI_GetITStatus(CST816T_EXTI_Line)==1)
	{
		CST816_GetAction( );
		printf("Sta:%d,X:%d,Y:%d\t\n",State,X_Axis,Y_Axis);
	}
	EXTI_ClearITPendingBit(CST816T_EXTI_Line);
}
