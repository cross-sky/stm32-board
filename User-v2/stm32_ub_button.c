#include "cominc.h"

BUTTON_t BUTTON[]={
	// Name    ,PORT , PIN																, CLOCK             ,0000 0111 1000 0000
	{BTN_USER, GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5, RCC_APB2Periph_GPIOA,  0x003f}
};
//beep and background-led init at here

KEY_t key;

void KeyBuffInit(void)
{
	key.in=0;
	key.out=0;
}

uint16_t KeyPop(void)
{
	uint16_t tValue;

	if (key.out != key.in)
	{
		key.out++;
		if (key.out >= KeyBuffLen)
		{
			key.out = 0;
		}
		tValue = key.buff[key.out];
	}
	else
	{
		tValue = BTN_NULL;
	}
	return tValue;
}

uint8_t KeyPush(uint16_t tvalue)
{
	if ((key.in+1)%KeyBuffLen == key.out)
	{
#ifdef Debug
		//printf("out of key buff \r\n");
#endif
		return FALSE;
	}

	if ( tvalue != BTN_NULL)
	{
		key.in++;
		if (key.in >= KeyBuffLen)
		{
			key.in = 0;
		}
		key.buff[key.in]=tvalue;

		//MenuParam.beepFlag = 1;
		//MenuParam.bgledFlag = 1;//light and beep

#ifdef Debug
		//printf("kin %x \r\n", tvalue);
#endif
		return TRUE;		
	}
	else
		return FALSE;
}


void UB_Button_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t	 btn_name;

	for(btn_name=0; btn_name<BUTTON_ANZ; btn_name++)
	{
		RCC_APB2PeriphClockCmd(BUTTON[btn_name].BUTTON_CLK,ENABLE);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = BUTTON[btn_name].BUTTON_PIN;
		GPIO_Init(BUTTON[btn_name].BUTTON_PORT, &GPIO_InitStructure);

	}
	KeyBuffInit();
}

void UB_Button_KeyIn(void)
{
	uint8_t btn_name=0;
	uint16_t value;
	static uint16_t Trg,Cont,cnt_last;
	uint16_t key_status;
	static uint16_t cnt_puls;

	cnt_last = Cont;
	value = (GPIO_ReadInputData(BUTTON[btn_name].BUTTON_PORT)&BUTTON[btn_name].BUTTON_MASK)
		^BUTTON[btn_name].BUTTON_MASK;
	Trg = value & (value ^ Cont);
	Cont = value;
	if(Cont ==cnt_last  ) 
		cnt_puls++;
	else
		cnt_puls = 0;

	if (cnt_puls >25 && Cont!= 0 &&Trg == 0 )
	{
		cnt_puls = 0;
		key_status = Cont & BUTTON[btn_name].BUTTON_MASK;
		switch(key_status)
		{
			// case BTN_RIGHT: return BTN_RIGHT_CON;//break;
			// case BTN_LEFT:	return BTN_LEFT_CON;//break;
		}
		return ;
	}

	key_status = Trg & BUTTON[btn_name].BUTTON_MASK;

	if (key_status)
	{
		//put down any keys, then light and beep
		MenuParam.beepFlag = 1;
		MenuParam.bgledFlag = 1;//light and beep

		switch(key_status)
		{
		case BTN_DOWN:	key_status=BTN_DOWN;	break;
		case BTN_TIMER:	key_status=BTN_TIMER;	break;
		case BTN_ELEC:	key_status=BTN_ELEC;	break;
		case BTN_SHUT:	key_status=BTN_SHUT;	break;
		case BTN_CLOCK:	key_status=BTN_CLOCK;	break;
		case BTN_UP:	key_status=BTN_UP;	break;
		case BTN_LOCK:	key_status=BTN_LOCK;	break;
		case BTN_SETCORE:key_status=BTN_SETCORE;break;
		case BTN_VIEWRC: key_status = BTN_VIEWRC; break;
		default:	key_status=BTN_NULL;	break;
		}

		if (MenuParam.lockFlag==1 && key_status != BTN_LOCK)
		{
			key_status = BTN_NULL;
			MenuParam.bgledFlag = 1;//light background-led
		}

		KeyPush(key_status);
	}
}


void KeyTest(void)
{
	uint8_t i;
	uint16_t tValue;
	UB_Button_KeyIn();

	if ((key.in+1)%KeyBuffLen == key.out)
	{
		for (i=0; i<=10; i++)
		{
			tValue = KeyPop();
			if ( tValue != BTN_NULL )
			{
				printf("key pop value %x \r\n", tValue);
			}
			else
				printf("key pop Null value \r\n");
		}
	}
}
