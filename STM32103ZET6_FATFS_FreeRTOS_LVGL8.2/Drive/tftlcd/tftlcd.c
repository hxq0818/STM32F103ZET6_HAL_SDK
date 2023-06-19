#include "tftlcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "SysTick.h"	   
#include "st7789v.h"

//LCD的画笔颜色和背景色	   
u16 FRONT_COLOR=BLACK;	//画笔颜色
u16 BACK_COLOR=WHITE;  //背景色 

_tftlcd_data tftlcd_data; 
  

//写寄存器函数
//cmd:寄存器值
void LCD_WriteCmd(u16 cmd)
{

#ifdef TFTLCD_ILI9488
	TFTLCD->LCD_CMD=cmd;
#endif
	
}

//写数据
//data:要写入的值
void LCD_WriteData(u16 data)
{

#ifdef TFTLCD_ILI9488
	TFTLCD->LCD_DATA=data;
#endif
}

void LCD_WriteCmdData(u16 cmd,u16 data)
{
	LCD_WriteCmd(cmd);
	LCD_WriteData(data);
}


u32 LCD_RGBColor_Change(u16 color)
{
	u8 r,g,b=0;
	
	r=(color>>11)&0x1f;
	g=(color>>5)&0x3f;
	b=color&0x1f;
	
	return ((r<<13)|(g<<6)|(b<<1));
}
void LCD_WriteData_Color(u16 color)
{

#ifdef TFTLCD_ILI9488
	TFTLCD->LCD_DATA=color>>8;
	TFTLCD->LCD_DATA=color&0xff;
#endif
}

//读数据
//返回值:读到的值
u16 LCD_ReadData(void)
{

#ifdef TFTLCD_ILI9488		
	u16 ram;		
	ram=TFTLCD->LCD_DATA;
	ram=ram<<8;
	ram|=TFTLCD->LCD_DATA;	
	return ram;		
#endif
}


//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(u8 dir)
{
	tftlcd_data.dir=dir;         //横屏/竖屏
	if(dir==0)  //默认竖屏方向
	{		

#ifdef TFTLCD_ILI9488		
		LCD_WriteCmdData(0x36,0x08);
		tftlcd_data.height=480;
		tftlcd_data.width=320;
#endif
	}
	else
	{	

#ifdef TFTLCD_ILI9488		
		LCD_WriteCmdData(0x36,0x28);
		LCD_WriteCmd(0XB6);
		LCD_WriteData(0x00);
		LCD_WriteData(0x02);
		LCD_WriteData(0x3B);
		tftlcd_data.height=320;
		tftlcd_data.width=480;
#endif
	}	
}


//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height. 
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{    

#ifdef TFTLCD_ILI9488
	LCD_WriteCmd(0x2A);
	LCD_WriteData(sx/256);   
	LCD_WriteData(sx%256); 	 
	LCD_WriteData(width/256); 
	LCD_WriteData(width%256);
	
	LCD_WriteCmd(0x2B);
	LCD_WriteData(sy/256);  
	LCD_WriteData(sy%256);
	LCD_WriteData(height/256); 
	LCD_WriteData(height%256); 	

	LCD_WriteCmd(0x2C);

#endif
}

//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
//	u16 r1,r2,r3;
//	u32 value;
	
	if(x>=tftlcd_data.width||y>=tftlcd_data.height)return 0;	//超过了范围,直接返回		     
	LCD_Set_Window(x, y, x, y);
	

#ifdef TFTLCD_ILI9488	
	LCD_WriteCmd(0X2e);
	r=LCD_ReadData();								//dummy Read     		 				    
	r=LCD_ReadData();								//实际坐标颜色	
	b=r>>8;
	g=r<<8;
	r=g+b;
//	printf("r=%X\r\n",r);	
	//printf("g=%X\r\n",g);
	//printf("b=%X\r\n",b);
#endif
 	return r;						
}

void TFTLCD_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG,ENABLE);//使能PORTD,E,G时钟
	
 	//PORTD复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
  	 
	//PORTE复用推挽输出  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);    	    	 											 

   	//	//PORTG12复用推挽输出 A10	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_12;	 //	//PORTG复用推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOG, &GPIO_InitStructure);  
	
	//背光控制管脚初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PB0 推挽输出 背光
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	LCD_LED=1;				//点亮背光
	
}

