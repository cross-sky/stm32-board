#include "cominc.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADCx_DMA1_IRQn     DMA1_Channel1_IRQn
//@@@@@@@@1. need init

Event_t eventADC={
	STATE_ON,0,5,100
};

uint8_t adcFlag=0;

const IOControl adcInput[]={
	{ADC01_AIN,		10	, GPIO_Pin_0, GPIOC,RCC_APB2Periph_GPIOC},  //channel10
	{ADC02_AMI,		11	, GPIO_Pin_1, GPIOC,RCC_APB2Periph_GPIOC},	//channel11
	{ADC03_AOUT,	12	, GPIO_Pin_2, GPIOC,RCC_APB2Periph_GPIOC},	//channel12
	{ADC04_WIN,		13	, GPIO_Pin_3, GPIOA,RCC_APB2Periph_GPIOC},	//channel4
	{ADC05_WOUT,	2	, GPIO_Pin_2, GPIOA,RCC_APB2Periph_GPIOA},	//channel6
	{ADC06_ENVT,	3	, GPIO_Pin_3, GPIOA,RCC_APB2Periph_GPIOA},	//channel7
	{ADC07_WTANK,	4	, GPIO_Pin_4, GPIOA,RCC_APB2Periph_GPIOA},	//channel5
	{ADC08_BIN,		5	, GPIO_Pin_5, GPIOC,RCC_APB2Periph_GPIOA},	//channel13
	{ADC09_BMI,		6	, GPIO_Pin_6, GPIOA,RCC_APB2Periph_GPIOA},	//channel2
	{ADC10_BOUT,	7	, GPIO_Pin_7, GPIOA,RCC_APB2Periph_GPIOA},	//channel3
	{ADC11_TCA,		0	, GPIO_Pin_0, GPIOA,RCC_APB2Periph_GPIOA},	//channel0
	{ADC12_TCB,		1	, GPIO_Pin_1, GPIOA,RCC_APB2Periph_GPIOA},	//channel1
//	{ADC13_INTER,	12	, GPIO_Pin_14, GPIOD,RCC_APB2Periph_GPIOD}	//channel16
};

void vADCInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t name;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_ClearFlag(DMA1_FLAG_TC1);
	NVIC_InitStructure.NVIC_IRQChannel=ADCx_DMA1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=10;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//手动开CLK吧，也就2个,注意开ADC的时钟树
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1
		, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);//<14MHz
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	//这里应该-1，初始化管脚不包括内部温度引脚
	for (name=0; name<ADCMAX-1; name++)
	{
		GPIO_InitStructure.GPIO_Pin = adcInput[name].pin;
		//这里设置为ADC输入是
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(adcInput[name].port, &GPIO_InitStructure);
	}

	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uINPGetADCaddress();
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADCMAX;	//
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	//
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);


	//使能DMA传输完成中断
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC,ENABLE);

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = ADCMAX;
	ADC_Init(ADC1, &ADC_InitStructure);

	//cdmax-1 个外部adc，1个内部adc
	for(name=0;name<ADCMAX-1; name++)
	{
		//  /* ADC1 regular channel14 configuration */ 
		//ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_55Cycles5);
		//1. rank start at 1
		ADC_RegularChannelConfig(ADC1, adcInput[name].nsoft, name+1, ADC_SampleTime_239Cycles5);
	}

	//1.Internal temp start
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, ADCMAX, ADC_SampleTime_239Cycles5);

	//need to enable
	ADC_TempSensorVrefintCmd(ENABLE);
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/* Start ADC1 Software Conversion */ 
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


void vADCStartSimpling(void)
{
	//1.重新设置规则组?；2.清除转换完成；3.dma设置；4.adc软启动
	//ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	//ADC_ClearFlag(ADC1, ADC_FLAG_EOC);  in DMA_ISR
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA1_Channel1->CMAR=(uint32_t)uINPGetADCaddress();
	DMA1_Channel1->CNDTR =ADCMAX;	//这里大意了
	DMA_Cmd(DMA1_Channel1, ENABLE);
//	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void vADCDmaChannel1_ISR(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC1) != RESET)
	{
//		ADC_ClearFlag(ADC1, ADC_FLAG_STRT);
//		ADC_Cmd(ADC1, DISABLE);
		DMA_ClearITPendingBit(DMA1_IT_GL1);
		DMA_ClearITPendingBit(DMA1_IT_TC1);//清除全局中断
//		DMA_Cmd(DMA1_Channel1, DISABLE);
		ADC_SoftwareStartConvCmd(ADC1, DISABLE);
		vINPADCCompleteSampling();
		adcFlag=0;
	}
}

void vADCSimpling(void)
{
	Event_t *e=&eventADC;


	if (e->state == STATE_OFF)
	{
		if ((++e->code) >= e->TIME)
		{
			e->state=STATE_ON;
			e->code=0;
		}
		else
			return;
	}

	if (uINPADCExcuted()==FALSE)
	{
		return;
	}

	if (adcFlag != 0)
	{
		return;
	}
	adcFlag=1;
	//1.Internal-软触发
	vADCStartSimpling();

	e->code++;
	if (e->code >= e->COUNT)
	{
		//1.set state_off;
		//@@@@@@@
		e->state=STATE_OFF;
		//e->code=0;
	}

}

