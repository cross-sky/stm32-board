#include "cominc.h"


// main menu handle and clock set
void fViewCoreParam(void);
void DisplayRecT(void);

void MenuOnStatus(void)
{
	uint16_t keyValue=0;

	keyValue = KeyPop();
	switch(keyValue)
	{
	case BTN_UP: fAddCoreParam(0);break;
	case BTN_DOWN:fSubCoreParam(0);break;
	case BTN_CLOCK:	MenuParam.pfmenu = MenuSetClock; MenuLcd();return; //break;
	case BTN_TIMER: MenuParam.pfmenu = MenuSetTime; lcd_wr_char(_lcd23_timeOn,1);lcd_wr_char(_lcd24_timeOff, 0);MenuLcd(); return; // break;
	case BTN_LOCK:	MenuParam.lockFlag ^= 1;break;//need to add lock ico
	case BTN_ELEC:	MenuParam.elecFlag ^= 1; 
					MenuParam.StartParamChange = 1;
					lcd_wr_char(_lcd1_elec, MenuParam.elecFlag); break;
	case BTN_SHUT:  lcd_wr_char(_lcd3_run, 0); 
					lcd_wr_char(_lcd10_hotWater, 0);
					MenuParam.runFlag ^= 0x01;
					MenuParam.StartParamChange=1;
					MenuParam.pfmenu =MenuOffStatus; break;
	case BTN_SETCORE:MenuParam.pfmenu = fViewCoreParam; lcd_wr_char(_lcd5_find,1);break;
	case BTN_VIEWRC: MenuParam.pfmenu = DisplayRecT; lcd_wr_char(_lcd5_find,1);break;
	default:	break;
	}

	DispLayWatetT();
	fClockOn(MenuParam.clock.hour, MenuParam.clock.min);
	LcdSetWater(0);
	//lcd_wr_char(_lcd11_waterT, 1);//@@@@@@@@@
	//lcd_wr_char(_lcd13_c, 1);//c@@@@@
	//lcd_wr_char(_lcd3_run, 1);//run
	lcd_wr_char(_lcd4_lock, MenuParam.lockFlag);//lock
	//pfmenu();
}

void fClockOn(uint8_t hour, uint8_t min)
{
	uint8_t i,j;
	i = hour / 10;
	j = hour % 10 ;

	lcd_wr_num(i, 6, 1);
	lcd_wr_num(j, 7, 1);

	i = min / 10;
	j = min % 10 ;

	lcd_wr_num(i, 8, 1);
	lcd_wr_num(j, 9, 1);
}

void ClockFlash(uint8_t number, uint8_t position, uint8_t flag_flash)
{
	uint8_t i,j;
	//static uint8_t flag_flash=0;

	i = number / 10;
	j = number % 10 ;

	lcd_wr_num(i, position, flag_flash);
	lcd_wr_num(j, position+1, flag_flash);

	//flag_flash ^= 0x01;

	//if (flag_flash == 0)
	//{
	//	lcd_wr_num(i, position, 1);// flash on when i=0, will not display; 0(i high)0(j low)
	//	lcd_wr_num(j, position+1, 1);// low position must be display
	//}
	//else
	//{
	//	lcd_wr_num(i, position, 0);// flash off when i=0, will not display; 0(i high)0(j low)
	//	lcd_wr_num(j, position+1, 0);// low position will not display
	//}
	//flag_flash++;

}

uint8_t addHourMin(uint8_t number, uint8_t isHour)
{
	number++;	
	if (isHour == 0)	//hours add
	{
		if (number > 23)
		{
			number = 0;
		}
	}
	else
	{
		if (number > 59)// min add
		{
			number = 0;
		}
	}
	return number;
}

uint8_t subHourMin(uint8_t number, uint8_t isHour)
{
	number--;	
	if (isHour == 0)	//hours add
	{
		if (number == 0xff)
		{
			number = 23;
		}
	}
	else
	{
		if (number == 0xff)// min add
		{
			number = 59;
		}
	}
	return number;
	//ClockFlash(number, pos);//flash
}

