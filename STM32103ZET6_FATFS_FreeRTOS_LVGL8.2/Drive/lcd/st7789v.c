
/**************作者B站UP：Sangk-Hu****************/

/*****************QQ群：690367095******************/

/****关注UP后，私信或者加群获得资料或者其他资料*****/

#include "st7789v.h"
#include "font.h"
#include "SysTick.h"


/*记录上一次传输数据长度*/
uint16_t Tx_Len=0;

/*
*Role     :
*         对于定义SPI初始化
*/
void ST7789V_SPIInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(ST7789V_SCK_RCC_GPIO|ST7789V_SDA_RCC_GPIO|ST7789V_RCC_SPIX, ENABLE);//使能GPIOA,使能SPI1

	GPIO_InitStructure.GPIO_Pin =ST7789V_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 //复用推挽输出
	GPIO_Init(ST7789V_SCK_GPIO, &GPIO_InitStructure); //初始化相应端接口
	GPIO_SetBits(ST7789V_SCK_GPIO,ST7789V_SCK_PIN);//端口输出初始化为1
	
	GPIO_InitStructure.GPIO_Pin =ST7789V_SDA_PIN;
	GPIO_Init(ST7789V_SDA_GPIO, &GPIO_InitStructure); //初始化相应端接口
	GPIO_SetBits(ST7789V_SDA_GPIO,ST7789V_SDA_PIN);

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;//只发送模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//设置SPI工作模式：主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//设置SPI数据大小：8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//串行同步时钟空闲时SCLK位高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//串行同步时钟空第二个时钟沿捕获
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS信号由硬件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//波特率预分频值：波特率预分频值为4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输高位先行
	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC值计算的多项式
	SPI_Init(ST7789V_SPIX,&SPI_InitStructure);//初始化SPI
	SPI_Cmd(ST7789V_SPIX, ENABLE);//使能SPI
}

#ifdef ST7789V_SPIX_DMA

/*
*Parameter:Madr
*					存储地址
*Parameter:DMA_MemoryInc,
*					存储地址是否可以
*Parameter:Len,
*					传输数据长度
*Role     :
*         初始当前应用场景下的DMA配置
*/
void ST7789V_DMAInit(uint32_t Madr,uint32_t DMA_MemoryInc,uint32_t DataSize,uint16_t Len)
{
	DMA_InitTypeDef  DMA_InitStructure;
	//使能 DMA 控制器（DMA1 或 DMA2）时钟
	RCC_AHBPeriphClockCmd(ST7789V_RCC_DMA, ENABLE);//DMA1时钟使能 
	
	DMA_DeInit(ST7789V_DMA_SPIX_TX_CH);
	
	/* 配置 DMA */
	//DMA外设地址
	DMA_InitStructure.DMA_PeripheralBaseAddr =ST7789V_SPIX_ADD;
	//DMA 存储器地址
	DMA_InitStructure.DMA_MemoryBaseAddr =Madr;
	//存储器到外设模式
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	//数据传输量 
	DMA_InitStructure.DMA_BufferSize =Len;
	Tx_Len=Len;
	//外设非增量模式
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//存储器增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc;
	if(DataSize==DMA_PeripheralDataSize_Byte||DataSize==DMA_MemoryDataSize_Byte)
	{
		//外设数据长度
		DMA_InitStructure.DMA_PeripheralDataSize = DataSize;
		//存储器数据长度
		DMA_InitStructure.DMA_MemoryDataSize = DataSize;
	}
	else if(DataSize==DMA_PeripheralDataSize_HalfWord||DataSize==DMA_MemoryDataSize_HalfWord)
	{
		//外设数据长度
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		//存储器数据长度
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	}
	else 
	{
		//外设数据长度
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
		//存储器数据长度
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	}
	// 使用普通模式 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//中等优先级
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	//DMA通道x没有设置为内存到内存传输
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	//初始化DMA 
	DMA_Init(ST7789V_DMA_SPIX_TX_CH, &DMA_InitStructure);
}

