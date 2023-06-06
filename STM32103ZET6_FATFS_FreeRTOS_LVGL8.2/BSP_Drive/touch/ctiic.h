#ifndef _ctiic_H
#define _ctiic_H


#include "system.h"


//IO��������	 
#define CT_IIC_SCL  PBout(1)  //SCL
#define CT_IIC_SDA  PFout(9)  //SDA	 
#define CT_READ_SDA PFin(9)   //����SDA  
 

//IIC���в�������
void CT_IIC_Init(void);                	//��ʼ��IIC��IO��				 
void CT_IIC_Start(void);				//����IIC��ʼ�ź�
void CT_IIC_Stop(void);	  				//����IICֹͣ�ź�
void CT_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 CT_IIC_Read_Byte(unsigned char ack);	//IIC��ȡһ���ֽ�
u8 CT_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void CT_IIC_Ack(void);					//IIC����ACK�ź�
void CT_IIC_NAck(void);					//IIC������ACK�ź�






#endif