void MenuSetClock(void)
{
	uint16_t keyValue=0;// when clock is setted ,need to keep the value on lcd,can't erase it
	static uint8_t numbs=0, pos=0;
	static uint8_t thour, tmin, isFirstFlag=1;
	static uint8_t flag_flash=0;
	uint8_t pcf853[3] = {0};
	if (isFirstFlag == 1)
	{
		thour = MenuParam.clock.hour;
		tmin = MenuParam.clock.min;
		isFirstFlag = 0;

		pos = MenuParam.clock.posHour;
		numbs = thour;
		flag_flash=0;
	}

	keyValue = KeyPop();
	switch(keyValue)
	{
	case BTN_UP:
		{
			numbs = addHourMin(numbs, MenuParam.clock.flag);
			//thour = numbs;
			//MenuParam.clock.hour = numbs;
			break;
		}
	case BTN_DOWN:
		{
			numbs = subHourMin(numbs, MenuParam.clock.flag);
			//tmin = numbs;
			//MenuParam.clock.min = numbs;
			break;
		}
	case BTN_CLOCK:
		{
			MenuParam.clock.flag++;//first click set hour, second click set min

			if (MenuParam.clock.flag  == 1)
			{
				flag_flash=0;
				
				pos = MenuParam.clock.posMin;
				thour = numbs;
				numbs = tmin;
				fClockOn(thour, tmin);// is nessary to clock on?== when next number chang,have to keep pre-number display
			}

			if (MenuParam.clock.flag > 1)
			{
				tmin = numbs;
				MenuParam.clock.flag = 0;
				MenuParam.pfmenu = MenuOnStatus; //on the third click,then save and quit
				MenuParam.clock.hour =thour;
				MenuParam.clock.min = tmin;
				isFirstFlag = 1;// 
				fClockOn(thour, tmin);

				pcf853[0]=0;
				pcf853[1]=tmin;
				pcf853[2]=thour;
				Pcf8563SetTime(pcf853);
				return ;
			}
			break;
		}
	case BTN_SHUT:
		{
			MenuParam.pfmenu = MenuOnStatus;
			MenuParam.clock.flag = 0; // quit without save
			isFirstFlag = 1;
			fClockOn(MenuParam.clock.hour, MenuParam.clock.min);
			return ;
			//break;
		}
	case BTN_LOCK:	MenuParam.lockFlag ^= 1;break;//need to add lock ico
	default: break;
	}
	DispLayWatetT();
	lcd_wr_char(_lcd4_lock, MenuParam.lockFlag);
	ClockFlash(numbs, pos,flag_flash);//flash
	flag_flash^= 0x01;
}

void MenuLcd(void)
{
	MenuParam.pfmenu();
}



