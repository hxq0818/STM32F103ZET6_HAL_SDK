
/**************����BվUP��Sangk-Hu****************/

/*****************QQȺ��690367095******************/

/****��עUP��˽�Ż��߼�Ⱥ������ϻ�����������*****/

/********************************************/
/*���þ�ȷ��ʱ��ʹ��SysTick_Init��ʼ��ʱ����*/
/********************************************/
#ifndef _st7789v_H
#define _st7789v_H

#include <stm32f10x.h>
#include "system.h"   

#define SPI_DMA 0

/*��һ�εĴ������ݳ���*/
extern uint16_t Tx_Len;

/*ST7789V_AdaptiveBrightnessColorEnhancementInit��ʼ���ṹ��*/
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
*ST7789V��֧�ֵõ����ֱ���Ϊ240*320������ʾ��֧��240*280������ƫ����
*ƫ���������������320-280��/2=20
*��USE_HORIZONTAL==1ʱΪ��׼��������
*/
#define WIDTH_OFFSET 		 0
#define HIGH_OFFSET 		 20

#define USE_HORIZONTAL 	 0  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 					 240
#define LCD_H 					 280

#else
#define LCD_W 					 280
#define LCD_H 					 240
#endif

//������ɫ
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
#define BROWN 			     (uint16_t)0XBC40 //��ɫ
#define BRRED 			     (uint16_t)0XFC07 //�غ�ɫ
#define GRAY  			     (uint16_t)0X8430 //��ɫ
#define DARKBLUE      	 (uint16_t)0X01CF	//����ɫ
#define LIGHTBLUE      	 (uint16_t)0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 (uint16_t)0X5458 //����ɫ
#define LIGHTGREEN     	 (uint16_t)0X841F //ǳ��ɫ
#define LGRAY 			     (uint16_t)0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        (uint16_t)0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           (uint16_t)0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

/*����ܽ�GPIOx��RCC*/
#define ST7789V_RESET_RCC_GPIO		RCC_APB2Periph_GPIOA
#define ST7789V_RS_RCC_GPIO 			RCC_APB2Periph_GPIOA
#define ST7789V_NSS_RCC_GPIO  		RCC_APB2Periph_GPIOA
#define ST7789V_SCK_RCC_GPIO 			RCC_APB2Periph_GPIOA
#define ST7789V_SDA_RCC_GPIO			RCC_APB2Periph_GPIOA
/*����ܽ�GPIOx*/
#define ST7789V_RESET_GPIO				GPIOA
#define ST7789V_RS_GPIO 					GPIOA
#define ST7789V_NSS_GPIO  				GPIOA
#define ST7789V_SCK_GPIO 					GPIOA
#define ST7789V_SDA_GPIO					GPIOA
/*����ܽ�λ��*/
#define ST7789V_RESET_PIN 				GPIO_Pin_1
#define ST7789V_RS_PIN 						GPIO_Pin_2
#define ST7789V_NSS_PIN   				GPIO_Pin_3
#define ST7789V_SCK_PIN 					GPIO_Pin_5
#define ST7789V_SDA_PIN 					GPIO_Pin_7
/*����λ����*/
#define ST7789V_RESET 						PAout(1)
#define ST7789V_RS 								PAout(2)
#define ST7789V_NSS								PAout(3)
/*����˿ڲ�������*/
#define ST7789V_RESET_HIGH() 			ST7789V_RESET=1
#define ST7789V_RESET_LOW() 			ST7789V_RESET=0
#define ST7789V_RS_HIGH() 				ST7789V_RS=1
#define ST7789V_RS_LOW() 					ST7789V_RS=0
#define ST7789V_NSS_HIGH()				ST7789V_NSS=1
#define ST7789V_NSS_LOW()					ST7789V_NSS=0
/*�������SPI��ʼ���꣬�Լ���DMA�ĺ�*/
#define ST7789V_SPIX 							SPI1
#define ST7789V_RCC_SPIX					RCC_APB2Periph_SPI1

/*ʹ��DMA*/
#define ST7789V_SPIX_DMA

#ifdef ST7789V_SPIX_DMA
#define ST7789V_SPIX_ADD					(uint32_t)(&(ST7789V_SPIX->DR))
#define ST7789V_RCC_DMA						RCC_AHBPeriph_DMA1
#define ST7789V_DMA_SPIX_TX_FLAG	DMA1_FLAG_TC3
#define ST7789V_DMA_SPIX_TX_CH		DMA1_Channel3
#endif

