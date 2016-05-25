#ifndef __STM32_UB_GUI_H
#define __STM32_UB_GUI_H

#include "stm32f10x.h"


extern void Gui_Circle(u16 X,u16 Y,u16 R,u16 fc); 
extern void Gui_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color);  
extern void Gui_box(u16 x, u16 y, u16 w, u16 h,u16 bc);
extern void Gui_box2(u16 x,u16 y,u16 w,u16 h, u8 mode);
extern void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2);
extern void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2);
void Gui_DrawFont_GBK16(u16 x, u16 y, char *s ,FONT_T *tFont);
//extern void Gui_DrawFont_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s);
//extern void Gui_DrawFont_Num32(u16 x, u16 y, u16 fc, u16 bc, u16 num) ;
extern void Gui_DrawFont_Num16(u16 x, u16 y, u16 fc, u16 bc, u16 num);
extern void LCD_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void LCD_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t _usColor);


#endif

