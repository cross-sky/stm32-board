#include "cominc.h"

const IOControl controlCd4051[]={
	{CDa, 0, GPIO_Pin_2, GPIOE,RCC_APB2Periph_GPIOE},
	{CDb, 1, GPIO_Pin_3, GPIOE,RCC_APB2Periph_GPIOE},
	{CDc, 2, GPIO_Pin_4, GPIOE,RCC_APB2Periph_GPIOE}
};
const IOControl comminCd4051[]={
	{CDcom1, 5, GPIO_Pin_5, GPIOE,RCC_APB2Periph_GPIOE},
	{CDcom2, 6, GPIO_Pin_6, GPIOE,RCC_APB2Periph_GPIOE}
};

const CDabcRW rwCdabc[CD4051_BANKS][8]={
	{{CDin02, 0},{CDin03, 1},{CDin04, 2},{CDin01, 3},
	 {CDin05, 4},{0,CDunused},{CDin06, 6},{CDin07, 7}}, 
	{{CDin16, 0},{0,CDunused},{0,CDunused},{0,CDunused},
	 {CDin08, 4},{CDin14, 5},{CDin12, 6},{CDin15, 7} }
};

//@@@@@@@@1. need init
Event_t eventCd4051={
	STATE_ON,0,8,100
};

void vCd4051Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t name;

	//手动开CLK吧，也就几个
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	for (name=0; name<CDcomMax; name++)
	{
		//如上手动开了，就1个
		//RCC_APB2PeriphClockCmd(controlCd4051[name].clk, ENABLE);
		GPIO_InitStructure.GPIO_Pin = comminCd4051[name].pin;
		//这里设置为浮空
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_Init(comminCd4051[name].port, &GPIO_InitStructure);
	}

	for (name=0; name<CDControlMAX; name++)
	{
		//如上手动开了，就1个
		//RCC_APB2PeriphClockCmd(controlCd4051[name].clk, ENABLE);
		GPIO_InitStructure.GPIO_Pin = controlCd4051[name].pin;
		//这里设置为开漏输出，已接上拉电阻
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(controlCd4051[name].port, &GPIO_InitStructure);
		//初始时要先设置cd4051打开cdin01对应的in口
		//CDabc set 0
		IOL(&controlCd4051[name]);
	}
}


void vCd4051Detect(void)
{
	uint8_t t,i;
	uint16_t tdata;
	static uint16_t data;
	Event_t *e=&eventCd4051;

	if (e->state == STATE_OFF)
	{
		if ((e->code++) >= e->TIME)
		{
			e->state=STATE_ON;
			e->code=0;
		}
		else
			return;
	}

	if (uINPCd405xExcuted() == FALSE)
	{
		return;
	}
	//读数据端口, 2个输入口在同一个port
	//tdata=GPIO_ReadInputData(comminCd4051[0].port);
	//1.send read 2 times;
	tdata=(uint16_t)comminCd4051[0].port->IDR;
	for(i=0;i<CD4051_BANKS;i++)
	{
		if (rwCdabc[i][e->code].nhard != CDunused)
		{
			t=(tdata>>comminCd4051[i].nsoft)&0x01;//硬件位数右移
			data |= t<<rwCdabc[i][e->code].name;//软件位数左移
		}
	}

	e->code++;
	if (e->code >= e->COUNT)
	{
		//1.send data;2.clear data;3.set state_off
		//e->code=0--有用处，不能设置0
		//@@@@@@@
		vINPSendCd405xData(data);
		data=0;
		e->state=STATE_OFF;
		//e->code=0;
	}

	//1.最后一次设置cdabc=0，下次直接读取
	t=(e->code)&CD4051_MASK_INPUTS;
	for(i=0; i<CD4051_NUMBER_CONTROL; i++)
	{
		if (t&0x01)
		{
			IOH(&controlCd4051[i]);
		}
		else
			IOL(&controlCd4051[i]);

		t>>=1;
	}
	
}

