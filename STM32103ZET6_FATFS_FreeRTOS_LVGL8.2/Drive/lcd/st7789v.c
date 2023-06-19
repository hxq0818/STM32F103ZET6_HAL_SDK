
/**************����BվUP��Sangk-Hu****************/

/*****************QQȺ��690367095******************/

/****��עUP��˽�Ż��߼�Ⱥ������ϻ�����������*****/

#include "st7789v.h"
#include "font.h"
#include "SysTick.h"


/*��¼��һ�δ������ݳ���*/
uint16_t Tx_Len=0;

/*
*Role     :
*         ���ڶ���SPI��ʼ��
*/
void ST7789V_SPIInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(ST7789V_SCK_RCC_GPIO|ST7789V_SDA_RCC_GPIO|ST7789V_RCC_SPIX, ENABLE);//ʹ��GPIOA,ʹ��SPI1

	GPIO_InitStructure.GPIO_Pin =ST7789V_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 //�����������
	GPIO_Init(ST7789V_SCK_GPIO, &GPIO_InitStructure); //��ʼ����Ӧ�˽ӿ�
	GPIO_SetBits(ST7789V_SCK_GPIO,ST7789V_SCK_PIN);//�˿������ʼ��Ϊ1
	
	GPIO_InitStructure.GPIO_Pin =ST7789V_SDA_PIN;
	GPIO_Init(ST7789V_SDA_GPIO, &GPIO_InitStructure); //��ʼ����Ӧ�˽ӿ�
	GPIO_SetBits(ST7789V_SDA_GPIO,ST7789V_SDA_PIN);

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;//ֻ����ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����SPI����ģʽ������ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//����SPI���ݴ�С��8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//����ͬ��ʱ�ӿ���ʱSCLKλ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//����ͬ��ʱ�ӿյڶ���ʱ���ز���
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS�ź���Ӳ������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//������Ԥ��Ƶֵ��������Ԥ��ƵֵΪ4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ݴ����λ����
	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRCֵ����Ķ���ʽ
	SPI_Init(ST7789V_SPIX,&SPI_InitStructure);//��ʼ��SPI
	SPI_Cmd(ST7789V_SPIX, ENABLE);//ʹ��SPI
}

#ifdef ST7789V_SPIX_DMA

/*
*Parameter:Madr
*					�洢��ַ
*Parameter:DMA_MemoryInc,
*					�洢��ַ�Ƿ����
*Parameter:Len,
*					�������ݳ���
*Role     :
*         ��ʼ��ǰӦ�ó����µ�DMA����
*/
void ST7789V_DMAInit(uint32_t Madr,uint32_t DMA_MemoryInc,uint32_t DataSize,uint16_t Len)
{
	DMA_InitTypeDef  DMA_InitStructure;
	//ʹ�� DMA ��������DMA1 �� DMA2��ʱ��
	RCC_AHBPeriphClockCmd(ST7789V_RCC_DMA, ENABLE);//DMA1ʱ��ʹ�� 
	
	DMA_DeInit(ST7789V_DMA_SPIX_TX_CH);
	
	/* ���� DMA */
	//DMA�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr =ST7789V_SPIX_ADD;
	//DMA �洢����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr =Madr;
	//�洢��������ģʽ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	//���ݴ����� 
	DMA_InitStructure.DMA_BufferSize =Len;
	Tx_Len=Len;
	//���������ģʽ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//�洢������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc;
	if(DataSize==DMA_PeripheralDataSize_Byte||DataSize==DMA_MemoryDataSize_Byte)
	{
		//�������ݳ���
		DMA_InitStructure.DMA_PeripheralDataSize = DataSize;
		//�洢�����ݳ���
		DMA_InitStructure.DMA_MemoryDataSize = DataSize;
	}
	else if(DataSize==DMA_PeripheralDataSize_HalfWord||DataSize==DMA_MemoryDataSize_HalfWord)
	{
		//�������ݳ���
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		//�洢�����ݳ���
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	}
	else 
	{
		//�������ݳ���
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
		//�洢�����ݳ���
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	}
	// ʹ����ͨģʽ 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//�е����ȼ�
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	//��ʼ��DMA 
	DMA_Init(ST7789V_DMA_SPIX_TX_CH, &DMA_InitStructure);
}

/*
*Parameter:Len
*					�������ݳ���
*Role     :
*         ʹ��DMA
*/
void ST7789V_DMAEnable(uint16_t Len)
{
	DMA_Cmd(ST7789V_DMA_SPIX_TX_CH, DISABLE);                      //�ر�DMA���� 
	DMA_SetCurrDataCounter(ST7789V_DMA_SPIX_TX_CH,Len);          //���ݴ����� 
	SPI_Cmd(ST7789V_SPIX, ENABLE);//ʹ��SPI	
	SPI_I2S_DMACmd(ST7789V_SPIX, SPI_I2S_DMAReq_Tx, ENABLE);			//ʹ��spi��dma����
	DMA_Cmd(ST7789V_DMA_SPIX_TX_CH, ENABLE);                      //����DMA���� 
}

