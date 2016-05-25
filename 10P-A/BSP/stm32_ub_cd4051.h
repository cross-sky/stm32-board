#ifndef __STM32_UB_CD4051_H
#define __STM32_UB_CD4051_H

typedef struct  
{
	uint8_t 		name;
	uint8_t			nsoft;
	uint16_t		pin;
	GPIO_TypeDef*	port;
	uint32_t		clk;
//	uint8_t			nhard;
}IOControl;

typedef struct
{
	uint8_t name;
	uint8_t nhard;
//	uint8_t nsoft;
}CDabcRW;

typedef enum {
	CDa=0x00,
	CDb,
	CDc,
	CDControlMAX
}CDControl;

typedef enum {
	CDcom1=0x00,
	CDcom2,
	CDcomMax
}CDCommInput;

typedef enum {
	CDin01=0x00,	//��ѹA  ��Ӧ���쳣
	CDin02,			//��ѹA	��Ӧ���쳣
	CDin03,			//ˮ��
	CDin04,			//��ѹB	��Ӧ���쳣
	CDin05,			//��ѹB	��Ӧ���쳣
	CDin06,			//ˮ��
	CDin07,			//��ˮλ
	CDin08,			//��ˮλ
	CDin09,			//
	CDin10,			//
	CDin11,			//
	CDin12,			//��ˮλ
	CDin13,			//
	CDin14,			//���뿪��1
	CDin15,			//���뿪��2
	CDin16,			//���뿪��3
	CDinMax,
	CDunused
}CD4051Input;

#define CORE_CDIN CDin03//���ļ�⣬��⵽0��ʾ�Ͽ�

#define CD4051_NUMBER_CONTROL	0x03
#define CD4051_MASK_INPUTS		(0x07)
#define CD4051_BANKS			0x02

extern const CDabcRW rwCdabc[CD4051_BANKS][8];

#define IOH(ioControl_) (((*ioControl_).port)->BSRR=(*ioControl_).pin)
//#define IOH(ioControl_) ((((IOControl*)ioControl_)->port)->BSRR=((IOControl*)ioControl_)->pin)
#define IOL(ioControl_) (((*ioControl_).port)->BRR=(*ioControl_).pin)

void vCd4051Init(void);
void vCd4051Detect(void);

#endif