void TFTLCD_FSMC_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  	FSMC_NORSRAMTimingInitTypeDef  FSMC_ReadTimingInitStructure; 
	FSMC_NORSRAMTimingInitTypeDef  FSMC_WriteTimingInitStructure;
	
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//使能FSMC时钟
	
	FSMC_ReadTimingInitStructure.FSMC_AddressSetupTime = 0x01;	 //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
  	FSMC_ReadTimingInitStructure.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到	
  	FSMC_ReadTimingInitStructure.FSMC_DataSetupTime = 0x0f;		 // 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
  	FSMC_ReadTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
  	FSMC_ReadTimingInitStructure.FSMC_CLKDivision = 0x00;
  	FSMC_ReadTimingInitStructure.FSMC_DataLatency = 0x00;
  	FSMC_ReadTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A 

	FSMC_WriteTimingInitStructure.FSMC_AddressSetupTime = 0x15;	 //地址建立时间（ADDSET）为16个HCLK  
  	FSMC_WriteTimingInitStructure.FSMC_AddressHoldTime = 0x15;	 //地址保持时间		
  	FSMC_WriteTimingInitStructure.FSMC_DataSetupTime = 0x05;		 //数据保存时间为6个HCLK	
  	FSMC_WriteTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
  	FSMC_WriteTimingInitStructure.FSMC_CLKDivision = 0x00;
  	FSMC_WriteTimingInitStructure.FSMC_DataLatency = 0x00;
  	FSMC_WriteTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A  

 
  	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
  	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
  	FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
  	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit   
  	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
  	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
  	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
  	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
  	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
  	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_ReadTimingInitStructure; //读写时序
  	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_WriteTimingInitStructure;  //写时序

  	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置

 	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // 使能BANK1 
	
}
	 
//SSD1963 背光设置
//pwm:背光等级,0~100.越大越亮.
void LCD_SSD_BackLightSet(u8 pwm)
{	
	LCD_WriteCmd(0xBE);	//配置PWM输出
	LCD_WriteData(0x05);	//1设置PWM频率
	LCD_WriteData(pwm*2.55);//2设置PWM占空比
	LCD_WriteData(0x01);	//3设置C
	LCD_WriteData(0xFF);	//4设置D
	LCD_WriteData(0x00);	//5设置E
	LCD_WriteData(0x00);	//6设置F
}

void TFTLCD_Init(void)
{
//	u16 i;

	TFTLCD_GPIO_Init();
	TFTLCD_FSMC_Init();
	
	delay_ms(50); 
	

#ifdef TFTLCD_ILI9488
	LCD_WriteCmd(0Xda);	
	tftlcd_data.id=TFTLCD->LCD_DATA;	//dummy read 
	tftlcd_data.id=TFTLCD->LCD_DATA<<8;
	LCD_WriteCmd(0Xdb);	
	tftlcd_data.id|=TFTLCD->LCD_DATA;	//dummy read 
	tftlcd_data.id|=TFTLCD->LCD_DATA;	
	
#endif
	
 	printf(" LCD ID:%x\r\n",tftlcd_data.id); //打印LCD ID
	

#ifdef TFTLCD_ILI9488
	LCD_WriteCmd(0xE0); //P-Gamma
	LCD_WriteData(0x00);
	LCD_WriteData(0x13);
	LCD_WriteData(0x18);
	LCD_WriteData(0x04);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x06);
	LCD_WriteData(0x3A);
	LCD_WriteData(0x56);
	LCD_WriteData(0x4D);
	LCD_WriteData(0x03);
	LCD_WriteData(0x0A);
	LCD_WriteData(0x06);
	LCD_WriteData(0x30);
	LCD_WriteData(0x3E);
	LCD_WriteData(0x0F);
	
	LCD_WriteCmd(0XE1); //N-Gamma
	LCD_WriteData(0x00);
	LCD_WriteData(0x13);
	LCD_WriteData(0x18);
	LCD_WriteData(0x01);
	LCD_WriteData(0x11);
	LCD_WriteData(0x06);
	LCD_WriteData(0x38);
	LCD_WriteData(0x34);
	LCD_WriteData(0x4D);
	LCD_WriteData(0x06);
	LCD_WriteData(0x0D);
	LCD_WriteData(0x0B);
	LCD_WriteData(0x31);
	LCD_WriteData(0x37);
	LCD_WriteData(0x0F); 
	
	LCD_WriteCmd(0XC0);   //Power Control 1
	LCD_WriteData(0x18); //Vreg1out
	LCD_WriteData(0x17); //Verg2out
	
	LCD_WriteCmd(0xC1);   //Power Control 2
	LCD_WriteData(0x41); //VGH,VGL
	
	LCD_WriteCmd(0xC5);   //Power Control 3
	LCD_WriteData(0x00);
	LCD_WriteData(0x1A); //Vcom
	LCD_WriteData(0x80);
	
	LCD_WriteCmd(0x36);   //Memory Access
	LCD_WriteData(0x08);   //48
	
	LCD_WriteCmd(0x3A);   // Interface Pixel Format
	LCD_WriteData(0x55); //16bit
	
	LCD_WriteCmd(0XB0);   // Interface Mode Control
	LCD_WriteData(0x00);
	
	LCD_WriteCmd(0xB1);   //Frame rate
	LCD_WriteData(0xA0); //60Hz
	
	LCD_WriteCmd(0xB4);   //Display Inversion Control
	LCD_WriteData(0x02); //2-dot
	
	LCD_WriteCmd(0XB6);   //RGB/MCU Interface Control