/*
*Parameter:NewState,
*					�Ƿ�ʹ��
*Parameter:Start_Line,
*					�ֲ���ʾ��ʼ��ַ
*Parameter:End_Line,
*					�ֲ���ʾ������ַ
*Role     :
*         �ֲ���ʾ
*/
void ST7789V_PartialDisplayModeCmd(FunctionalState NewState,uint16_t Start_Line,uint16_t End_Line);
/*
*Parameter:NewState,
*					�Ƿ�ʹ��
*Role     :
*         ����ģʽ���͹أ���ͼ��Ҫ�󲻸��£����ٺĵ磩
*/
void ST7789V_IdleModeCmd(FunctionalState NewState);
/*
*Parameter:NewState,
*					�Ƿ�ʹ��
*Role     :
*         ��ʾ���͹�
*/
void ST7789V_DisplayCmd(FunctionalState NewState);
/*
*Parameter:NewState,
*					�Ƿ�ʹ��
*Role     :
*         ˯��ģʽ���͹أ��ر���ʾ��ֹͣ��ʾ�����ݱ��棬���ٺĵ磩
*/
void ST7789V_SleepModeCmd(FunctionalState NewState);
/*
*Parameter:WRCACE_InitStruct,
*					��ʼ�ṹ��
*Role     :
*         ��������Ӧ����ɫ��ǿ����
*/
void ST7789V_AdaptiveBrightnessColorEnhancementInit(WRCACE_InitTypedef*WRCACE_InitStruct);
/*
*Parameter:TopStill_H
*					������ֹ�߶�
*Parameter:VerticalScroll_H,
*					��������߶�
*Parameter:BottomStill_H,
*					�ײ���ֹ�߶�
*Parameter:StartAddr,
*					��ʼ����(ע����ΧΪTopStill_H~VerticalScroll_H+TopStill_H)
*Parameter:ScrollTime,
*					ˢ���ٶȣ��������50��
*Return   :
*					��ǰ��ʼ��
*Role     :
*         ���ò���ʼ��ֱ������ע������ѭ����������
*/
uint16_t ST7789V_VerticalScrollMode(uint16_t TopStill_H,uint16_t VerticalScroll_H,uint16_t BottomStill_H,uint16_t StartAddr,uint16_t ScrollTime);
/*
*Parameter:Start_X
*					��ʼ��x
*Parameter:Start_Y,
*					��ʼ��y
*Parameter:End_X,
*					������x
*Parameter:End_Y,
*					������y
*Parameter:Dir_Mode,
*					���õ�ǰ��ʾ�����µ�����ϵ
*Role     :
*         ������ʾ��Χ
*/
void ST7789V_SetLcdAddress(uint16_t Start_X,uint16_t Start_Y,uint16_t End_X,uint16_t End_Y,uint8_t Dir_Mode);
/*
*Parameter:Dir_Mode,
*					��ʾ����
*Role     :
*         ���õ�ǰ��ʾ����
*/
void ST7789V_SetDir(uint8_t Dir_Mode);
/*
*Role     :
*         ��λ
*/
void ST7789V_Reset(void);
/*
*Role     :
*         lcd��ʼ��
*/
void ST7789V_LcdInit(void);
/*
*Parameter:Color,
*					��ɫ
*Role     :
*         ����һ����ɫ����
*/
void ST7789V_SendOneColor(uint16_t Color);
/*
*Parameter:Start_X
*					��ʼ��x
*Parameter:Start_Y,
*					��ʼ��y
*Parameter:End_X,
*					������x
*Parameter:End_Y,
*					������y
*Parameter:Color,
*					��ɫ
*Role     :
*         ���þ�������Ϊĳһ��ɫ
*/
void ST7789V_FillLcdScreen(uint16_t Start_X,uint16_t Start_Y,uint16_t End_X,uint16_t End_Y,uint16_t* Color,uint8_t Dir_Mode);
/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Color,
*					��ɫ
*Role     :
*         ��һ�㣬��ɫ
*/
void ST7789V_LcdDrawPoint(uint16_t X,uint16_t Y,uint16_t Color,uint8_t Dir_Mode);
/*
*Parameter:Start_X
*					��ʼ��x
*Parameter:Start_Y,
*					��ʼ��y
*Parameter:End_X,
*					������x
*Parameter:End_Y,
*					������y
*Parameter:Color,
*					��ɫ
*Role     :
*         ���ߣ���ɫ
*/
void ST7789V_LcdDrawLine(uint16_t Start_X,uint16_t Start_Y,uint16_t End_X,uint16_t End_Y,uint16_t Color,uint8_t Dir_Mode);
/*
*Parameter:Start_X
*					��ʼ��x
*Parameter:Start_Y,
*					��ʼ��y
*Parameter:End_X,
*					������x
*Parameter:End_Y,
*					������y
*Parameter:Color,
*					��ɫ
*Role     :
*         �����Σ���ɫ
*/
void ST7789V_LcdDrawRectangle(uint16_t Start_X,uint16_t Start_Y,uint16_t End_X,uint16_t End_Y,uint16_t Color,uint8_t Dir_Mode);
/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:R,
*					�뾶
*Parameter:Color,
*					��ɫ
*Role     :
*         ��Բ����ɫ
*/
void ST7789V_LcdDrawCircle(uint16_t X,uint16_t Y,uint8_t R,uint16_t Color,uint8_t Dir_Mode);
/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Str,
*					�ַ���
*Parameter:Word_Color,
*					����ɫ
*Parameter:Back_Color,
*					������ɫ
*Parameter:Size,
*					�ֵ��ͺ�
*Parameter:Mode,
*					0�ǵ���ģʽ  1����ģʽ
*Role     :
*         ��ʾ�������������ʽ�ַ�
*/
void ST7789V_LcdShowChinese(uint16_t X,uint16_t Y,uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode);
/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Char,
*					�ַ�
*Parameter:Word_Color,
*					����ɫ
*Parameter:Back_Color,
*					������ɫ
*Parameter:Size,
*					�ֵ��ͺ�
*Parameter:Mode,
*					0�ǵ���ģʽ  1����ģʽ
*Role     :
*         ��ʾ�������������ʽ�ַ�
*/
void ST7789V_LcdShowChar(uint16_t X,uint16_t Y,uint8_t Char,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode);
/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Str,
*					�ַ���
*Parameter:Word_Color,
*					����ɫ
*Parameter:Back_Color,
*					������ɫ
*Parameter:Size,
*					�ֵ��ͺ�
*Parameter:Mode,
*					0�ǵ���ģʽ  1����ģʽ
*Role     :
*         ��ʾ�������������ʽ�ַ�
*/
void ST7789V_LcdShowString(uint16_t X,uint16_t Y,const uint8_t*Str,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Mode,uint8_t Dir_Mode);
/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Number,
*					�������Ͳ���
*Parameter:Len,
*					�������Ͳ��ֳ���
*Parameter:Word_Color,
*					����ɫ
*Parameter:Back_Color,
*					������ɫ
*Parameter:Size,
*					�ֵ��ͺ�
*Role     :
*         ��ʾ�������������ʽ����
*/
void ST7789V_LcdShowIntNum(uint16_t X,uint16_t Y,uint16_t Number,uint8_t Len,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Dir_Mode);
/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Number,
*					���ָ�����С������
*Parameter:Len,
*					���ָ�����С�����ֳ���
*Parameter:Word_Color,
*					����ɫ
*Parameter:Back_Color,
*					������ɫ
*Parameter:Size,
*					�ֵ��ͺ�
*Role     :
*         ��ʾ�������������ʽ����
*/
void ST7789V_LcdShowFloatNum(uint16_t X,uint16_t Y,float Number,uint8_t Len,uint16_t Word_Color,uint16_t Back_Color,uint8_t Size,uint8_t Dir_Mode);
/*
*Parameter:X,
*					��x
*Parameter:Y,
*					��y
*Parameter:Width,
*					ͼƬ���
*Parameter:Length,
*					ͼƬ����
*Parameter:Pic,
*					ͼƬ����
*Role     :
*         ��ʾͼƬ
*/
void ST7789V_LcdShowPicture(uint16_t X,uint16_t Y,uint16_t Width,uint16_t Length,uint16_t Pic[],uint8_t Dir_Mode);

#endif