#endif

void ST7789V_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	/* SPI��IO�ں�SPI�����ʱ�� */
  RCC_APB2PeriphClockCmd(	ST7789V_RESET_RCC_GPIO|ST7789V_RS_RCC_GPIO|ST7789V_NSS_RCC_GPIO, ENABLE );

//	/* SPI��IO������ */
	GPIO_InitStructure.GPIO_Pin = ST7789V_RESET_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ST7789V_RESET_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(ST7789V_RESET_GPIO,ST7789V_RESET_PIN);
	
	GPIO_InitStructure.GPIO_Pin = ST7789V_RS_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ST7789V_RS_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(ST7789V_RS_GPIO,ST7789V_RS_PIN);
	
	GPIO_InitStructure.GPIO_Pin = ST7789V_NSS_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ST7789V_NSS_GPIO, &GPIO_InitStructure);
	//�˿������ʼ��Ϊ1
	GPIO_SetBits(ST7789V_NSS_GPIO,ST7789V_NSS_PIN);
}

//SPI2�ٶ����ú���
//SPI�ٶ�=fAPB1/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB1ʱ��һ��Ϊ36Mhz��
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	ST7789V_SPIX->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	ST7789V_SPIX->CR1|=SPI_BaudRatePrescaler;	//����SPI�ٶ� 
	SPI_Cmd(ST7789V_SPIX,ENABLE); //ʹ��SPI2
} 
/*SPI���ͺ���*/
void delay(int t)
{
	while(t--);
}

