#ifndef _font_show_H
#define _font_show_H

#include "system.h"
#include "font_update.h"

void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			//得到汉字的点阵码
void LCD_Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode);					//在指定位置显示一个汉字
void LCD_ShowFontString(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode);	//在指定位置显示一个字符串 
void LCD_ShowFontStringMid(u16 x,u16 y,u8*str,u8 size,u8 len);


#endif