/*
*Parameter:Len
*					传输数据长度
*Role     :
*         使能DMA
*/
void ST7789V_DMAEnable(uint16_t Len)
{
	DMA_Cmd(ST7789V_DMA_SPIX_TX_CH, DISABLE);                      //关闭DMA传输 
	DMA_SetCurrDataCounter(ST7789V_DMA_SPIX_TX_CH,Len);          //数据传输量 
	SPI_Cmd(ST7789V_SPIX, ENABLE);//使能SPI	
	SPI_I2S_DMACmd(ST7789V_SPIX, SPI_I2S_DMAReq_Tx, ENABLE);			//使能spi的dma功能
	DMA_Cmd(ST7789V_DMA_SPIX_TX_CH, ENABLE);                      //开启DMA传输 
}

#endif

void ST7789V_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	/* SPI的IO口和SPI外设打开时钟 */
  RCC_APB2PeriphClockCmd(	ST7789V_RESET_RCC_GPIO|ST7789V_RS_RCC_GPIO|ST7789V_NSS_RCC_GPIO, ENABLE );

//	/* SPI的IO口设置 */
	GPIO_InitStructure.GPIO_Pin = ST7789V_RESET_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出 
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
	//端口输出初始化为1
	GPIO_SetBits(ST7789V_NSS_GPIO,ST7789V_NSS_PIN);
}

//SPI2速度设置函数
//SPI速度=fAPB1/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB1时钟一般为36Mhz：
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler)
{
	ST7789V_SPIX->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	ST7789V_SPIX->CR1|=SPI_BaudRatePrescaler;	//设置SPI速度 
	SPI_Cmd(ST7789V_SPIX,ENABLE); //使能SPI2
} 
/*SPI发送函数*/
void delay(int t)
{
	while(t--);
}