void ST7789V_SPISend(uint8_t Data)
{
	ST7789V_NSS_LOW();
	while (SPI_I2S_GetFlagStatus(ST7789V_SPIX, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(ST7789V_SPIX,Data);
	delay(1);//ע���ӳ�
	ST7789V_NSS_HIGH();
}

void ST7789V_SPISendDataByte(uint8_t Data)
{
	ST7789V_SPISend(Data);
}

void ST7789V_SPISendData2Bytes(uint16_t Data)
{
	ST7789V_SPISend(Data>>8);
	ST7789V_SPISend(Data);
}

void ST7789V_SPISendCmdByte(uint8_t Data)
{
	ST7789V_RS_LOW() ;
	ST7789V_SPISend(Data);
	ST7789V_RS_HIGH() ;
}
/*ST7789VӦ�ú���*/
void ST7789V_SendOneColor(uint16_t Color)
{
	ST7789V_SPISendData2Bytes(Color);
}

void ST7789V_SetLcdAddress(uint16_t Start_X,uint16_t Start_Y,uint16_t End_X,uint16_t End_Y,uint8_t Dir_Mode)
{
	if(Dir_Mode==0)
	{
		ST7789V_SPISendCmdByte(0x2a);//�е�ַ����
		ST7789V_SPISendData2Bytes(Start_X+WIDTH_OFFSET);
		ST7789V_SPISendData2Bytes(End_X+WIDTH_OFFSET);
		ST7789V_SPISendCmdByte(0x2b);//�е�ַ����
		ST7789V_SPISendData2Bytes(Start_Y+HIGH_OFFSET);
		ST7789V_SPISendData2Bytes(End_Y+HIGH_OFFSET);
		ST7789V_SPISendCmdByte(0x2c);//������д
	}
	else if(Dir_Mode==1)
	{
		ST7789V_SPISendCmdByte(0x2a);//�е�ַ����
		ST7789V_SPISendData2Bytes(Start_X+WIDTH_OFFSET);
		ST7789V_SPISendData2Bytes(End_X+WIDTH_OFFSET);
		ST7789V_SPISendCmdByte(0x2b);//�е�ַ����
		ST7789V_SPISendData2Bytes(Start_Y+HIGH_OFFSET);
		ST7789V_SPISendData2Bytes(End_Y+HIGH_OFFSET);
		ST7789V_SPISendCmdByte(0x2c);//������д
	}
	else if(Dir_Mode==2)
	{
		ST7789V_SPISendCmdByte(0x2a);//�е�ַ����
		ST7789V_SPISendData2Bytes(Start_X+HIGH_OFFSET);
		ST7789V_SPISendData2Bytes(End_X+HIGH_OFFSET);
		ST7789V_SPISendCmdByte(0x2b);//�е�ַ����
		ST7789V_SPISendData2Bytes(Start_Y+WIDTH_OFFSET);
		ST7789V_SPISendData2Bytes(End_Y+WIDTH_OFFSET);
		ST7789V_SPISendCmdByte(0x2c);//������д
	}
	else
	{
		ST7789V_SPISendCmdByte(0x2a);//�е�ַ����
		ST7789V_SPISendData2Bytes(Start_X+HIGH_OFFSET);
		ST7789V_SPISendData2Bytes(End_X+HIGH_OFFSET);
		ST7789V_SPISendCmdByte(0x2b);//�е�ַ����
		ST7789V_SPISendData2Bytes(Start_Y+WIDTH_OFFSET);
		ST7789V_SPISendData2Bytes(End_Y+WIDTH_OFFSET);
		ST7789V_SPISendCmdByte(0x2c);//������д
	}
}

void ST7789V_Reset(void)
{
	ST7789V_RESET_LOW();  //��λ
	Correct_DelayMs(100);
	ST7789V_RESET_HIGH();
	Correct_DelayMs(100);
}

void ST7789V_SetDir(uint8_t Dir_Mode)
{
	ST7789V_SPISendCmdByte(0x36);/*��ʾ����*/ 
	if(Dir_Mode==0)ST7789V_SPISendDataByte(0x00);
	else if(Dir_Mode==1)ST7789V_SPISendDataByte(0xC0);
	else if(Dir_Mode==2)ST7789V_SPISendDataByte(0x70);
	else ST7789V_SPISendDataByte(0xA0);
}

void ST7789V_LcdInit(void)
{
	ST7789V_GPIOInit();		//��ʼ��GPIO
	ST7789V_SPIInit();    //��ʼ��SPI1
	
	ST7789V_Reset();//��λ
	
	ST7789V_SPISendCmdByte(0x11); 
	Correct_DelayMs(120); 

	ST7789V_SetDir(USE_HORIZONTAL);
	
	/*12λ��ɫΪ03h��16λ��ɫλ05h��18λΪ06h����������ɫ���࣬��ʾ����*/
	ST7789V_SPISendCmdByte(0x3A);
	ST7789V_SPISendDataByte(0x55);
//-------------ST7789V Frame rate setting-----------//
	ST7789V_SPISendCmdByte(0xB2);//Porch Setting
	ST7789V_SPISendDataByte(0x0C);
	ST7789V_SPISendDataByte(0x0C);
	ST7789V_SPISendDataByte(0x00);
	ST7789V_SPISendDataByte(0x33);
	ST7789V_SPISendDataByte(0x33); 
	
	ST7789V_SPISendCmdByte(0xB7);//Gate Control
	ST7789V_SPISendDataByte(0x75);//������������12.2v   -10.43v  
//--------------ST7789V Power setting---------------//
	ST7789V_SPISendCmdByte(0xC2);//VDV and VRH Command Enable
	ST7789V_SPISendDataByte(0x01);

	ST7789V_SPISendCmdByte(0xC3);//VRH Set
	ST7789V_SPISendDataByte(0x16);//������������4.3+( vcom+vcom offset+vdv)   

	ST7789V_SPISendCmdByte(0xC4);//VDV Set
	ST7789V_SPISendDataByte(0x20); //0v 

	ST7789V_SPISendCmdByte(0xC6);//Frame Rate Control in Normal Mode 
	ST7789V_SPISendDataByte(0x0F);//111Hz   

	ST7789V_SPISendCmdByte(0xD0);//Power Control 1 
	ST7789V_SPISendDataByte(0xA4);
	ST7789V_SPISendDataByte(0xA1);
	
	ST7789V_SPISendCmdByte(0xD6);
	ST7789V_SPISendDataByte(0xA1);
	
	ST7789V_SPISendCmdByte(0xBB);//VCOM
	ST7789V_SPISendDataByte(0x3B);
//---------------ST7789V gamma setting-------------//
	ST7789V_SPISendCmdByte(0xE0);//Set Gamma
	ST7789V_SPISendDataByte(0xF0);
	ST7789V_SPISendDataByte(0x0B);
	ST7789V_SPISendDataByte(0x11);
	ST7789V_SPISendDataByte(0x0E);
	ST7789V_SPISendDataByte(0x0D);
	ST7789V_SPISendDataByte(0x19);
	ST7789V_SPISendDataByte(0x36);
	ST7789V_SPISendDataByte(0x33);
	ST7789V_SPISendDataByte(0x4B);
	ST7789V_SPISendDataByte(0x07);
	ST7789V_SPISendDataByte(0x14);
	ST7789V_SPISendDataByte(0x14);
	ST7789V_SPISendDataByte(0x2C);
	ST7789V_SPISendDataByte(0x2E);    

	ST7789V_SPISendCmdByte(0xE1);//Set Gamma
	ST7789V_SPISendDataByte(0xF0);
	ST7789V_SPISendDataByte(0x0D);
	ST7789V_SPISendDataByte(0x12);
	ST7789V_SPISendDataByte(0x0B);
	ST7789V_SPISendDataByte(0x09);
	ST7789V_SPISendDataByte(0x03);
	ST7789V_SPISendDataByte(0x32);
	ST7789V_SPISendDataByte(0x44);
	ST7789V_SPISendDataByte(0x48);
	ST7789V_SPISendDataByte(0x39);
	ST7789V_SPISendDataByte(0x16);
	ST7789V_SPISendDataByte(0x16);
	ST7789V_SPISendDataByte(0x2d);
	ST7789V_SPISendDataByte(0x30);

	ST7789V_SPISendCmdByte(0xE4);
	ST7789V_SPISendDataByte(0x25);
	ST7789V_SPISendDataByte(0x00);
	ST7789V_SPISendDataByte(0x00);

	ST7789V_SPISendCmdByte(0x21);//���������ڴ���ʾ��תģʽ�ָ���

	ST7789V_SPISendCmdByte(0x29);//��ʾ��
	
	ST7789V_SPISendCmdByte(0x2c);
	/*
���������ڽ����ݴ�MCU���䵽֡�洢����
-���ܴ�����ʱ���мĴ�����ҳ��Ĵ���������Ϊ��ʼ��/��ʼ
ҳ��λ�á�
-���� MADCTL ���ã���ʼ��/��ʼҳλ�ò�ͬ��
- �����κ������������ֹͣ֡д�롣
	*/
	
}

uint16_t ST7789V_VerticalScrollMode(uint16_t TopStill_H,uint16_t VerticalScroll_H,uint16_t BottomStill_H,uint16_t StartAddr,uint16_t ScrollTime)
{
	static uint16_t i=0;
	static uint8_t k=1;
	if(k==1)
	{
		k=0;
		if((TopStill_H+VerticalScroll_H+BottomStill_H)!=320)return VerticalScroll_H+1;
		ST7789V_SPISendCmdByte(0x33);//���ù��������ĸ�������߶ȣ����������=320
		ST7789V_SPISendData2Bytes(TopStill_H);
		ST7789V_SPISendData2Bytes(VerticalScroll_H);
		ST7789V_SPISendData2Bytes(BottomStill_H);
		ST7789V_SPISendCmdByte(0x37);
		ST7789V_SPISendData2Bytes(StartAddr);
		i=StartAddr;
	}
	if(i++>VerticalScroll_H+TopStill_H)
	{
		i=TopStill_H;
	}
	ST7789V_SPISendCmdByte(0x37);
	ST7789V_SPISendData2Bytes(i);
	Correct_DelayMs(ScrollTime);
	return i;
}

void ST7789V_PartialDisplayModeCmd(FunctionalState NewState,uint16_t Start_Line,uint16_t End_Line)
{
	if(NewState==ENABLE)
	{
		ST7789V_SPISendCmdByte(0x13);
	}
	else
	{
		ST7789V_SPISendCmdByte(0x30);
		ST7789V_SPISendDataByte((uint8_t)Start_Line>>8);
		ST7789V_SPISendDataByte((uint8_t)Start_Line);
		ST7789V_SPISendDataByte((uint8_t)End_Line>>8);
		ST7789V_SPISendDataByte((uint8_t)End_Line);
		ST7789V_SPISendCmdByte(0x12);
	}
}

void ST7789V_IdleModeCmd(FunctionalState NewState)
{
	if(NewState==ENABLE)
	{
		ST7789V_SPISendCmdByte(0x39);
	}
	else if(NewState==DISABLE)
	{
		ST7789V_SPISendCmdByte(0x38);
	}
}

void ST7789V_DisplayCmd(FunctionalState NewState)
{
	if(NewState==ENABLE)
	{
		ST7789V_SPISendCmdByte(0x29);
	}
	else if(NewState==DISABLE)
	{
		ST7789V_SPISendCmdByte(0x28);
	}
}

void ST7789V_SleepModeCmd(FunctionalState NewState)
{
	if(NewState==ENABLE)
	{
		ST7789V_SPISendCmdByte(0x10);
		Correct_DelayMs(120);
	}
	else if(NewState==DISABLE)
	{
		ST7789V_SPISendCmdByte(0x11);
		Correct_DelayMs(120);
	}
}

void ST7789V_AdaptiveBrightnessColorEnhancementInit(WRCACE_InitTypedef*WRCACE_InitStruct)
{
	ST7789V_SPISendCmdByte(0x55);
	ST7789V_SPISendDataByte((WRCACE_InitStruct->Color_Enhancement_Extent<<4)|(WRCACE_InitStruct->Color_Enhancement_Cmd<<7)|WRCACE_InitStruct->Adaptive_Brightness_Goal);
}

void ST7789V_FillLcdScreen(uint16_t Start_X,uint16_t Start_Y,uint16_t End_X,uint16_t End_Y,uint16_t* Color,uint8_t Dir_Mode)
{          
	uint32_t num;
	num=(End_X-Start_X)*(End_Y-Start_Y);
	ST7789V_SetLcdAddress(Start_X,Start_Y,End_X-1,End_Y-1,Dir_Mode);//������ʾ��Χ
	
	#if SPI_DMA
	{
		uint32_t num1,num2;
		ST7789V_SPIX->CR1|=1<<11;//����SPI16λ����ģʽ
		if(num>65535)
		{
			num1=65535;
			num2=num-num1;
			ST7789V_NSS_LOW();
			ST7789V_DMAInit((uint32_t)(&Color),DMA_MemoryInc_Disable,DMA_MemoryDataSize_HalfWord,num1);
			ST7789V_DMAEnable(num1);
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_TC3)!=RESET)//�ȴ�ͨ��4�������
				{
					DMA_ClearFlag(DMA1_FLAG_TC3);//���ͨ��3������ɱ�־
					break;
				}
			}
			ST7789V_DMAEnable(num2);
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_TC3)!=RESET)//�ȴ�ͨ��4�������
				{
					DMA_ClearFlag(DMA1_FLAG_TC3);//���ͨ��3������ɱ�־
					break;
				}
			}
		}
		else
		{
			ST7789V_NSS_LOW();
			ST7789V_DMAInit((uint32_t)(&Color),DMA_MemoryInc_Disable,DMA_MemoryDataSize_HalfWord,num);
			ST7789V_DMAEnable(num);
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_TC3)!=RESET)//�ȴ�ͨ��4�������
				{
					DMA_ClearFlag(DMA1_FLAG_TC3);//���ͨ��3������ɱ�־
					break;
				}
			}
		}
		ST7789V_NSS_HIGH();
		ST7789V_SPIX->CR1=~ST7789V_SPIX->CR1;
		ST7789V_SPIX->CR1|=1<<11;
		ST7789V_SPIX->CR1=~ST7789V_SPIX->CR1;//����SPI8λ����ģʽ
		SPI_Cmd(ST7789V_SPIX, ENABLE);//ʹ��SPI
	}
	#else
	{
		uint32_t i;
		for(i=0;i<num;i++)
		{
			ST7789V_SPISendData2Bytes(Color[i]);
		}
	}
	#endif
	
}