//	LCD_WriteData(0x02); //MCU RGB
//	LCD_WriteData(0x02); //Source,Gate scan dieection
	LCD_WriteData(0x00);
	LCD_WriteData(0x22);//0 GS SS SM ISC[3:0];其中GS SS控制显示方向，同时修改R36
	LCD_WriteData(0x3B);
	
	LCD_WriteCmd(0XE9);    // Set Image Function
	LCD_WriteData(0x00);  //disable 24 bit data input
	
	LCD_WriteCmd(0xF7);    // Adjust Control
	LCD_WriteData(0xA9);
	LCD_WriteData(0x51);
	LCD_WriteData(0x2C);
	LCD_WriteData(0x82);  // D7 stream, loose
	
	LCD_WriteCmd(0x11);    //Sleep out
	delay_ms(120);
	LCD_WriteCmd(0x29);  
	LCD_WriteCmd(0x2C);
#endif

	LCD_Display_Dir(TFTLCD_DIR);		//0：竖屏  1：横屏  默认竖屏
	LCD_Clear(WHITE);
}

//清屏函数
//color:要清屏的填充色
void LCD_Clear(u16 color)
{
	uint16_t i, j ;

	LCD_Set_Window(0, 0, tftlcd_data.width-1, tftlcd_data.height-1);	 //作用区域
  	for(i=0; i<tftlcd_data.width; i++)
	{
		for (j=0; j<tftlcd_data.height; j++)
		{
			LCD_WriteData_Color(color);
		}
	} 
}


//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(u16 xState,u16 yState,u16 xEnd,u16 yEnd,u16 color)
{          
	uint16_t temp;

    if((xState > xEnd) || (yState > yEnd))
    {
        return;
    }   
	LCD_Set_Window(xState, yState, xEnd, yEnd); 
    xState = xEnd - xState + 1;
	yState = yEnd - yState + 1;

	while(xState--)
	{
	 	temp = yState;
		while (temp--)
	 	{			
			LCD_WriteData_Color(color);	
		}
	}	
} 

//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
	
	LCD_Set_Window(sx, sy,ex, ey);
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_WriteData_Color(color[i*width+j]);
		}
	}		  
}
//画点
//x,y:坐标
//FRONT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_Set_Window(x, y, x, y);  //设置点的位置
	LCD_WriteData_Color(FRONT_COLOR);	
}

//快速画点
//x,y:坐标
//color:颜色
void LCD_DrawFRONT_COLOR(u16 x,u16 y,u16 color)
{	   
	LCD_Set_Window(x, y, x, y);
	LCD_WriteData_Color(color);	
} 

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
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

