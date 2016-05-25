#ifndef __EVENT_H
#define __EVENT_H

typedef struct{
	uint16_t state;		//状态
	uint16_t code;		//执行的次数-变量
	uint16_t COUNT;		//需要连续执行多少次，5ms/次间隔
	uint16_t TIME;		//隔多长时间开始
}Event_t;

typedef enum{
	STATE_OFF=0x00,
	STATE_ON,
	STATE_INIT,
	STATE_ERR
}State_t;


#endif