void ST7789V_LcdDrawPoint(uint16_t X,uint16_t Y,uint16_t Color,uint8_t Dir_Mode)
{
	ST7789V_SetLcdAddress(X,Y,X,Y,Dir_Mode);//���ù��λ�� 
	ST7789V_SPISendData2Bytes(Color);
} 

void ST7789V_LcdDrawLine(uint16_t Start_X,uint16_t Start_Y,uint16_t End_X,uint16_t End_Y,uint16_t Color,uint8_t Dir_Mode)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=End_X-Start_X; //������������ 
	delta_y=End_Y-Start_Y;
	uRow=Start_X;//�����������
	uCol=Start_Y;
	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		ST7789V_LcdDrawPoint(uRow,uCol,Color,Dir_Mode);//����
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

void ST7789V_LcdDrawRectangle(uint16_t Start_X,uint16_t Start_Y,uint16_t End_X,uint16_t End_Y,uint16_t Color,uint8_t Dir_Mode)
{
	ST7789V_LcdDrawLine(Start_X,Start_Y,End_X,Start_Y,Color,Dir_Mode);
	ST7789V_LcdDrawLine(Start_X,Start_Y,Start_X,End_Y,Color,Dir_Mode);
	ST7789V_LcdDrawLine(Start_X,End_Y,End_X,End_Y,Color,Dir_Mode);
	ST7789V_LcdDrawLine(End_X,Start_Y,End_X,End_Y,Color,Dir_Mode);
}

