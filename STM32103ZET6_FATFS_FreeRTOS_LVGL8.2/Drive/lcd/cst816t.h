
/**************����BվUP��Sangk-Hu****************/

/*****************QQȺ��690367095******************/

/****��עUP��˽�Ż��߼�Ⱥ������ϻ�����������*****/

#ifndef _cst816t_H
#define _cst816t_H

#include "system.h"
#include <stm32f10x.h>


/*CST816T������Ϣ*/
extern uint8_t State;
extern uint16_t X_Axis;
extern uint16_t Y_Axis;

/*IO��������*/ 
#define CST816T_RESET 						 PAout(0)
#define CST816T_RESET_RCC_GPIO     RCC_APB2Periph_GPIOA
#define CST816T_RESET_GPIO         GPIOA
#define CST816T_RESET_GPIO_PIN     GPIO_Pin_0

#define CST816T_IRQ 						 	 PBout(5)
#define CST816T_IRQ_RCC_GPIO     	 RCC_APB2Periph_GPIOB
#define CST816T_IRQ_GPIO         	 GPIOB
#define CST816T_IRQ_GPIO_PIN    	 GPIO_Pin_5
/*SCL,SDA��λ����*/
#define _CST816T_RESET_HIGH_() 		 (CST816T_RESET=1)
#define _CST816T_RESET_LOW_() 		 (CST816T_RESET=0)
/*�ⲿ�жϺ�*/
#define CST816T_EXTI_Line					 EXTI_Line5
#define CST816T_IRQCH_PP					 3
#define CST816T_IRQCH_SP					 3

/*
*Role     :
*         ����CST816T��ʼ��
*/
void CST816T_Init(void);
/*
*Parameter:Addr
*					�Ĵ�����ַ
*Parameter:Data,
*					�洢��ַ
*Role     :
*         ��CST816T��Ӧ�Ĵ�����������
*/
void CST816T_SendByte(uint8_t Addr,uint8_t* Data);
/*
*Parameter:Addr
*					�Ĵ�����ַ
*Parameter:Data,
*					�洢��ַ
*Role     :
*         ��CST816T��Ӧ�Ĵ�����������
*/
void CST816T_ReceiveByte(uint8_t Addr,uint8_t* Data);
/*
*Role     :
*         �õ�����״̬������
*/
void CST816_GetAction(void);
/*
*Role     :
*         ����CST816T��λ
*/
void CST816T_Reset(void);
/*
*Role     :
*         ����ϴ����꣬״̬��Ϣ
*/
void CST816T_ClrBaseDatas(void);

#endif