void ST7789V_SPISend(uint8_t Data)
{
	ST7789V_NSS_LOW();
	while (SPI_I2S_GetFlagStatus(ST7789V_SPIX, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(ST7789V_SPIX,Data);
	delay(1);//注意延迟
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
/*ST7789V应用函数*/
void ST7789V_SendOneColor(uint16_t Color)
{
	ST7789V_SPISendData2Bytes(Color);
}

void ST7789V_SetLcdAddress(uint16_t Start_X,uint16_t Start_Y,uint16_t End_X,uint16_t End_Y,uint8_t Dir_Mode)
{
	if(Dir_Mode==0)
	{
		ST7789V_SPISendCmdByte(0x2a);//列地址设置
		ST7789V_SPISendData2Bytes(Start_X+WIDTH_OFFSET);
		ST7789V_SPISendData2Bytes(End_X+WIDTH_OFFSET);
		ST7789V_SPISendCmdByte(0x2b);//行地址设置
		ST7789V_SPISendData2Bytes(Start_Y+HIGH_OFFSET);
		ST7789V_SPISendData2Bytes(End_Y+HIGH_OFFSET);
		ST7789V_SPISendCmdByte(0x2c);//储存器写
	}
	else if(Dir_Mode==1)
	{
		ST7789V_SPISendCmdByte(0x2a);//列地址设置
		ST7789V_SPISendData2Bytes(Start_X+WIDTH_OFFSET);
		ST7789V_SPISendData2Bytes(End_X+WIDTH_OFFSET);
		ST7789V_SPISendCmdByte(0x2b);//行地址设置
		ST7789V_SPISendData2Bytes(Start_Y+HIGH_OFFSET);
		ST7789V_SPISendData2Bytes(End_Y+HIGH_OFFSET);
		ST7789V_SPISendCmdByte(0x2c);//储存器写
	}
	else if(Dir_Mode==2)
	{
		ST7789V_SPISendCmdByte(0x2a);//列地址设置
		ST7789V_SPISendData2Bytes(Start_X+HIGH_OFFSET);
		ST7789V_SPISendData2Bytes(End_X+HIGH_OFFSET);
		ST7789V_SPISendCmdByte(0x2b);//行地址设置
		ST7789V_SPISendData2Bytes(Start_Y+WIDTH_OFFSET);
		ST7789V_SPISendData2Bytes(End_Y+WIDTH_OFFSET);
		ST7789V_SPISendCmdByte(0x2c);//储存器写
	}
	else
	{
		ST7789V_SPISendCmdByte(0x2a);//列地址设置
		ST7789V_SPISendData2Bytes(Start_X+HIGH_OFFSET);
		ST7789V_SPISendData2Bytes(End_X+HIGH_OFFSET);
		ST7789V_SPISendCmdByte(0x2b);//行地址设置
		ST7789V_SPISendData2Bytes(Start_Y+WIDTH_OFFSET);
		ST7789V_SPISendData2Bytes(End_Y+WIDTH_OFFSET);
		ST7789V_SPISendCmdByte(0x2c);//储存器写
	}
}

void ST7789V_Reset(void)
{
	ST7789V_RESET_LOW();  //复位
	Correct_DelayMs(100);
	ST7789V_RESET_HIGH();
	Correct_DelayMs(100);
}

void ST7789V_SetDir(uint8_t Dir_Mode)
{
	ST7789V_SPISendCmdByte(0x36);/*显示方向*/ 
	if(Dir_Mode==0)ST7789V_SPISendDataByte(0x00);
	else if(Dir_Mode==1)ST7789V_SPISendDataByte(0xC0);
	else if(Dir_Mode==2)ST7789V_SPISendDataByte(0x70);
	else ST7789V_SPISendDataByte(0xA0);
}

void ST7789V_LcdInit(void)
{
	ST7789V_GPIOInit();		//初始化GPIO
	ST7789V_SPIInit();    //初始化SPI1
	
	ST7789V_Reset();//复位
	
	ST7789V_SPISendCmdByte(0x11); 
	Correct_DelayMs(120); 

	ST7789V_SetDir(USE_HORIZONTAL);
	
	/*12位颜色为03h，16位颜色位05h，18位为06h，代表了颜色更多，显示更好*/
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
	ST7789V_SPISendDataByte(0x75);//······12.2v   -10.43v  
//--------------ST7789V Power setting---------------//
	ST7789V_SPISendCmdByte(0xC2);//VDV and VRH Command Enable
	ST7789V_SPISendDataByte(0x01);

	ST7789V_SPISendCmdByte(0xC3);//VRH Set
	ST7789V_SPISendDataByte(0x16);//······4.3+( vcom+vcom offset+vdv)   

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

	ST7789V_SPISendCmdByte(0x21);//此命令用于从显示反转模式恢复。

	ST7789V_SPISendCmdByte(0x29);//显示打开
	
	ST7789V_SPISendCmdByte(0x2c);
	/*
此命令用于将数据从MCU传输到帧存储器。
-接受此命令时，列寄存器和页面寄存器将重置为开始列/开始
页面位置。
-根据 MADCTL 设置，起始列/起始页位置不同。
- 发送任何其他命令都可以停止帧写入。
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
		ST7789V_SPISendCmdByte(0x33);//设置滚动条件的各个区域高度，三部分相加=320
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
	ST7789V_SetLcdAddress(Start_X,Start_Y,End_X-1,End_Y-1,Dir_Mode);//设置显示范围
	
	#if SPI_DMA
	{
		uint32_t num1,num2;
		ST7789V_SPIX->CR1|=1<<11;//设置SPI16位传输模式
		if(num>65535)
		{
			num1=65535;
			num2=num-num1;
			ST7789V_NSS_LOW();
			ST7789V_DMAInit((uint32_t)(&Color),DMA_MemoryInc_Disable,DMA_MemoryDataSize_HalfWord,num1);
			ST7789V_DMAEnable(num1);
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_TC3)!=RESET)//等待通道4传输完成
				{
					DMA_ClearFlag(DMA1_FLAG_TC3);//清除通道3传输完成标志
					break;
				}
			}
			ST7789V_DMAEnable(num2);
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_TC3)!=RESET)//等待通道4传输完成
				{
					DMA_ClearFlag(DMA1_FLAG_TC3);//清除通道3传输完成标志
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
				if(DMA_GetFlagStatus(DMA1_FLAG_TC3)!=RESET)//等待通道4传输完成
				{
					DMA_ClearFlag(DMA1_FLAG_TC3);//清除通道3传输完成标志
					break;
				}
			}
		}
		ST7789V_NSS_HIGH();
		ST7789V_SPIX->CR1=~ST7789V_SPIX->CR1;
		ST7789V_SPIX->CR1|=1<<11;
		ST7789V_SPIX->CR1=~ST7789V_SPIX->CR1;//设置SPI8位传输模式
		SPI_Cmd(ST7789V_SPIX, ENABLE);//使能SPI
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
	ST7789V_SetLcdAddress(X,Y,X,Y,Dir_Mode);//设置光标位置 
	ST7789V_SPISendData2Bytes(Color);
} 

void ST7789V_LcdDrawLine(uint16_t Start_X,uint16_t Start_Y,uint16_t End_X,uint16_t End_Y,uint16_t Color,uint8_t Dir_Mode)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=End_X-Start_X; //计算坐标增量 
	delta_y=End_Y-Start_Y;
	uRow=Start_X;//画线起点坐标
	uCol=Start_Y;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		ST7789V_LcdDrawPoint(uRow,uCol,Color,Dir_Mode);//画点
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
		if((a*a+b*b)>(R*R))//判断要画的点是否过远
		{
			b--;
		}
	}
}

/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Str,
*					12*12格式，字符串
*Parameter:Word_Color,
*					字颜色
*Parameter:Back_Color,
*					背景颜色
*Parameter:Size,
*					字的型号
*Parameter:Mode,
*					0非叠加模式  1叠加模式
*Role     :
*         显示该格式字符
*/
void ST7789V_LcdShowChinese12x12(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode)
{
//	uint8_t i,j,m=0;
//	uint16_t k;
//	uint16_t HZnum;//汉字数目
//	uint16_t TypefaceNum;//一个字符所占字节大小
//	uint16_t x0=X;
//	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
//	                         
//	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//统计汉字数目
//	for(k=0;k<HZnum;k++) 
//	{
//		if((tfont12[k].Index[0]==*(Str))&&(tfont12[k].Index[1]==*(Str+1)))
//		{ 	
//			ST7789V_SetLcdAddress(X,Y,X+Size-1,Y+Size-1,Dir_Mode);
//			for(i=0;i<TypefaceNum;i++)
//			{
//				for(j=0;j<8;j++)
//				{	
//					if(!Mode)//非叠加方式
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
//					else//叠加方式
//					{
//						if(tfont12[k].Msk[i]&(0x01<<j))	ST7789V_LcdDrawPoint(X,Y,Word_Color,Dir_Mode);//画一个点
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
//		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
//	}
} 

/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Str,
*					16*16格式，字符串
*Parameter:Word_Color,
*					字颜色
*Parameter:Back_Color,
*					背景颜色
*Parameter:Size,
*					字的型号
*Parameter:Mode,
*					0非叠加模式  1叠加模式
*Role     :
*         显示该格式字符
*/
void ST7789V_LcdShowChinese16x16(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode)
{
//	uint8_t i,j,m=0;
//	uint16_t k;
//	uint16_t HZnum;//汉字数目
//	uint16_t TypefaceNum;//一个字符所占字节大小
//	uint16_t x0=X;
//	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
//	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//统计汉字数目
//	for(k=0;k<HZnum;k++) 
//	{
//		if ((tfont16[k].Index[0]==*(Str))&&(tfont16[k].Index[1]==*(Str+1)))
//		{ 	
//			ST7789V_SetLcdAddress(X,Y,X+Size-1,Y+Size-1,Dir_Mode);
//			for(i=0;i<TypefaceNum;i++)
//			{
//				for(j=0;j<8;j++)
//				{	
//					if(!Mode)//非叠加方式
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
//					else//叠加方式
//					{
//						if(tfont16[k].Msk[i]&(0x01<<j))	ST7789V_LcdDrawPoint(X,Y,Word_Color,Dir_Mode);//画一个点
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
//		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
//	}
} 

/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Str,
*					24*24格式，字符串
*Parameter:Word_Color,
*					字颜色
*Parameter:Back_Color,
*					背景颜色
*Parameter:Size,
*					字的型号
*Parameter:Mode,
*					0非叠加模式  1叠加模式
*Role     :
*         显示该格式字符
*/
void ST7789V_LcdShowChinese24x24(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode)
{
//	uint8_t i,j,m=0;
//	uint16_t k;
//	uint16_t HZnum;//汉字数目
//	uint16_t TypefaceNum;//一个字符所占字节大小
//	uint16_t x0=X;
//	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
//	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//统计汉字数目
//	for(k=0;k<HZnum;k++) 
//	{
//		if ((tfont24[k].Index[0]==*(Str))&&(tfont24[k].Index[1]==*(Str+1)))
//		{ 	
//			ST7789V_SetLcdAddress(X,Y,X+Size-1,Y+Size-1,Dir_Mode);
//			for(i=0;i<TypefaceNum;i++)
//			{
//				for(j=0;j<8;j++)
//				{	
//					if(!Mode)//非叠加方式
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
//					else//叠加方式
//					{
//						if(tfont24[k].Msk[i]&(0x01<<j))	ST7789V_LcdDrawPoint(X,Y,Word_Color,Dir_Mode);//画一个点
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
//		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
//	}
}

/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Str,
*					32*32格式，字符串
*Parameter:Word_Color,
*					字颜色
*Parameter:Back_Color,
*					背景颜色
*Parameter:Size,
*					字的型号
*Parameter:Mode,
*					0非叠加模式  1叠加模式
*Role     :
*         显示该格式字符
*/
void ST7789V_LcdShowChinese32x32(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode)
{
//	uint8_t i,j,m=0;
//	uint16_t k;
//	uint16_t HZnum;//汉字数目
//	uint16_t TypefaceNum;//一个字符所占字节大小
//	uint16_t x0=X;
//	TypefaceNum=(Size/8+((Size%8)?1:0))*Size;
//	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//统计汉字数目
//	for(k=0;k<HZnum;k++) 
//	{
//		if ((tfont32[k].Index[0]==*(Str))&&(tfont32[k].Index[1]==*(Str+1)))
//		{ 	
//			ST7789V_SetLcdAddress(X,Y,X+Size-1,Y+Size-1,Dir_Mode);
//			for(i=0;i<TypefaceNum;i++)
//			{
//				for(j=0;j<8;j++)
//				{	
//					if(!Mode)//非叠加方式
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
//					else//叠加方式
//					{
//						if(tfont32[k].Msk[i]&(0x01<<j))	ST7789V_LcdDrawPoint(X,Y,Word_Color,Dir_Mode);//画一个点
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
//		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
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
//	uint16_t i,TypefaceNum;//一个字符所占字节大小
//	uint16_t x0=X;
//	sizex=Size/2;
//	TypefaceNum=(sizex/8+((sizex%8)?1:0))*Size;
//	Char=Char-' ';    //得到偏移后的值
//	ST7789V_SetLcdAddress(X,Y,X+sizex-1,Y+Size-1,Dir_Mode);  //设置光标位置 
//	for(i=0;i<TypefaceNum;i++)
//	{ 
//		if(Size==12)temp=ascii_1206[Char][i];		       //调用6x12字体
//		else if(Size==16)temp=ascii_1608[Char][i];		 //调用8x16字体
//		else if(Size==24)temp=ascii_2412[Char][i];		 //调用12x24字体
//		else if(Size==32)temp=ascii_3216[Char][i];		 //调用16x32字体
//		else return;
//		for(t=0;t<8;t++)
//		{
//			if(!Mode)//非叠加模式
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
//			else//叠加模式
//			{
//				if(temp&(0x01<<t))ST7789V_LcdDrawPoint(X,Y,Word_Color,Dir_Mode);//画一个点
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
				if(DMA_GetFlagStatus(DMA1_FLAG_TC3)!=RESET)//等待通道4传输完成
				{
					DMA_ClearFlag(DMA1_FLAG_TC3);//清除通道3传输完成标志
					break;
				}
			}
			ST7789V_DMA_SPIX_TX_CH->CMAR = (uint32_t)((Pic+65535));
			ST7789V_DMAEnable(num2);
			while(1)
			{
				if(DMA_GetFlagStatus(DMA1_FLAG_TC3)!=RESET)//等待通道4传输完成
				{
					DMA_ClearFlag(DMA1_FLAG_TC3);//清除通道3传输完成标志
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
				if(DMA_GetFlagStatus(DMA1_FLAG_TC3)!=RESET)//等待通道4传输完成
				{
					DMA_ClearFlag(DMA1_FLAG_TC3);//清除通道3传输完成标志
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