void ST7789V_LcdDrawCircle(uint16_t X,uint16_t Y,uint8_t R,uint16_t Color,uint8_t Dir_Mode)
{
	int a,b;
	a=0;b=R;	  
	while(a<=b)
	{
		ST7789V_LcdDrawPoint(X-b,Y-a,Color,Dir_Mode);             //3           
		ST7789V_LcdDrawPoint(X+b,Y-a,Color,Dir_Mode);             //0           
		ST7789V_LcdDrawPoint(X-a,Y+b,Color,Dir_Mode);             //1                
		ST7789V_LcdDrawPoint(X-a,Y-b,Color,Dir_Mode);             //2             
		ST7789V_LcdDrawPoint(X+b,Y+a,Color,Dir_Mode);             //4               
		ST7789V_LcdDrawPoint(X+a,Y-b,Color,Dir_Mode);             //5
		ST7789V_LcdDrawPoint(X+a,Y+b,Color,Dir_Mode);             //6 
		ST7789V_LcdDrawPoint(X-b,Y+a,Color,Dir_Mode);             //7
		a++;
		if((a*a+b*b)>(R*R))//�ж�Ҫ���ĵ��Ƿ��Զ
		{
			b--;
		}
	}
}

/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Str,
*					12*12��ʽ���ַ���
*Parameter:Word_Color,
*					����ɫ
*Parameter:Back_Color,
*					������ɫ
*Parameter:Size,
*					�ֵ��ͺ�
*Parameter:Mode,
*					0�ǵ���ģʽ  1����ģʽ
*Role     :
*         ��ʾ�ø�ʽ�ַ�
*/
void ST7789V_LcdShowChinese12x12(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode)
{
//	uint8_t i,j,m=0;
//	uint16_t k;
//	uint16_t HZnum;//������Ŀ
//	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
//	uint16_t x0=X;
//	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
//	                         
//	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//ͳ�ƺ�����Ŀ
//	for(k=0;k<HZnum;k++) 
//	{
//		if((tfont12[k].Index[0]==*(Str))&&(tfont12[k].Index[1]==*(Str+1)))
//		{ 	
//			ST7789V_SetLcdAddress(X,Y,X+Size-1,Y+Size-1,Dir_Mode);
//			for(i=0;i<TypefaceNum;i++)
//			{
//				for(j=0;j<8;j++)
//				{	
//					if(!Mode)//�ǵ��ӷ�ʽ
//					{
//						if(tfont12[k].Msk[i]&(0x01<<j))ST7789V_SPISendData2Bytes(Word_Color);
//						else ST7789V_SPISendData2Bytes(Back_Color);
//						m++;
//						if(m%Size==0)
//						{
//							m=0;
//							break;
//						}
//					}
//					else//���ӷ�ʽ
//					{
//						if(tfont12[k].Msk[i]&(0x01<<j))	ST7789V_LcdDrawPoint(X,Y,Word_Color,Dir_Mode);//��һ����
//						X++;
//						if((X-x0)==Size)
//						{
//							X=x0;
//							Y++;
//							break;
//						}
//					}
//				}
//			}
//		}				  	
//		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
//	}
} 