void LCD_DrawLine_Color(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawFRONT_COLOR(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
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


// 画一个十字的标记
// x：标记的X坐标
// y：标记的Y坐标
// color：标记的颜色
void LCD_DrowSign(uint16_t x, uint16_t y, uint16_t color)
{
    uint8_t i;

    /* 画点 */
    LCD_Set_Window(x-1, y-1, x+1, y+1);
    for(i=0; i<9; i++)
    {
		LCD_WriteData_Color(color);   
    }

    /* 画竖 */
    LCD_Set_Window(x-4, y, x+4, y);
    for(i=0; i<9; i++)
    {
		LCD_WriteData_Color(color); 
    }

    /* 画横 */
    LCD_Set_Window(x, y-4, x, y+4);
    for(i=0; i<9; i++)
    {
		LCD_WriteData_Color(color); 
    }
}

//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 



//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
//    u8 temp,t1,t;
//	u16 y0=y;
//	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
// 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
//	for(t=0;t<csize;t++)
//	{   
//		if(size==12)temp=ascii_1206[num][t]; 	 	//调用1206字体
//		else if(size==16)temp=ascii_1608[num][t];	//调用1608字体
//		else if(size==24)temp=ascii_2412[num][t];	//调用2412字体
//		else return;								//没有的字库
//		for(t1=0;t1<8;t1++)
//		{			    
//			if(temp&0x80)LCD_DrawFRONT_COLOR(x,y,FRONT_COLOR);
//			else if(mode==0)LCD_DrawFRONT_COLOR(x,y,BACK_COLOR);
//			temp<<=1;
//			y++;
//			if(y>=tftlcd_data.height)return;		//超区域了
//			if((y-y0)==size)
//			{
//				y=y0;
//				x++;
//				if(x>=tftlcd_data.width)return;	//超区域了
//				break;
//			}
//		}  	 
//	}  	    	   	 	  
}   
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 

//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

/****************************************************************************
*函数名：LCD_ShowFontHZ
*输  入：x：汉字显示的X坐标
*      * y：汉字显示的Y坐标
*      * cn：要显示的汉字
*      * wordColor：文字的颜色
*      * backColor：背景颜色
*输  出：
*功  能：写二号楷体汉字
****************************************************************************/
#if 0
void LCD_ShowFontHZ(u16 x, u16 y, u8 *cn)	 
{  
	u8 i, j, wordNum;
	u16 color;
	while (*cn != '\0')
	{
		LCD_Set_Window(x, y, x+31, y+28);
		for (wordNum=0; wordNum<20; wordNum++)
		{	//wordNum扫描字库的字数
			if ((CnChar32x29[wordNum].Index[0]==*cn)
			     &&(CnChar32x29[wordNum].Index[1]==*(cn+1)))
			{
				for(i=0; i<116; i++) 
				{	//MSK的位数
					color=CnChar32x29[wordNum].Msk[i];
					for(j=0;j<8;j++) 
					{
						if((color&0x80)==0x80)
						{
							LCD_WriteData_Color(FRONT_COLOR); 						
						} 						
						else
						{
							LCD_WriteData_Color(BACK_COLOR); 
						} 
						color<<=1;
					}//for(j=0;j<8;j++)结束
				}    
			}
		} //for (wordNum=0; wordNum<20; wordNum++)结束 	
		cn += 2;
		x += 32;
	}
}
#endif


#if 0
void LCD_ShowFontHZ(u16 x, u16 y, u8 *cn)
{
	u8 i, j, wordNum;
	u16 color;
	u16 x0=x; 
	u16 y0=y; 
	while (*cn != '\0')
	{
		for (wordNum=0; wordNum<20; wordNum++)
		{	//wordNum扫描字库的字数
			if ((CnChar32x29[wordNum].Index[0]==*cn)
			     &&(CnChar32x29[wordNum].Index[1]==*(cn+1)))
			{
				for(i=0; i<116; i++) 
				{	//MSK的位数
					color=CnChar32x29[wordNum].Msk[i];
					for(j=0;j<8;j++) 
					{
						if((color&0x80)==0x80)
						{
							LCD_DrawFRONT_COLOR(x,y,FRONT_COLOR);
						} 						
						else
						{
							LCD_DrawFRONT_COLOR(x,y,BACK_COLOR);
						} 
						color<<=1;
						x++;
						if((x-x0)==32)
						{
							x=x0;
							y++;
							if((y-y0)==29)
							{
								y=y0;
							}
						}
					}//for(j=0;j<8;j++)结束
				}	
			}
			
		} //for (wordNum=0; wordNum<20; wordNum++)结束 	
		cn += 2;
		x += 32;
		x0=x;
	}
}	
#endif

void LCD_ShowPicture(u16 x, u16 y, u16 wide, u16 high,u8 *pic)
{
	u16 i,j;
	u16 temp = 0;
	long tmp=0;
//	long num=0;
//	num = wide * high*2 ;
	//方法1：提高显示速度，太快可能导致图像显示不全
//	do
//	{  
//		temp = pic[tmp + 1];
//		temp = temp << 8;
//		temp = temp | pic[tmp];
//		LCD_WriteData_Color(temp);//逐点显示
//		tmp += 2;
//	}
//	while(tmp < num);
	
	LCD_Set_Window(x, y, x+wide-1, y+high-1);
	//方法2：可有效消除LCD显示速度过快导致显示不全问题
	for(i=0;i<high;i++)
	{
		for(j=0;j<wide;j++)
		{
			temp = pic[tmp + 1];
			temp = temp << 8;
			temp = temp | pic[tmp];
			LCD_DrawFRONT_COLOR(x+j,y+i,temp);
			tmp += 2;
		}
	}	
}
