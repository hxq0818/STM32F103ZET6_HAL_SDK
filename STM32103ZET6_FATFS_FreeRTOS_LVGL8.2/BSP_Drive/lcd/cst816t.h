
/**************作者B站UP：Sangk-Hu****************/

/*****************QQ群：690367095******************/

/****关注UP后，私信或者加群获得资料或者其他资料*****/

#ifndef _cst816t_H
#define _cst816t_H

#include "system.h"
#include <stm32f10x.h>


/*CST816T基本信息*/
extern uint8_t State;
extern uint16_t X_Axis;
extern uint16_t Y_Axis;

/*IO操作函数*/ 
#define CST816T_RESET 						 PAout(0)
#define CST816T_RESET_RCC_GPIO     RCC_APB2Periph_GPIOA
#define CST816T_RESET_GPIO         GPIOA
#define CST816T_RESET_GPIO_PIN     GPIO_Pin_0

#define CST816T_IRQ 						 	 PBout(5)
#define CST816T_IRQ_RCC_GPIO     	 RCC_APB2Periph_GPIOB
#define CST816T_IRQ_GPIO         	 GPIOB
#define CST816T_IRQ_GPIO_PIN    	 GPIO_Pin_5
/*SCL,SDA的位操作*/
#define _CST816T_RESET_HIGH_() 		 (CST816T_RESET=1)
#define _CST816T_RESET_LOW_() 		 (CST816T_RESET=0)
/*外部中断宏*/
#define CST816T_EXTI_Line					 EXTI_Line5
#define CST816T_IRQCH_PP					 3
#define CST816T_IRQCH_SP					 3

/*
*Role     :
*         对于CST816T初始化
*/
void CST816T_Init(void);
/*
*Parameter:Addr
*					寄存器地址
*Parameter:Data,
*					存储地址
*Role     :
*         向CST816T对应寄存器发送数据
*/
void CST816T_SendByte(uint8_t Addr,uint8_t* Data);
/*
*Parameter:Addr
*					寄存器地址
*Parameter:Data,
*					存储地址
*Role     :
*         向CST816T对应寄存器接收数据
*/
void CST816T_ReceiveByte(uint8_t Addr,uint8_t* Data);
/*
*Role     :
*         得到手势状态，坐标
*/
void CST816_GetAction(void);
/*
*Role     :
*         对于CST816T复位
*/
void CST816T_Reset(void);
/*
*Role     :
*         清除上次坐标，状态信息
*/
void CST816T_ClrBaseDatas(void);

#endif
