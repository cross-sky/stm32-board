#include "cominc.h"

MENUS menus_tab[]={
	{	1,
	{"dso","mpu6050","other"},
	10,10,80,16,
	3,XDIR,CL_BLACK,CL_BLUE,
	NULL,NULL,NULL,NULL,
	0
	},
	
	{	11,
	{"dso_wave","dso_set"},
	10,50,20,40,
	2,YDIR,CL_BLACK,CL_BLUE,
	NULL,NULL,NULL,NULL,
	0
	},
	
	{	111,
	{"dso_wave"},
	10,0,20,20,
	1,YDIR,CL_BLACK,CL_BLUE,
	NULL,NULL,NULL,NULL,
	0
	},
	
	{	112,
	{"dso_set"},
	10,0,20,20,
	1,YDIR,CL_BLACK,CL_BLUE,
	NULL,NULL,NULL,NULL,
	0
	},
	
	{	12,
	{"mpu6050_wave","mpu6050_set"},
	10,50,20,40,
	2,YDIR,CL_BLACK,CL_BLUE,
	NULL,NULL,NULL,NULL,
	0
	},
	
	{	121,
	{"mpu6050_wave"},
	10,0,20,20,
	1,YDIR,CL_BLACK,CL_BLUE,
	NULL,NULL,NULL,NULL,
	0
	},
	
	{	122,
	{"mpu6050_set"},
	10,0,20,20,
	1,YDIR,CL_BLACK,CL_BLUE,
	NULL,NULL,NULL,NULL,
	0
	},
	
	{	13,
	{"temp_disp","tim_set"},
	10,50,20,40,
	2,YDIR,CL_BLACK,CL_BLUE,
	NULL,NULL,NULL,NULL,
	0
	},
	
	{	131,
	{"temp_disp"},
	10,0,20,20,
	1,YDIR,CL_BLACK,CL_BLUE,
	NULL,NULL,NULL,NULL,
	0
	},
	
	{	132,
	{"tim_set"},
	10,0,20,20,
	1,YDIR,CL_BLACK,CL_BLUE,
	NULL,NULL,NULL,NULL,
	0
	},
	
	{-1,}

};

typedef struct fmenu_table{
	int16_t		MenuId;
	void	(*pfmenu)(void);
} FUNTAB;

FUNTAB  FunctionTab[]={
	111,Menu111_DsoWave,112,Menu112_DsoSET,
	121,Menu121_MpuWave,122,Menu122_MpuSet,
	131,Menu131_Temp,	132,Menu132_Time,
};

MENUS *pmenu;

MENUS *GetPmenuFromID(int16_t id);
MENUS *MenuSelect(MENUS *pmenu,BUTTON_STATUS_t key);

//第三级菜单中，获取按键退出时，才返回pmenu->up
void MenusDisplay(void)
{
	u8 i;
	BUTTON_STATUS_t key_status;
	
	if(qBTNLink->front != qBTNLink->rear &&pmenu->down != NULL)
	{
		BTN_DeQueue(qBTNLink,&key_status);
//	key_status = BUTTON[0].BUTTON_STATUS;   //......................................
	pmenu = MenuSelect(pmenu,key_status);
	}
	
	if (pmenu->total == 1)
	{
		i=0;
		do 
		{
			if(FunctionTab[i].MenuId == pmenu->ID) break;
			i++;
		} while (1);
		FunctionTab[i].pfmenu();
		
			if(qBTNLink->front != qBTNLink->rear)
	{
		BTN_DeQueue(qBTNLink,&key_status);
		switch(key_status){
		case BTN_BACK: TaskComps[TASK_DSO].Suspend = 1;
						TaskComps[TASK_RDADC].Suspend = 1;
						Lcd_Clear(CL_BLUE);
						pmenu = pmenu->up;
						MenuWinDisplay(pmenu);
						MenuCurDisplay(pmenu,pmenu->cur);
						break;
		default: break;
		}
	}
	}
}

void InitMenuTree(void)
{
	u8 i =0;
	do 
	{
		if(menus_tab[i].ID == -1) break;
		menus_tab[i].up	= GetPmenuFromID(menus_tab[i].ID/10);
		menus_tab[i].down = GetPmenuFromID(menus_tab[i].ID*10+1);
		menus_tab[i].right = GetPmenuFromID(menus_tab[i].ID+1);
		menus_tab[i].left = GetPmenuFromID(menus_tab[i].ID-1);
		i++;
	} while (1);
	pmenu = &menus_tab[0];
	MenuWinDisplay(pmenu);
	MenuCurDisplay(pmenu,pmenu->cur);
}

MENUS *GetPmenuFromID(int16_t id)
{
	u8 i = 0;
	do 
	{
		if(menus_tab[i].ID == -1) break;
		if(menus_tab[i].ID == id)
			return(&menus_tab[i]);
		i++;
	} while (1);

	return(NULL);
}


void MenuWinDisplay(MENUS *pmenu)
{
	u8 i;
	FONT_T tfont;
	for (i=0;i<pmenu->total;i++)
	{
		tfont.usTextColor = pmenu->wcolor;
		tfont.usBackColor = pmenu->bcolor;
		
		if(pmenu->dir == XDIR)
		{		
			Gui_DrawFont_GBK16(i*pmenu->width+pmenu->x0,pmenu->y0,pmenu->name[i],&tfont);
		}
		else
			Gui_DrawFont_GBK16(pmenu->x0,i*pmenu->high+pmenu->y0,pmenu->name[i],&tfont);	
	}
}

