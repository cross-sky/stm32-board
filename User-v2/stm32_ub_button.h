#ifndef __STM32_UB_BUTTON_H
#define __STM32_UB_BUTTON_H

typedef enum
{
	BTN_USER = 0,
	BUTTON_ANZ =1
}BUTTON_NAME_t;

typedef enum
{
	BTN_NULL = 0,
	BTN_DOWN =	0x0001,	//	k1
	BTN_TIMER =	0x0002,	//	k2
	BTN_ELEC =	0x0004,	//	k3
	BTN_SHUT =	0x0008,	//	k4
	BTN_CLOCK =	0x0010,	//	k5
	BTN_UP =	0x0020,	//	k6

	BTN_LOCK =	0x0021,	//lock
	BTN_SETCORE = 0x0012, // 1.add key 2.add key_status
	BTN_VIEWRC = 0x0030
//	BTN_RIGHT_CON = 0x1080,
//	BTN_LEFT_CON = 0x1100

}BUTTON_STATUS_t;

typedef struct 
{
	BUTTON_NAME_t	BUTTON_NAME;
	GPIO_TypeDef*	BUTTON_PORT;
	const uint16_t	BUTTON_PIN;
	const uint32_t	BUTTON_CLK;
	uint16_t		BUTTON_MASK;
}BUTTON_t;

//uint16_t KeyPush(void);//按键读取
void UB_Button_Init(void);//按键初始化


#define KeyBuffLen 8

typedef struct 
{
	uint16_t buff[KeyBuffLen];
	uint8_t in;
	uint8_t out;
}KEY_t;

void UB_Button_KeyIn(void);
void KeyTest(void);
uint16_t KeyPop(void);//按键读取
void UB_Button_Init(void);//按键初始化
uint8_t KeyPush(uint16_t tvalue);

#endif
