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
	CDin01=0x00,	//高压A  对应的异常
	CDin02,			//低压A	对应的异常
	CDin03,			//水流
	CDin04,			//高压B	对应的异常
	CDin05,			//低压B	对应的异常
	CDin06,			//水泵
	CDin07,			//低水位
	CDin08,			//中水位
	CDin09,			//
	CDin10,			//
	CDin11,			//
	CDin12,			//高水位
	CDin13,			//
	CDin14,			//拨码开关1
	CDin15,			//拨码开关2
	CDin16,			//拨码开关3
	CDinMax,
	CDunused
}CD4051Input;

#define CORE_CDIN CDin03//核心检测，检测到0表示断开

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

