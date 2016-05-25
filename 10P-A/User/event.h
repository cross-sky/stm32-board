#ifndef __EVENT_H
#define __EVENT_H

typedef struct{
	uint16_t state;		//״̬
	uint16_t code;		//ִ�еĴ���-����
	uint16_t COUNT;		//��Ҫ����ִ�ж��ٴΣ�5ms/�μ��
	uint16_t TIME;		//���೤ʱ�俪ʼ
}Event_t;

typedef enum{
	STATE_OFF=0x00,
	STATE_ON,
	STATE_INIT,
	STATE_ERR
}State_t;


#endif

