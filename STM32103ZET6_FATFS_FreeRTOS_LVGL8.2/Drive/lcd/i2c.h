#ifndef _i2c_H
#define _i2c_H

/*
*I2C，单主对多从或一丛，主一般为MCU，由主设备产生时钟（SCL）；
*I2C，SCL,SDA双方都可以操作，但从拉低SCL，强行使主设备待机或者降低其数据传输速度（使用较少）；
*I2C,高电平空闲，低电平忙碌；
*I2C,开始结束信由主设备产生；
*I2C,应答双方都可以;
*I2C,从高到低发送；
*/

/*采用精确定时，使用SysTick_Init初始延时函数*/

//#define I2C_HARDWARE_ON //32的硬件i2c有问题，不用，自己有想法可以弄
#define I2C_SOFTWARE_ON

/*---------------------------硬件I2C----------------------------*/

#ifdef I2C_HARDWARE_ON



#define I2C_HARDWARE_IT_ON   ENABLE//DISABLE

#if I2C_HARDWARE_IT_ON==ENABLE

#endif
/*I2C_HARDWARE结束*/
#endif

/*---------------------------软件I2C----------------------------*/

#ifdef I2C_SOFTWARE_ON

#include "system.h"
#include "SysTick.h"

/*定义应答状态*/
typedef enum
{
	Nack=0,
	Ack=1
}FunctionalResponse;

/*IO操作函数*/ 
#define I2C_SCL    								 PBout(6) //SCL
#define I2C_SCL_RCC_GPIO           RCC_APB2Periph_GPIOB
#define I2C_SCL_GPIO               GPIOB
#define I2C_SCL_GPIO_PIN           GPIO_Pin_6

#define I2C_SDA    								 PBout(7) //SDA
#define I2C_SDA_RCC_GPIO           RCC_APB2Periph_GPIOB
#define I2C_SDA_GPIO               GPIOB
#define I2C_SDA_GPIO_PIN           GPIO_Pin_7

/*SCL,SDA的位操作*/
#define _I2C_SCL_HIGH_() 					 (I2C_SCL=1)
#define _I2C_SCL_LOW_() 					 (I2C_SCL=0)
#define _I2C_SDA_HIGH_() 					 (I2C_SDA=1)
#define _I2C_SDA_LOW_() 					 (I2C_SDA=0)

/*
*Role     :初始化I2C
*/
void My_I2C_SOFTWARE_Init(void);
/*
*Role     :初始化I2C_SDA为输出模式
*/
//static void My_I2C_SOFTWARE_IOOutputInit(void);
/*
*Role     :初始化I2C_SDA为输入模式
*/
//static void My_I2C_SOFTWARE_IOInputInit(void);
/*
*Role     :I2C的起始信号
*/
void I2C_SOFTWARE_StartSignal(void);
/*
*Role     :I2C的停止信号
*/
void I2C_SOFTWARE_StopSignal(void);
/*
*Role     :主机等待ACK
*Return   :1为得到ACK，0未有
*/
uint8_t I2C_SOFTWARE_WaitAck(void);
/*
*Parameter:Data,
*					发送的数据
*Role     :发送数据并判断是否应答
*Return   :1为得到ACK，0未有
*/
uint8_t I2C_SOFTWARE_SendByte(uint8_t Data);
/*
*Parameter:NewResponse,
*					应答类型
*Role     :接收数据并判断是否应答
*Return   :数据
*/
uint8_t I2C_SOFTWARE_ReceiveByte(FunctionalResponse NewResponse);

/*I2C_SOFTWARE结束*/
#endif
/**************************************/
#endif
