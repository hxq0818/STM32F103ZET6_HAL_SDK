#include "i2c.h"

#ifdef I2C_SOFTWARE_ON


#include "SysTick.h"

void My_I2C_SOFTWARE_Init(void)
{
	GPIO_InitTypeDef I2C_SOFTWARE_GPIO_PIN_INIT;
	I2C_SOFTWARE_GPIO_PIN_INIT.GPIO_Mode=GPIO_Mode_Out_PP;
	I2C_SOFTWARE_GPIO_PIN_INIT.GPIO_Pin=I2C_SCL_GPIO_PIN;
	I2C_SOFTWARE_GPIO_PIN_INIT.GPIO_Speed=GPIO_Speed_50MHz;
	
	RCC_APB2PeriphClockCmd(I2C_SCL_RCC_GPIO|I2C_SDA_RCC_GPIO,ENABLE);
	
	GPIO_Init(I2C_SCL_GPIO , &I2C_SOFTWARE_GPIO_PIN_INIT);

	_I2C_SCL_HIGH_();
	_I2C_SDA_HIGH_();
}

static void My_I2C_SOFTWARE_IOOutputInit(void)
{
	GPIO_InitTypeDef I2C_SOFTWARE_GPIO_PIN_INIT;
	I2C_SOFTWARE_GPIO_PIN_INIT.GPIO_Mode=GPIO_Mode_Out_PP;
	I2C_SOFTWARE_GPIO_PIN_INIT.GPIO_Pin=I2C_SDA_GPIO_PIN;
	I2C_SOFTWARE_GPIO_PIN_INIT.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(I2C_SDA_GPIO , &I2C_SOFTWARE_GPIO_PIN_INIT);
}

static void My_I2C_SOFTWARE_IOInputInit(void)
{
	GPIO_InitTypeDef I2C_SOFTWARE_GPIO_PIN_INIT;
	I2C_SOFTWARE_GPIO_PIN_INIT.GPIO_Mode=GPIO_Mode_IPU;
	I2C_SOFTWARE_GPIO_PIN_INIT.GPIO_Pin=I2C_SDA_GPIO_PIN;
	I2C_SOFTWARE_GPIO_PIN_INIT.GPIO_Speed=GPIO_Speed_50MHz;
		
	GPIO_Init(I2C_SDA_GPIO , &I2C_SOFTWARE_GPIO_PIN_INIT);
}

void I2C_SOFTWARE_StartSignal(void)
{
	My_I2C_SOFTWARE_IOOutputInit();
	_I2C_SCL_LOW_();
	_I2C_SDA_HIGH_();
	_I2C_SCL_HIGH_();
	Correct_DelayUs(5);
	_I2C_SDA_LOW_() ;
	Correct_DelayUs(5);
	_I2C_SCL_LOW_();
}

void I2C_SOFTWARE_StopSignal(void)
{
	My_I2C_SOFTWARE_IOOutputInit();
	_I2C_SCL_LOW_();
	_I2C_SDA_LOW_();
	_I2C_SCL_HIGH_();
	Correct_DelayUs(5);
	_I2C_SDA_HIGH_();
	Correct_DelayUs(5);
}

void I2C_SOFTWARE_AckSignal(void)
{
	My_I2C_SOFTWARE_IOOutputInit();
	_I2C_SCL_LOW_();
	_I2C_SDA_LOW_();
	Correct_DelayUs(5);
	_I2C_SCL_HIGH_();
	Correct_DelayUs(5);
	_I2C_SCL_LOW_();
}

void I2C_SOFTWARE_NackSignal(void)
{
	My_I2C_SOFTWARE_IOOutputInit();
	_I2C_SCL_LOW_();
	_I2C_SDA_HIGH_();
	Correct_DelayUs(5);
	_I2C_SCL_HIGH_();
	Correct_DelayUs(5);
//	_I2C_SCL_LOW_();
}

uint8_t I2C_SOFTWARE_WaitAck(void)
{
	u8 wait;
	My_I2C_SOFTWARE_IOOutputInit();
	_I2C_SDA_HIGH_();
	My_I2C_SOFTWARE_IOInputInit();
	_I2C_SCL_HIGH_();
	Correct_DelayUs(5);
	while(GPIO_ReadInputDataBit(I2C_SDA_GPIO, I2C_SDA_GPIO_PIN))
	{
		wait++;
		if(wait>200)
		{
			I2C_SOFTWARE_StopSignal();
			return 0;
		}
	}
	_I2C_SCL_LOW_();
	return 1;
}

void I2C_SOFTWARE_HighSignal(void)
{
	_I2C_SCL_LOW_();
	_I2C_SDA_HIGH_();
	Correct_DelayUs(5);
	_I2C_SCL_HIGH_();
	Correct_DelayUs(5);
	_I2C_SCL_LOW_();
}

void I2C_SOFTWARE_LowSignal(void)
{
	_I2C_SCL_LOW_();
	_I2C_SDA_LOW_();
	Correct_DelayUs(5);
	_I2C_SCL_HIGH_();
	Correct_DelayUs(5);
	_I2C_SCL_LOW_();
}

uint8_t I2C_SOFTWARE_SendByte(uint8_t Data)
{
	uint8_t i;
	My_I2C_SOFTWARE_IOOutputInit();
	for(i=0x80;i!=0;i>>=1)
	{
		if(Data&i)
		{
			I2C_SOFTWARE_HighSignal();
		}
		else
		{
			I2C_SOFTWARE_LowSignal();
		}
	}
	return(I2C_SOFTWARE_WaitAck());
}

uint8_t I2C_SOFTWARE_ReceiveByte(FunctionalResponse NewResponse)
{
	uint8_t data=0,i;
	My_I2C_SOFTWARE_IOInputInit();
	_I2C_SCL_HIGH_();
	Correct_DelayUs(5);
	for(i=0x80;i!=0;i>>=1)
	{
		if(GPIO_ReadInputDataBit(I2C_SDA_GPIO, I2C_SDA_GPIO_PIN)==1)
		{
			data|=i;
		}
		Correct_DelayUs(5);
		_I2C_SCL_LOW_();
		Correct_DelayUs(5);
		_I2C_SCL_HIGH_();
		Correct_DelayUs(5);
	}	
	if(NewResponse==Ack)
	{
		I2C_SOFTWARE_AckSignal();
	}
	else
	{
		I2C_SOFTWARE_NackSignal();
	}
	return data;
}

/*I2C_SOFTWARE½áÊø*/
#endif

