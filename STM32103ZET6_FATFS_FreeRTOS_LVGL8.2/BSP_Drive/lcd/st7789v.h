
/**************作者B站UP：Sangk-Hu****************/

/*****************QQ群：690367095******************/

/****关注UP后，私信或者加群获得资料或者其他资料*****/

/********************************************/
/*采用精确定时，使用SysTick_Init初始延时函数*/
/********************************************/
#ifndef _st7789v_H
#define _st7789v_H

#include <stm32f10x.h>
#include "system.h"   

#define SPI_DMA 0

/*上一次的传输数据长度*/
extern uint16_t Tx_Len;

/*ST7789V_AdaptiveBrightnessColorEnhancementInit初始化结构体*/
typedef struct
{
	/*off:00,User Interface Mode:01,Still Picture:10,Moving Image:11*/
	uint8_t Adaptive_Brightness_Goal;
	/*open:1,close:0*/
	uint8_t Color_Enhancement_Cmd;
	/*Low enhancement:00,Medium enhancement:01,High enhancement:11*/
	uint8_t Color_Enhancement_Extent;
}WRCACE_InitTypedef;

/*
*ST7789V的支持得到最大分辨率为240*320，该显示屏支持240*280，故有偏移量
*偏移量计算举例：（320-280）/2=20
*以USE_HORIZONTAL==1时为基准，即正屏
*/
#define WIDTH_OFFSET 		 0
#define HIGH_OFFSET 		 20

#define USE_HORIZONTAL 	 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 					 240
#define LCD_H 					 280

#else
#define LCD_W 					 280
#define LCD_H 					 240
#endif

//画笔颜色
#define WHITE         	 (uint16_t)0xFFFF
#define BLACK         	 (uint16_t)0x0000	  
#define BLUE           	 (uint16_t)0x001F  
#define BRED             (uint16_t)0XF81F
#define GRED 			       (uint16_t)0XFFE0
#define GBLUE			       (uint16_t)0X07FF
#define RED           	 (uint16_t)0xF800
#define MAGENTA       	 (uint16_t)0xF81F
#define GREEN         	 (uint16_t)0x07E0
#define CYAN          	 (uint16_t)0x7FFF
#define YELLOW        	 (uint16_t)0xFFE0
#define BROWN 			     (uint16_t)0XBC40 //棕色
#define BRRED 			     (uint16_t)0XFC07 //棕红色
#define GRAY  			     (uint16_t)0X8430 //灰色
#define DARKBLUE      	 (uint16_t)0X01CF	//深蓝色
#define LIGHTBLUE      	 (uint16_t)0X7D7C	//浅蓝色  
#define GRAYBLUE       	 (uint16_t)0X5458 //灰蓝色
#define LIGHTGREEN     	 (uint16_t)0X841F //浅绿色
#define LGRAY 			     (uint16_t)0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        (uint16_t)0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           (uint16_t)0X2B12 //浅棕蓝色(选择条目的反色)

/*定义管脚GPIOx的RCC*/
#define ST7789V_RESET_RCC_GPIO		RCC_APB2Periph_GPIOA
#define ST7789V_RS_RCC_GPIO 			RCC_APB2Periph_GPIOA
#define ST7789V_NSS_RCC_GPIO  		RCC_APB2Periph_GPIOA
#define ST7789V_SCK_RCC_GPIO 			RCC_APB2Periph_GPIOA
#define ST7789V_SDA_RCC_GPIO			RCC_APB2Periph_GPIOA
/*定义管脚GPIOx*/
#define ST7789V_RESET_GPIO				GPIOA
#define ST7789V_RS_GPIO 					GPIOA
#define ST7789V_NSS_GPIO  				GPIOA
#define ST7789V_SCK_GPIO 					GPIOA
#define ST7789V_SDA_GPIO					GPIOA
/*定义管脚位置*/
#define ST7789V_RESET_PIN 				GPIO_Pin_1
#define ST7789V_RS_PIN 						GPIO_Pin_2
#define ST7789V_NSS_PIN   				GPIO_Pin_3
#define ST7789V_SCK_PIN 					GPIO_Pin_5
#define ST7789V_SDA_PIN 					GPIO_Pin_7
/*定义位操作*/
#define ST7789V_RESET 						PAout(1)
#define ST7789V_RS 								PAout(2)
#define ST7789V_NSS								PAout(3)
/*定义端口操作函数*/
#define ST7789V_RESET_HIGH() 			ST7789V_RESET=1
#define ST7789V_RESET_LOW() 			ST7789V_RESET=0
#define ST7789V_RS_HIGH() 				ST7789V_RS=1
#define ST7789V_RS_LOW() 					ST7789V_RS=0
#define ST7789V_NSS_HIGH()				ST7789V_NSS=1
#define ST7789V_NSS_LOW()					ST7789V_NSS=0
/*定义关于SPI初始化宏，以及其DMA的宏*/
#define ST7789V_SPIX 							SPI1
#define ST7789V_RCC_SPIX					RCC_APB2Periph_SPI1