/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Str,
*					16*16��ʽ���ַ���
*Parameter:Word_Color,
*					����ɫ
*Parameter:Back_Color,
*					������ɫ
*Parameter:Size,
*					�ֵ��ͺ�
*Parameter:Mode,
*					0�ǵ���ģʽ  1����ģʽ
*Role     :
*         ��ʾ�ø�ʽ�ַ�
*/
void ST7789V_LcdShowChinese16x16(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode)
{
//	uint8_t i,j,m=0;
//	uint16_t k;
//	uint16_t HZnum;//������Ŀ
//	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
//	uint16_t x0=X;
//	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
//	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//ͳ�ƺ�����Ŀ
//	for(k=0;k<HZnum;k++) 
//	{
//		if ((tfont16[k].Index[0]==*(Str))&&(tfont16[k].Index[1]==*(Str+1)))
//		{ 	
//			ST7789V_SetLcdAddress(X,Y,X+Size-1,Y+Size-1,Dir_Mode);
//			for(i=0;i<TypefaceNum;i++)
//			{
//				for(j=0;j<8;j++)
//				{	
//					if(!Mode)//�ǵ��ӷ�ʽ
//					{
//						if(tfont16[k].Msk[i]&(0x01<<j))ST7789V_SPISendData2Bytes(Word_Color);
//						else ST7789V_SPISendData2Bytes(Back_Color);
//						m++;
//						if(m%Size==0)
//						{
//							m=0;
//							break;
//						}
//					}
//					else//���ӷ�ʽ
//					{
//						if(tfont16[k].Msk[i]&(0x01<<j))	ST7789V_LcdDrawPoint(X,Y,Word_Color,Dir_Mode);//��һ����
//						X++;
//						if((X-x0)==Size)
//						{
//							X=x0;
//							Y++;
//							break;
//						}
//					}
//				}
//			}
//		}				  	
//		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
//	}
} 