void DelMenuWin(MENUS *pmenu)
{
	u8 i;
	FONT_T tfont;
	tfont.usTextColor = CL_BLUE;
	tfont.usBackColor = CL_BLUE;
	for (i=0;i<pmenu->total;i++)
	{
		if(pmenu->dir == XDIR)
		{
			//			Lcd_SetXY(10+i*(pmenu->width/pmenu->total,10));
			Gui_DrawFont_GBK16(i*pmenu->width+pmenu->x0,pmenu->y0,pmenu->name[i],&tfont);
		}
		else
			//			Lcd_SetXY(10,(i+1)<<4);
			Gui_DrawFont_GBK16(pmenu->x0,i*pmenu->high+pmenu->y0,pmenu->name[i],&tfont);

	}
}

void MenuCurDisplay(MENUS *pmenu, int16_t cur)
{
	FONT_T tfont;
	tfont.usBackColor = CL_BLUE2;
	tfont.usTextColor = pmenu->wcolor;

	if(pmenu->dir == XDIR)
		Gui_DrawFont_GBK16(cur*pmenu->width+pmenu->x0,pmenu->y0,pmenu->name[cur],&tfont);
	else
		Gui_DrawFont_GBK16(pmenu->x0,cur*pmenu->high+pmenu->y0,pmenu->name[cur],&tfont);

}

void DelMenuCur(MENUS *pmenu, int16_t cur)
{
	FONT_T tfont;
	tfont.usBackColor = CL_BLUE;
	tfont.usTextColor = pmenu->wcolor;

	if(pmenu->dir == XDIR)
		Gui_DrawFont_GBK16(cur*pmenu->width+pmenu->x0,pmenu->y0,pmenu->name[cur],&tfont);
	else
		Gui_DrawFont_GBK16(pmenu->x0,cur*pmenu->high+pmenu->y0,pmenu->name[cur],&tfont);

}

MENUS *MenuSelect(MENUS *pmenu,BUTTON_STATUS_t key)
{
	int cur, i;
	switch(key){
	case BTN_RIGHT:
//		if(pmenu->dir == YDIR) break;
		DelMenuCur(pmenu,pmenu->cur++);
		if(pmenu->cur == pmenu->total) pmenu->cur=0;
		MenuCurDisplay(pmenu,pmenu->cur);
		break;
	case BTN_LEFT:
//		if(pmenu->dir == YDIR) break;
		DelMenuCur(pmenu,pmenu->cur--);
		if(pmenu->cur < 0) pmenu->cur=pmenu->total-1;
		MenuCurDisplay(pmenu,pmenu->cur);
		break;
// 	case UP_KEY:
// 		if(pmenu->dir == XDIR) break;
// 		DelMenuCur(pmenu,pmenu->cur++);
// 		if(pmenu->cur == pmenu->total) pmenu->cur=0;
// 		MenuCurDisplay(pmenu,pmenu->cur);
// 		break;
// 	case DOWN_KEY:
// 		if(pmenu->dir == XDIR) break;
// 		DelMenuCur(pmenu,pmenu->cur--);
// 		if(pmenu->cur < 0) pmenu->cur=pmenu->total-1;
// 		MenuCurDisplay(pmenu,pmenu->cur);
// 		break;
	case BTN_EN:
		if (pmenu->down != NULL)
		{
			cur = pmenu->cur;
			DelMenuWin(pmenu);
			pmenu = pmenu->down;
//		
//	  Lcd_Clear(CL_BLUE);
//		DelMenuCur(pmenu,pmenu->cur);
		for(i=0;i<cur;i++)
			pmenu = pmenu->right;
		if(pmenu->total > 1)
		{
			pmenu->cur = 0;
			MenuWinDisplay(pmenu);
			MenuCurDisplay(pmenu,pmenu->cur);
		}
   }
		break;

	case BTN_BACK:
		if (pmenu->up != NULL)
		{
//			DelMenuWin(pmenu);
			Lcd_Clear(CL_BLUE);
			pmenu = pmenu->up;
 			MenuWinDisplay(pmenu);
 			MenuCurDisplay(pmenu,pmenu->cur);
		}
		break;
	default:		break;
	}
	return(pmenu);
}

void Menu111_DsoWave(void)
{
	
	if (TaskComps[TASK_DSO].Suspend == 1)
	{
		TaskComps[TASK_DSO].Suspend = 0;  //
		TaskComps[TASK_RDADC].Suspend = 0;
		TIM_Cmd(TIM2, ENABLE);
	}
}

void Menu112_DsoSET(void)
{
	FmenuProcess(112);

}

void Menu121_MpuWave(void)
{
	FmenuProcess(121);

}

void Menu122_MpuSet(void)
{
	FmenuProcess(122);

}

void Menu131_Temp(void)
{
	FmenuProcess(131);
}

void Menu132_Time(void)
{
	FmenuProcess(132);
}

void FmenuProcess(int16_t i)
{
	char buf[10];
	FONT_T tfont;
	tfont.usBackColor = CL_BLUE2;
	tfont.usTextColor = CL_RED;
	sprintf(buf,"%d",i);
	Gui_DrawFont_GBK16(70,70,buf,&tfont);
  
}

