#ifndef _font_show_H
#define _font_show_H

#include "system.h"
#include "font_update.h"

void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			//�õ����ֵĵ�����
void LCD_Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode);					//��ָ��λ����ʾһ������
void LCD_ShowFontString(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode);	//��ָ��λ����ʾһ���ַ��� 
void LCD_ShowFontStringMid(u16 x,u16 y,u8*str,u8 size,u8 len);


#endif