/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Str,
*					24*24��ʽ���ַ���
*Parameter:Word_Color,
*					����ɫ
*Parameter:Back_Color,
*					������ɫ
*Parameter:Size,
*					�ֵ��ͺ�
*Parameter:Mode,
*					0�ǵ���ģʽ  1����ģʽ
*Role     :
*         ��ʾ�ø�ʽ�ַ�
*/
void ST7789V_LcdShowChinese24x24(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode)
{
//	uint8_t i,j,m=0;
//	uint16_t k;
//	uint16_t HZnum;//������Ŀ
//	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
//	uint16_t x0=X;
//	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
//	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//ͳ�ƺ�����Ŀ
//	for(k=0;k<HZnum;k++) 
//	{
//		if ((tfont24[k].Index[0]==*(Str))&&(tfont24[k].Index[1]==*(Str+1)))
//		{ 	
//			ST7789V_SetLcdAddress(X,Y,X+Size-1,Y+Size-1,Dir_Mode);
//			for(i=0;i<TypefaceNum;i++)
//			{
//				for(j=0;j<8;j++)
//				{	
//					if(!Mode)//�ǵ��ӷ�ʽ
//					{
//						if(tfont24[k].Msk[i]&(0x01<<j))ST7789V_SPISendData2Bytes(Word_Color);
//						else ST7789V_SPISendData2Bytes(Back_Color);
//						m++;
//						if(m%Size==0)
//						{
//							m=0;
//							break;
//						}
//					}
//					else//���ӷ�ʽ
//					{
//						if(tfont24[k].Msk[i]&(0x01<<j))	ST7789V_LcdDrawPoint(X,Y,Word_Color,Dir_Mode);//��һ����
//						X++;
//						if((X-x0)==Size)
//						{
//							X=x0;
//							Y++;
//							break;
//						}
//					}
//				}
//			}
//		}				  	
//		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
//	}
}

/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Str,
*					32*32��ʽ���ַ���
*Parameter:Word_Color,
*					����ɫ
*Parameter:Back_Color,
*					������ɫ
*Parameter:Size,
*					�ֵ��ͺ�
*Parameter:Mode,
*					0�ǵ���ģʽ  1����ģʽ
*Role     :
*         ��ʾ�ø�ʽ�ַ�
*/
void ST7789V_LcdShowChinese32x32(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode)
{
//	uint8_t i,j,m=0;
//	uint16_t k;
//	uint16_t HZnum;//������Ŀ
//	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
//	uint16_t x0=X;
//	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
//	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//ͳ�ƺ�����Ŀ
//	for(k=0;k<HZnum;k++) 
//	{
//		if ((tfont32[k].Index[0]==*(Str))&&(tfont32[k].Index[1]==*(Str+1)))
//		{ 	
//			ST7789V_SetLcdAddress(X,Y,X+Size-1,Y+Size-1,Dir_Mode);
//			for(i=0;i<TypefaceNum;i++)
//			{
//				for(j=0;j<8;j++)
//				{	
//					if(!Mode)//�ǵ��ӷ�ʽ
//					{
//						if(tfont32[k].Msk[i]&(0x01<<j))ST7789V_SPISendData2Bytes(Word_Color);
//						else ST7789V_SPISendData2Bytes(Back_Color);
//						m++;
//						if(m%Size==0)
//						{
//							m=0;
//							break;
//						}
//					}
//					else//���ӷ�ʽ
//					{
//						if(tfont32[k].Msk[i]&(0x01<<j))	ST7789V_LcdDrawPoint(X,Y,Word_Color,Dir_Mode);//��һ����
//						X++;
//						if((X-x0)==Size)
//						{
//							X=x0;
//							Y++;
//							break;
//						}
//					}
//				}
//			}
//		}				  	
//		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
//	}
}

void ST7789V_LcdShowChinese(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode)
{
//	while(*Str!=0)
//	{
//		if(Size==12) ST7789V_LcdShowChinese12x12(X,Y,Str,Word_Color,Back_Color,Size,Mode,Dir_Mode);
//		else if(Size==16) ST7789V_LcdShowChinese16x16(X,Y,Str,Word_Color,Back_Color,Size,Mode,Dir_Mode);
//		else if(Size==24) ST7789V_LcdShowChinese24x24(X,Y,Str,Word_Color,Back_Color,Size,Mode,Dir_Mode);
//		else if(Size==32) ST7789V_LcdShowChinese32x32(X,Y,Str,Word_Color,Back_Color,Size,Mode,Dir_Mode);
//		else return;
//		Str+=2;
//		X+=Size;
//	}
}

void ST7789V_LcdShowChar(uint16_t X,uint16_t Y,uint8_t Char,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode)
{
//	uint8_t temp,sizex,t,m=0;
//	uint16_t i,TypefaceNum;//һ���ַ���ռ�ֽڴ�С
//	uint16_t x0=X;
//	sizex=Size/2;
//	TypefaceNum=(sizex/8+((sizex%8)?1:0))*Size;
//	Char=Char-' ';    //�õ�ƫ�ƺ��ֵ
//	ST7789V_SetLcdAddress(X,Y,X+sizex-1,Y+Size-1,Dir_Mode);  //���ù��λ�� 
//	for(i=0;i<TypefaceNum;i++)
//	{ 
//		if(Size==12)temp=ascii_1206[Char][i];		       //����6x12����
//		else if(Size==16)temp=ascii_1608[Char][i];		 //����8x16����
//		else if(Size==24)temp=ascii_2412[Char][i];		 //����12x24����
//		else if(Size==32)temp=ascii_3216[Char][i];		 //����16x32����
//		else return;
//		for(t=0;t<8;t++)
//		{
//			if(!Mode)//�ǵ���ģʽ
//			{
//				if(temp&(0x01<<t))ST7789V_SPISendData2Bytes(Word_Color);
//				else ST7789V_SPISendData2Bytes(Back_Color);
//				m++;
//				if(m%sizex==0)
//				{
//					m=0;
//					break;
//				}
//			}
//			else//����ģʽ
//			{
//				if(temp&(0x01<<t))ST7789V_LcdDrawPoint(X,Y,Word_Color,Dir_Mode);//��һ����
//				X++;
//				if((X-x0)==sizex)
//				{
//					X=x0;
//					Y++;
//					break;
//				}
//			}
//		}
//	}   	 	  
}

