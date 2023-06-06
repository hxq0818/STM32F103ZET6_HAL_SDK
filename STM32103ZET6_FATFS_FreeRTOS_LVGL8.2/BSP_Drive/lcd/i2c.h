#ifndef _i2c_H
#define _i2c_H

/*
*I2C�������Զ�ӻ�һ�ԣ���һ��ΪMCU�������豸����ʱ�ӣ�SCL����
*I2C��SCL,SDA˫�������Բ�������������SCL��ǿ��ʹ���豸�������߽��������ݴ����ٶȣ�ʹ�ý��٣���
*I2C,�ߵ�ƽ���У��͵�ƽæµ��
*I2C,��ʼ�����������豸������
*I2C,Ӧ��˫��������;
*I2C,�Ӹߵ��ͷ��ͣ�
*/

/*���þ�ȷ��ʱ��ʹ��SysTick_Init��ʼ��ʱ����*/

//#define I2C_HARDWARE_ON //32��Ӳ��i2c�����⣬���ã��Լ����뷨����Ū
#define I2C_SOFTWARE_ON

/*---------------------------Ӳ��I2C----------------------------*/

#ifdef I2C_HARDWARE_ON



#define I2C_HARDWARE_IT_ON   ENABLE//DISABLE

#if I2C_HARDWARE_IT_ON==ENABLE

#endif
/*I2C_HARDWARE����*/
#endif

/*---------------------------���I2C----------------------------*/

#ifdef I2C_SOFTWARE_ON

#include "system.h"
#include "SysTick.h"

/*����Ӧ��״̬*/
typedef enum
{
	Nack=0,
	Ack=1
}FunctionalResponse;

/*IO��������*/ 
#define I2C_SCL    								 PBout(6) //SCL
#define I2C_SCL_RCC_GPIO           RCC_APB2Periph_GPIOB
#define I2C_SCL_GPIO               GPIOB
#define I2C_SCL_GPIO_PIN           GPIO_Pin_6

#define I2C_SDA    								 PBout(7) //SDA
#define I2C_SDA_RCC_GPIO           RCC_APB2Periph_GPIOB
#define I2C_SDA_GPIO               GPIOB
#define I2C_SDA_GPIO_PIN           GPIO_Pin_7

/*SCL,SDA��λ����*/
#define _I2C_SCL_HIGH_() 					 (I2C_SCL=1)
#define _I2C_SCL_LOW_() 					 (I2C_SCL=0)
#define _I2C_SDA_HIGH_() 					 (I2C_SDA=1)
#define _I2C_SDA_LOW_() 					 (I2C_SDA=0)

/*
*Role     :��ʼ��I2C
*/
void My_I2C_SOFTWARE_Init(void);
/*
*Role     :��ʼ��I2C_SDAΪ���ģʽ
*/
//static void My_I2C_SOFTWARE_IOOutputInit(void);
/*
*Role     :��ʼ��I2C_SDAΪ����ģʽ
*/
//static void My_I2C_SOFTWARE_IOInputInit(void);
/*
*Role     :I2C����ʼ�ź�
*/
void I2C_SOFTWARE_StartSignal(void);
/*
*Role     :I2C��ֹͣ�ź�
*/
void I2C_SOFTWARE_StopSignal(void);
/*
*Role     :�����ȴ�ACK
*Return   :1Ϊ�õ�ACK��0δ��
*/
uint8_t I2C_SOFTWARE_WaitAck(void);
/*
*Parameter:Data,
*					���͵�����
*Role     :�������ݲ��ж��Ƿ�Ӧ��
*Return   :1Ϊ�õ�ACK��0δ��
*/
uint8_t I2C_SOFTWARE_SendByte(uint8_t Data);
/*
*Parameter:NewResponse,
*					Ӧ������
*Role     :�������ݲ��ж��Ƿ�Ӧ��
*Return   :����
*/
uint8_t I2C_SOFTWARE_ReceiveByte(FunctionalResponse NewResponse);

/*I2C_SOFTWARE����*/
#endif
/**************************************/
#endif
