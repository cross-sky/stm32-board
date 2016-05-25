#include "cominc.h"

BUTTON_t BUTTON[]={
  // Name    ,PORT , PIN                                            , CLOCK             , Status   ,0000 0111 1000 0000
	{BTN_USER, GPIOB, GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10, RCC_APB2Periph_GPIOB, BTN_NO_KEY, 0x0780},
};

#define BTN_MASK 0x0780 //

//入队
Status BTN_ENQueue(BTN_LinkQueue *q, BUTTON_STATUS_t e)
{
	BTN_QueuePtr s= (BTN_QueuePtr)malloc(sizeof(BTN_QNode));
	if(!s)
		return OVERFLOW;
	s->dat = e;
	s->next = NULL;
	q->rear->next = s;  //入队
	q->rear = s;		//修改队尾
	return Ok;
}

Status BTN_DeQueue(BTN_LinkQueue *q, BUTTON_STATUS_t *e)
{
	BTN_QueuePtr p;
	if(q->front == q->rear)
		return OVERFLOW;
	p = q->front->next;
	*e = p->dat;
	q->front->next = p->next;
	if(q->rear == p)
		q->rear = q->front;
	free(p);
	return Ok;
}

BTN_LinkQueue *qBTNLink;

BTN_LinkQueue* BTN_LinkQueueInit(void)
{
	BTN_LinkQueue *q;
	BTN_QueuePtr p;
	q = (BTN_LinkQueue *)malloc(sizeof(BTN_LinkQueue));
	p = (BTN_QueuePtr)malloc(sizeof(BTN_QNode));
	p->next = NULL;
	q->front = q->rear =p;
	return q;

}


void UB_Button_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	u8	 btn_name;

	for(btn_name=0; btn_name<BUTTON_ANZ; btn_name++)
	{
		RCC_APB2PeriphClockCmd(BUTTON[btn_name].BUTTON_CLK,ENABLE);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = BUTTON[btn_name].BUTTON_PIN;
		GPIO_Init(BUTTON[btn_name].BUTTON_PORT, &GPIO_InitStructure);

	}
	
	qBTNLink = BTN_LinkQueueInit();
}

/*
1.没有按键的时候，
		ReadData = PINB^0xff=0xff^0xff=0x00
		Trg = ReadData & (ReadData ^ Cont)=0x00&(0x00^0x00)=0x00
		Cont = ReadData=0x00
2.第一次PB0按下的情况
		ReadData = PINB^0xff=0xfe^0xff=0x01
		Trg = ReadData & (ReadData ^ Cont)=0x01&(0x01^0x00)=0x01
		Cont = ReadData=0x01
3. PB0按着不松（长按键）的情况
		ReadData = PINB^0xff=0xfe^0xff=0x01
		Trg = ReadData & (ReadData ^ Cont)=0x01&(0x01^0x01)=0x00
		Cont = ReadData=0x01
4.按键松开的情况
		ReadData = PINB^0xff=0xff^0xff=0x00
		Trg = ReadData & (ReadData ^ Cont)=0x00&(0x00^0x01)=0x00
		Cont = ReadData=0x00  //


*/
BUTTON_STATUS_t UB_Button_read(BUTTON_NAME_t btn_name)
{
	u16 value;
	static u16 Trg,Cont,cnt_last;
	static u16 key_status;
	static u8 cnt_puls;
	//value = GPIO_ReadInputDataBit(BUTTON[btn_name].BUTTON_PORT,BUTTON[btn_name].BUTTON_PIN);
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
// 			case BTN_RIGHT: return BTN_RIGHT_CON;//break;
// 			case BTN_LEFT:	return BTN_LEFT_CON;//break;
// 			default:		return BTN_NO_KEY;//break;
			case BTN_RIGHT: BTN_ENQueue(qBTNLink,BTN_RIGHT);break;
			case BTN_LEFT:	BTN_ENQueue(qBTNLink,BTN_LEFT);break;
			default:		break;					
		}
		return BTN_NO_KEY;
	}

	key_status = Trg & BUTTON[btn_name].BUTTON_MASK;
	if (key_status)
	{
		switch(key_status)
		{
			case BTN_RIGHT:	BTN_ENQueue(qBTNLink,BTN_RIGHT);break;
			case BTN_LEFT:	BTN_ENQueue(qBTNLink,BTN_LEFT);break;
			case BTN_EN:	  BTN_ENQueue(qBTNLink,BTN_EN);break;
			case BTN_BACK:	BTN_ENQueue(qBTNLink,BTN_BACK);break;
			default:		break;
		}
	}
	return BTN_NO_KEY;

}

void ButtonRead(void)
{
	BUTTON[0].BUTTON_STATUS = UB_Button_read(BTN_USER);
}

