#ifndef __STM32_UB_BUTTON_H
#define __STM32_UB_BUTTON_H

typedef enum
{
	BTN_USER = 0,
	BUTTON_ANZ =1
}BUTTON_NAME_t;

typedef enum
{
	BTN_NO_KEY = 0,
	BTN_RIGHT =	0x0080,
	BTN_LEFT =	0x0100,
	BTN_EN =	0x0200,
	BTN_BACK =	0x0400,
	BTN_RIGHT_CON = 0x1080,
	BTN_LEFT_CON = 0x1100

}BUTTON_STATUS_t;

typedef struct
{
	BUTTON_NAME_t	BUTTON_NAME;
	GPIO_TypeDef*	BUTTON_PORT;
	const u16		BUTTON_PIN;
	const u32		BUTTON_CLK;
	BUTTON_STATUS_t				BUTTON_STATUS;
	u16				BUTTON_MASK;
}BUTTON_t;

typedef struct BTN_QNode{
	BUTTON_STATUS_t dat;
	struct BTN_QNode *next;
}BTN_QNode,*BTN_QueuePtr;

typedef struct{
	BTN_QueuePtr front,rear;
}BTN_LinkQueue;

typedef enum{
	OVERFLOW = 0,
	Ok    = 1
}Status;

//BUTTON_STATUS_t UB_Button_read(BUTTON_NAME_t btn_name);
extern BUTTON_t BUTTON[];
void ButtonRead(void);//按键读取
void UB_Button_Init(void);//按键初始化
extern BTN_LinkQueue *qBTNLink;;

Status BTN_ENQueue(BTN_LinkQueue *q, BUTTON_STATUS_t e);
Status BTN_DeQueue(BTN_LinkQueue *q, BUTTON_STATUS_t *e);
#endif