void MenuSetTime(void)
{
	uint16_t keyValue=0;// when clock is setted ,need to keep the value on lcd,can't erase it
	static uint8_t numbs=0, pos=0;
	static uint8_t tonHour, tonMin, toffHour, toffMin, isFirstFlag=1;
	static uint8_t flag_flash=0;
	if (isFirstFlag == 1)
	{
		tonHour = MenuParam.timer.onHour;
		tonMin = MenuParam.timer.onMin;
		toffHour = MenuParam.timer.offHour;
		toffMin = MenuParam.timer.offMin;

		isFirstFlag = 0;
		flag_flash=0;
		pos = MenuParam.timer.posHour;//need to add timer on/off ico **********
		numbs = tonHour;
	
		fClockOn(tonHour, tonMin);
		return;
	}

	keyValue = KeyPop();
	switch(keyValue)
	{
	case BTN_UP:
		{
			numbs = addHourMin(numbs, MenuParam.timer.flag&0x01);
			break;
		}
	case BTN_DOWN:
		{
			numbs = subHourMin(numbs, MenuParam.timer.flag&0x01);
			break;
		}
	case BTN_TIMER:
		{
			MenuParam.timer.flag++;
			switch(MenuParam.timer.flag)
			{
			case 1: 
				{
					pos = MenuParam.timer.posMin;	//1. assign position
					tonHour = numbs;		//2.save  last numbers
					numbs = tonMin;	//3. assign new numbers
					fClockOn(tonHour, tonMin);

					break;
				}
			case 2:
				{
					lcd_wr_char(_lcd23_timeOn, 0);
					lcd_wr_char(_lcd24_timeOff, 1);
					pos = MenuParam.timer.posHour;	//1. assign position
					tonMin = numbs;		//2.save  last numbers
					numbs = toffHour;	//3. assign new numbers
					fClockOn(toffHour, toffMin);
					
					break;
				}
			case 3:
				{
					pos = MenuParam.timer.posMin;	//1. assign position
					toffHour = numbs;		//2.save  last numbers
					numbs = toffMin;	//3. assign new numbers
					fClockOn(toffHour, toffMin);
					
					break;
				}
			default:
				{
					lcd_wr_char(_lcd24_timeOff, 0);
					toffMin = numbs;				//quit and save
					MenuParam.timer.flag = 0;		//1. reset flag
					MenuParam.pfmenu = MenuOnStatus;	// 2.reset pfmenu
					MenuParam.timer.onHour = tonHour;	//3. save params
					MenuParam.timer.onMin = tonMin;
					MenuParam.timer.offHour = toffHour;
					MenuParam.timer.offMin = toffMin;
					isFirstFlag = 1;					//4. reset firstflag
					fClockOn(MenuParam.clock.hour, MenuParam.clock.min);//5. display clock
					CheckTimer(1);					
					return ;					
				}
			}
			break;
		}

	case  BTN_SHUT:
		{
			MenuParam.pfmenu = MenuOnStatus;
			MenuParam.timer.flag = 0; // quit without save
			isFirstFlag = 1;
			fClockOn(MenuParam.clock.hour, MenuParam.clock.min);//5. display clock
			CheckTimer(0);
			return ;
		}
	case BTN_LOCK:	MenuParam.lockFlag ^= 1;break;//need to add lock ico

	default: break;
	}

	DispLayWatetT();
	lcd_wr_char(_lcd4_lock, MenuParam.lockFlag);
	//ClockFlash(numbs, pos);//flash
	ClockFlash(numbs, pos,flag_flash);//flash
	flag_flash^= 0x01;
}


// pa6 beep pa7 bgled init with button
void fBeepOn(uint8_t *isBeep)
{
	BeepOff;

	if (*isBeep == 1)
	{
		BeepOn;
		*isBeep = 0;// set beepflag off, beeping untill next shift
	}
}

void TBeepOn(void)
{
	//beep 100ms
	fBeepOn(&MenuParam.beepFlag);
}

void TClockPointFlash(void)
{
	static uint8_t tflag=0;
	tflag++;
	lcd_wr_char(_lcd22_timePlus, tflag);
	tflag &=0x01;
}

void fBgledOn(uint8_t *isLightOn, uint8_t *isLock)
{
	static uint8_t ttime=0, tlightFlag=0, tlock=0;
	uint8_t i;
	if (*isLightOn == 1)
	{
		*isLightOn = 0;
		ttime = 0;
		tlightFlag = 1;
		LCD_LED_ON;
	}

	if (tlightFlag == 1)
	{
		ttime++;
		if (ttime > 9)//light on 10s
		{
			ttime = 0;

			tlock = 0;//reset tlock;

			tlightFlag = 0;
			LCD_LED_OFF;
			 //then  send coreparams
			if (NumCoreParam[0].isChange == 1)
			{
				i=eepWRCoreParam()^0x01;
				lcd_wr_char(_lcd6_err,i);
				NumCoreParam[0].isChange = 0;
			}

		}
	}

	if (ttime == 0)
	{
		tlock++;
		if (tlock > 100)
		{
			tlock = 0;   // when bgled off ,
			*isLock = 1; // if did not put anykey in 100s key,then lock
		}
	}
}

void TBgledOn(void)
{
	//1s run 
	fBgledOn(&MenuParam.bgledFlag, &MenuParam.lockFlag);
	TClockPointFlash();
}

