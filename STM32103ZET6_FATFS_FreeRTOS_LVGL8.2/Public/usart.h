#ifndef _usart_H
#define _usart_H


#include "system.h"
#include "stdio.h"


#define USART1_REC_LEN  			200  	//定义最大接收字节数 200
	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         		//接收状态标记	
extern UART_HandleTypeDef UART1_Handler; //UART句柄

#define RXBUFFERSIZE   1 //缓存大小
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL库USART接收Buffer

void USART1_Init(u32 bound);



#endif