void ST7789V_LcdShowString(uint16_t X,uint16_t Y,const uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode)
{         
	while(*Str!='\0')
	{       
		ST7789V_LcdShowChar(X,Y,*Str,Word_Color,Back_Color,Size,Mode,Dir_Mode);
		X+=Size/2;
		Str++;
	}  
}

uint32_t Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;
	return result;
}

void ST7789V_LcdShowIntNum(uint16_t X,uint16_t Y,uint16_t Number,uint8_t Len,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Dir_Mode)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	uint8_t sizex=Size/2;
	for(t=0;t<Len;t++)
	{
		temp=(Number/Pow(10,Len-t-1))%10;
		if(enshow==0&&t<(Len-1))
		{
			if(temp==0)
			{
				ST7789V_LcdShowChar(X+t*sizex,Y,' ',Word_Color,Back_Color,Size,0,Dir_Mode);
				continue;
			}else enshow=1; 
		 	 
		}
	 	ST7789V_LcdShowChar(X+t*sizex,Y,temp+48,Word_Color,Back_Color,Size,0,Dir_Mode);
	}
} 

void ST7789V_LcdShowFloatNum(uint16_t X,uint16_t Y,float Number,uint8_t Len,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Dir_Mode)
{         	
	uint8_t t,temp,sizex;
	uint16_t num1;
	sizex=Size/2;
	num1=Number*100;
	for(t=0;t<Len;t++)
	{
		temp=(num1/Pow(10,Len-t-1))%10;
		if(t==(Len-2))
		{
			ST7789V_LcdShowChar(X+(Len-2)*sizex,Y,'.',Word_Color,Back_Color,Size,0,Dir_Mode);
			t++;
			Len+=1;
		}
	 	ST7789V_LcdShowChar(X+t*sizex,Y,temp+48,Word_Color,Back_Color,Size,0,Dir_Mode);
	}
}

void ST7789V_LcdShowPicture(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Length,uint16_t Pic[],uint8_t Dir_Mode)
{
	ST7789V_SetLcdAddress(X,Y,X+Width-1,Y+Length-1,Dir_Mode);

	#ifdef SPI_DMA
	{
		if(Length*Width*2>65535)
		{
			uint32_t num1,num2;
			num1=65535;
			num2=Length*Width*2-num1;
			ST7789V_NSS_LOW();
			ST7789V_DMAInit((uint32_t)Pic,DMA_MemoryInc_Enable,DMA_MemoryDataSize_Byte,num1);
			ST7789V_DMAEnable(num1);
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_TC3)!=RESET)//�ȴ�ͨ��4�������
				{
					DMA_ClearFlag(DMA1_FLAG_TC3);//���ͨ��3������ɱ�־
					break;
				}
			}
			ST7789V_DMA_SPIX_TX_CH->CMAR = (uint32_t)((Pic+65535));
			ST7789V_DMAEnable(num2);
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_TC3)!=RESET)//�ȴ�ͨ��4�������
				{
					DMA_ClearFlag(DMA1_FLAG_TC3);//���ͨ��3������ɱ�־
					break;
				}
			}
		}
		else
		{
			ST7789V_NSS_LOW();
			ST7789V_DMAInit((uint32_t)Pic,DMA_MemoryInc_Enable,DMA_MemoryDataSize_Byte,Length*Width*2);
			ST7789V_DMAEnable(Length*Width*2);
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_TC3)!=RESET)//�ȴ�ͨ��4�������
				{
					DMA_ClearFlag(DMA1_FLAG_TC3);//���ͨ��3������ɱ�־
					break;
				}
			}
		}
		ST7789V_NSS_HIGH();
	}
	#else
	{
		uint32_t num=0;
		uint16_t i,j;
		for(i=0;i<Length;i++)
		{
			for(j=0;j<Width;j++)
			{
				ST7789V_SPISendDataByte(Pic[num*2]);
				ST7789V_SPISendDataByte(Pic[num*2+1]);
				num++;
			}
		}
	}
	#endif
}