/*使用DMA*/
#define ST7789V_SPIX_DMA

#ifdef ST7789V_SPIX_DMA
#define ST7789V_SPIX_ADD					(uint32_t)(&(ST7789V_SPIX->DR))
#define ST7789V_RCC_DMA						RCC_AHBPeriph_DMA1
#define ST7789V_DMA_SPIX_TX_FLAG	DMA1_FLAG_TC3
#define ST7789V_DMA_SPIX_TX_CH		DMA1_Channel3
#endif

/*
*Parameter:NewState,
*					是否使能
*Parameter:Start_Line,
*					局部显示开始地址
*Parameter:End_Line,
*					局部显示结束地址
*Role     :
*         局部显示
*/
void ST7789V_PartialDisplayModeCmd(FunctionalState NewState,uint16_t Start_Line,uint16_t End_Line);
/*
*Parameter:NewState,
*					是否使能
*Role     :
*         空闲模式开和关（在图像要求不高下，减少耗电）
*/
void ST7789V_IdleModeCmd(FunctionalState NewState);
/*
*Parameter:NewState,
*					是否使能
*Role     :
*         显示开和关
*/
void ST7789V_DisplayCmd(FunctionalState NewState);
/*
*Parameter:NewState,
*					是否使能
*Role     :
*         睡眠模式开和关（关闭显示，停止显示，内容保存，减少耗电）
*/
void ST7789V_SleepModeCmd(FunctionalState NewState);
/*
*Parameter:WRCACE_InitStruct,
*					初始结构体
*Role     :
*         亮度自适应，颜色增强配置
*/
void ST7789V_AdaptiveBrightnessColorEnhancementInit(WRCACE_InitTypedef*WRCACE_InitStruct);
/*
*Parameter:TopStill_H
*					顶部静止高度
*Parameter:VerticalScroll_H,
*					滚动区域高度
*Parameter:BottomStill_H,
*					底部静止高度
*Parameter:StartAddr,
*					开始的行(注：范围为TopStill_H~VerticalScroll_H+TopStill_H)
*Parameter:ScrollTime,
*					刷新速度（建议最大50）
*Return   :
*					当前起始行
*Role     :
*         配置并开始垂直滚动（注：放入循环！！！）
*/
uint16_t ST7789V_VerticalScrollMode(uint16_t TopStill_H,uint16_t VerticalScroll_H,uint16_t BottomStill_H,uint16_t StartAddr,uint16_t ScrollTime);
/*
*Parameter:Start_X
*					起始点x
*Parameter:Start_Y,
*					起始点y
*Parameter:End_X,
*					结束点x
*Parameter:End_Y,
*					结束点y
*Parameter:Dir_Mode,
*					设置当前显示方向下的坐标系
*Role     :
*         设置显示范围
*/
void ST7789V_SetLcdAddress(uint16_t Start_X,uint16_t Start_Y,uint16_t End_X,uint16_t End_Y,uint8_t Dir_Mode);
/*
*Parameter:Dir_Mode,
*					显示方向
*Role     :
*         设置当前显示方向
*/
void ST7789V_SetDir(uint8_t Dir_Mode);
/*
*Role     :
*         复位
*/
void ST7789V_Reset(void);
/*
*Role     :
*         lcd初始化
*/
void ST7789V_LcdInit(void);
/*
*Parameter:Color,
*					颜色
*Role     :
*         发送一个颜色数据
*/
void ST7789V_SendOneColor(uint16_t Color);
/*
*Parameter:Start_X
*					起始点x
*Parameter:Start_Y,
*					起始点y
*Parameter:End_X,
*					结束点x
*Parameter:End_Y,
*					结束点y
*Parameter:Color,
*					颜色
*Role     :
*         填充该矩形区域为某一颜色
*/
void ST7789V_FillLcdScreen(uint16_t Start_X,uint16_t Start_Y,uint16_t End_X,uint16_t End_Y,uint16_t* Color,uint8_t Dir_Mode);
/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Color,
*					颜色
*Role     :
*         花一点，添色
*/
void ST7789V_LcdDrawPoint(uint16_t X,uint16_t Y,uint16_t Color,uint8_t Dir_Mode);
/*
*Parameter:Start_X
*					起始点x
*Parameter:Start_Y,
*					起始点y
*Parameter:End_X,
*					结束点x
*Parameter:End_Y,
*					结束点y
*Parameter:Color,
*					颜色
*Role     :
*         画线，添色
*/
void ST7789V_LcdDrawLine(uint16_t Start_X,uint16_t Start_Y,uint16_t End_X,uint16_t End_Y,uint16_t Color,uint8_t Dir_Mode);
/*
*Parameter:Start_X
*					起始点x
*Parameter:Start_Y,
*					起始点y
*Parameter:End_X,
*					结束点x
*Parameter:End_Y,
*					结束点y
*Parameter:Color,
*					颜色
*Role     :
*         画矩形，添色
*/
void ST7789V_LcdDrawRectangle(uint16_t Start_X,uint16_t Start_Y,uint16_t End_X,uint16_t End_Y,uint16_t Color,uint8_t Dir_Mode);
/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:R,
*					半径
*Parameter:Color,
*					颜色
*Role     :
*         画圆，添色
*/
void ST7789V_LcdDrawCircle(uint16_t X,uint16_t Y,uint8_t R,uint16_t Color,uint8_t Dir_Mode);
/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Str,
*					字符串
*Parameter:Word_Color,
*					字颜色
*Parameter:Back_Color,
*					背景颜色
*Parameter:Size,
*					字的型号
*Parameter:Mode,
*					0非叠加模式  1叠加模式
*Role     :
*         显示以上四种字体格式字符
*/
void ST7789V_LcdShowChinese(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode);
/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Char,
*					字符
*Parameter:Word_Color,
*					字颜色
*Parameter:Back_Color,
*					背景颜色
*Parameter:Size,
*					字的型号
*Parameter:Mode,
*					0非叠加模式  1叠加模式
*Role     :
*         显示以上四种字体格式字符
*/
void ST7789V_LcdShowChar(uint16_t X,uint16_t Y,uint8_t Char,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode);
/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Str,
*					字符串
*Parameter:Word_Color,
*					字颜色
*Parameter:Back_Color,
*					背景颜色
*Parameter:Size,
*					字的型号
*Parameter:Mode,
*					0非叠加模式  1叠加模式
*Role     :
*         显示以上四种字体格式字符
*/
void ST7789V_LcdShowString(uint16_t X,uint16_t Y,const uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode);
/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Number,
*					数字整型部分
*Parameter:Len,
*					数字整型部分长度
*Parameter:Word_Color,
*					字颜色
*Parameter:Back_Color,
*					背景颜色
*Parameter:Size,
*					字的型号
*Role     :
*         显示以上四种字体格式数字
*/
void ST7789V_LcdShowIntNum(uint16_t X,uint16_t Y,uint16_t Number,uint8_t Len,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Dir_Mode);
/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Number,
*					数字浮点型小数部分
*Parameter:Len,
*					数字浮点型小数部分长度
*Parameter:Word_Color,
*					字颜色
*Parameter:Back_Color,
*					背景颜色
*Parameter:Size,
*					字的型号
*Role     :
*         显示以上四种字体格式数字
*/
void ST7789V_LcdShowFloatNum(uint16_t X,uint16_t Y,float Number,uint8_t Len,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Dir_Mode);
/*
*Parameter:X,
*					点x
*Parameter:Y,
*					点y
*Parameter:Width,
*					图片宽度
*Parameter:Length,
*					图片长度
*Parameter:Pic,
*					图片数组
*Role     :
*         显示图片
*/
void ST7789V_LcdShowPicture(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Length,uint16_t Pic[],uint8_t Dir_Mode);

#endif
