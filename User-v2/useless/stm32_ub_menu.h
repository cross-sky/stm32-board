#ifndef __STM32_UB_MENU_H
#define __STM32_UB_MENU_H

#include "stm32_ub_lcd_drive.h"
#include<stdio.h>
//#define NULL ((void *)0)

enum DIRECTION{XDIR,YDIR};

typedef struct menu_struct{
	int16_t		ID;
	char			*name[10];
	u16			x0,y0,width,high;
	u8			total,dir;
	u16			wcolor,bcolor;
	struct menu_struct *up,*down,*left,*right;
	s8			cur;
}MENUS;



void InitMenuTree(void);  //初始化菜单
void MenuWinDisplay(MENUS *pmenu);
void DelMenuWin(MENUS *pmenu);
void MenuCurDisplay(MENUS *pmenu, int16_t cur);
void DelMenuCur(MENUS *pmenu, int16_t cur);
void FmenuProcess(int16_t i);
void MenusDisplay(void);//菜单显示函数

void Menu111_DsoWave(void);
void Menu112_DsoSET(void);
void Menu121_MpuWave(void);
void Menu122_MpuSet(void);
void Menu131_Temp(void);
void Menu132_Time(void);


#endif