void fAddCoreParam(uint8_t id)
{
	NumCoreParam[id].value++;
	if (NumCoreParam[id].value > NumCoreParam[id].max )
	{
		NumCoreParam[id].value = NumCoreParam[id].min;
	}
	NumCoreParam[0].isChange = 1;
	//MenuParam.CoreParam[id].value++;
	//if (MenuParam.CoreParam[id].value > MenuParam.CoreParam[id].max)
	//{
	//	MenuParam.CoreParam[id].value = MenuParam.CoreParam[id].min;
	//}
}

void fSubCoreParam(uint8_t id)
{
	NumCoreParam[id].value--;
	if (NumCoreParam[id].value < NumCoreParam[id].min )
	{
		NumCoreParam[id].value = NumCoreParam[id].max;
	}
	NumCoreParam[0].isChange = 1;
	//MenuParam.CoreParam[id].value--;
	//if (MenuParam.CoreParam[id].value < MenuParam.CoreParam[id].min)
	//{
	//	MenuParam.CoreParam[id].value = MenuParam.CoreParam[id].max;
	//}
}

void LcdSetWater(uint8_t id)
{
	uint8_t i=0, j=0;
	i = NumCoreParam[id].value / 10;
	j = NumCoreParam[id].value % 10;

	lcd_wr_num(0, 3, 0);
	lcd_wr_num(i, 4, 1);
	lcd_wr_num(j, 5, 1);
}

void fViewCoreParam(void)
{
	uint16_t keyValue=0;
	static uint8_t Id=0;

	keyValue = KeyPop();
	switch(keyValue)
	{
	case BTN_CLOCK: Id++; 
		if (Id >= CoreParamsMax)
		{
			Id = 0;
		}
		break;
	case BTN_TIMER: Id--;
		if (Id >= 0xff)
		{
			Id = CoreParamsMax-1;
		}
		break;
	case BTN_SHUT: MenuParam.pfmenu = MenuOnStatus;  Id=0; lcd_wr_char(_lcd5_find,0);return;
	case BTN_LOCK:	MenuParam.lockFlag ^= 1;break;//need to add lock ico
	default: break;
	}

	lcd_wr_char(_lcd4_lock, MenuParam.lockFlag);
	fOffStatusPara(Id);
}

void fRecPara(uint8_t Id)
{
	uint8_t i,j,m;
	int16_t k;
	i = Id/10;
	j = Id%10;
	k = RECWatreT[Id].Value;

	lcd_wr_num(0, 3, 1);
	lcd_wr_num(i, 4, 1);
	lcd_wr_num(j, 5, 1);

	if (k >= 0)
	{
		k = k/10;
		m = k / 100;
		i = k/10;
		j = k % 10;
		lcd_wr_num(m, 0, m);
		lcd_wr_num(i, 1, 1);
		lcd_wr_num(j, 2, 1);
	} 
	else
	{
		k = 0 - k;
		k = k/10;
		i = k/10;
		j = k % 10;
		lcd_wr_num(10, 0, 1);
		lcd_wr_num(i, 1, 1);
		lcd_wr_num(j, 2, 1);
	}
}

void DisplayRecT(void)
{
	uint16_t keyValue=0;
	static uint8_t Id=0;
//	uint8_t i,j,k;

	keyValue = KeyPop();
	switch(keyValue)
	{
	case BTN_CLOCK: Id++; 
		if (Id > 4)
		{
			Id = 0;
		}
		break;
	case BTN_TIMER: Id--;
		if (Id >= 0xff)
		{
			Id = 4;
		}
		break;
	case BTN_SHUT: MenuParam.pfmenu = MenuOnStatus;  Id=0; lcd_wr_char(_lcd5_find,0);return;
	case BTN_LOCK:	MenuParam.lockFlag ^= 1;break;//need to add lock ico
	default: break;
	}

	lcd_wr_char(_lcd4_lock, MenuParam.lockFlag);
	fRecPara(Id);
}


