#include "cominc.h"

//const u8* hs_stab[]={"50us",	"100us","200us","1ms",	"2ms",	"10ms","20ms","100ms"};
const u8  Simple_Num[]={20,		40,		40,		100,	100,	100,	100,	100};
const float  Simple_Tim[]={2.5,	2.5,	5,		10,		20,		100,	200,	1000};
const u16 Simple_Period[]={179,	179,	359,	719,	1339,	7199,	7199,	7199};
const u8  Simple_Prescal[]={0,		0,		0,		0,		0,		0,		1,		9};

T_DSO g_DsoA;

//DMA数据，寻找触发点
//若没找到，重新开dma，T2定时器 cc2触发
//20ms循环1次
void DMAReadAdc(void)
{
	u16 i;
	if(g_DsoA.DMAState == 1)
	{
		for (i=0; i<300; i++)
		{
			if(g_DsoA.TrigEdge == 0)
			{
				if ((g_DsoA.DMA_ADCBuf[i] < g_DsoA.TrigLevel)
					&&(g_DsoA.DMA_ADCBuf[i+1] >= g_DsoA.TrigLevel))
				{
					g_DsoA.ADCTrigCnt = i;
					g_DsoA.TrigFlag = 1;
					break;
				}
			}
			else
			{
				if ((g_DsoA.DMA_ADCBuf[i] > g_DsoA.TrigLevel)
					&&(g_DsoA.DMA_ADCBuf[i+1] <= g_DsoA.TrigLevel))
				{
					g_DsoA.ADCTrigCnt = i;
					g_DsoA.TrigFlag = 1;
					break;
				}

			}

		}
		g_DsoA.DMAState = 0;
		
		if(i >= 300 )
		{
			g_DsoA.TrigFlag = 0;
					
			DMA_Cmd(DMA1_Channel1,DISABLE);
			DMA1_Channel1->CNDTR = DMA_ADCBUFSIZE;
			DMA1_Channel1->CMAR = (uint32_t)&g_DsoA.DMA_ADCBuf;
			DMA_Cmd(DMA1_Channel1,ENABLE);
			TIM_Cmd(TIM2, ENABLE);
		}


	}

}

/*
*********************************************************************************************************
*	函 数 名: SetSampRate
*	功能说明: 修改采样频率
*	形    参：freq : 采样频率 单位Hz
*	返 回 值: 无
*********************************************************************************************************
*/
static void Set_SampRate(T_DSO *g_DsoA)
{
	/* 第4步： 配置定时器2第2通道CC2, CC2作为ADC1的触发源 */
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef			TIM_OCInitStructure;
//	uint16_t Period;

	TIM_Cmd(TIM2, DISABLE);

	/*
	采样频率计算公式 ：
		period = 72000000 / freq ;
		
		1200 == 60KHz 采样频率
	*/
//	Period = 72000 / freq -1;		

	/* 使能 TIM1 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
	TIM_TimeBaseStructure.TIM_Period = g_DsoA->SimplePeriod;          
	TIM_TimeBaseStructure.TIM_Prescaler = g_DsoA->SimplePrescal;   /* 计数频率 = 72000 000 / 18 = 4000 000	 */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* TIM1 通道1配置为PWM模式 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
	TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period / 2; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	/* TIM1 计数器使能 */
	TIM_Cmd(TIM2, DISABLE);

	/* TIM1 输出使能 */
	TIM_CtrlPWMOutputs(TIM2, ENABLE);  
}

static void Init_DSO(T_DSO *g_DsoA)
{
	/* 配置GPIO.  */
	
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef	ADC_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;

	/* 打开GPIO_C 和 AFIO 的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC	| RCC_APB2Periph_AFIO, ENABLE);

	/* 配置PC0为模拟输入模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* 使能 DMA1 时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Channel1);		/* 复位DMA1寄存器到缺省状态 */
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	/* 选择ADC1的数据寄存器作为源 */
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_DsoA->DMA_ADCBuf;	/* 目标地址 */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	/* 设置DMA传输方向，外设(ADC)作为源 */
	DMA_InitStructure.DMA_BufferSize = DMA_ADCBUFSIZE;	/* 设置缓冲区大小 */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	/* 外设地址不自增 */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	/* 存储器地址需要自增 */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	/* 选择外设传输单位：16bit */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	/* 选择内存传输单位：16bit */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	/* 无需循环模式   */
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	/* 选择DMA优先级 */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	/* DMA传输类型，不是内存到内存 */
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TE,ENABLE);
	/* 使能 DMA1 通道1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/*Enable DMA Channel5 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	/* 配置ADC1  */
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		/* 连续转换静止 */
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;	/* 选择TIM1的CC3做触发 */

	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	/* 数据右对齐,高位为0 */
	ADC_InitStructure.ADC_NbrOfChannel = 1;	/* 1个通道 */
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 规则通道配置 */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_1Cycles5);

	/* 使能 ADC1 外部触发转换 */
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);

	/* 使能 ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* 使能 ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* 使能 ADC1 复位校准寄存器 */
	ADC_ResetCalibration(ADC1);
	/* 检测复位校准寄存器 */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* 开始 ADC1 校准 */
	ADC_StartCalibration(ADC1);
	/* 等待校准结束 */
	while(ADC_GetCalibrationStatus(ADC1));

	Set_SampRate(g_DsoA);
}



//dma采集600点后关T1定时器
void Dma1_Channel1Handle(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC1);
	//	DMA_ClearFlag(DMA1_FLAG_TC4);
	DMA_ClearITPendingBit(DMA1_IT_TE1);
	g_DsoA.DMAState=1;//DMA传输完成
	DMA_Cmd(DMA1_Channel1,DISABLE);
	TIM_Cmd(TIM2, DISABLE);

}

/*
*********************************************************************************************************
*	函 数 名: InitDsoParam
*	功能说明: 初始化全局参数变量
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void DsoInitParam(void)
{
	
	g_DsoA.BufUsed = 0;

	g_DsoA.TrigLevel = 2048;//触发电平
	g_DsoA.TrigPs = 0;//触发位置:时间
	g_DsoA.TrigEdge = 1;//触发边沿:0上升沿,1下降沿
	g_DsoA.TrigFlag = 0;//被触发的标志

	g_DsoA.TimeId = 3;			/* 时基索引, 查表可得到 us单位的时基 */
	g_DsoA.SimpleNum = Simple_Num[g_DsoA.TimeId];			/* 采样点数量 */
	g_DsoA.SimpleTim = Simple_Tim[g_DsoA.TimeId];				/* 采样周期 */
	g_DsoA.SimplePeriod = Simple_Period[g_DsoA.TimeId];			//Tim1时钟数
	g_DsoA.SimplePrescal = Simple_Prescal[g_DsoA.TimeId];			//Tim1时钟分频数

	g_DsoA.ADCTrigCnt=0;//ADC触发位置
	g_DsoA.DMAState=0;   //dma采集600点完成

}


/*
*********************************************************************************************************
*	函 数 名: FrameDisp
*	功能说明: 显示波形窗口的边框和刻度线
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void FrameDisp(void)
{
	uint16_t x, y;

	/* 绘制一个实线矩形框 x, y, h, w */
	LCD_DrawRect(9, 19, 202, 302, CL_WHITE);

	/* 绘制垂直刻度点 */
	for (x = 0; x < 13; x++)
	{
		for (y = 0; y < 41; y++)
		{
			Gui_DrawPoint(10 + (x * 25), 20 + (y * 5), CL_WHITE);
		}
	}

	/* 绘制水平刻度点 */
	for (y = 0; y < 9; y++)
	{
		for (x = 0; x < 61; x++)
		{
			Gui_DrawPoint(10 + (x * 5), 20 + (y * 25), CL_WHITE);
		}
	}

	/* 绘制垂直中心刻度点 */
	for (y = 0; y < 41; y++)
	{	 
		Gui_DrawPoint(9 + (6 * 25), 20 + (y * 5), CL_WHITE);
		Gui_DrawPoint(11 + (6 * 25), 20 + (y * 5), CL_WHITE);
	}

	/* 绘制水平中心刻度点 */
	for (x = 0; x < 61; x++)
	{	 
		Gui_DrawPoint(10 + (x * 5), 19 + (4 * 25), CL_WHITE);
		Gui_DrawPoint(10 + (x * 5), 21 + (4 * 25), CL_WHITE);
	}
}

/*
*********************************************************************************************************
*	函 数 名: DispChInfo
*	功能说明: 显示通道信息
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ChInfoDisp(void)
{
	char buf[32];   	/* 字符显示缓冲区 */
	FONT_T tFont;		/* 定义一个字体结构体变量，用于设置字体参数 */
//	float tTem;
	/* 设置字体属性 */
	tFont.usFontCode = FC_ST_16X16;		/* 字体选择宋体16点阵，高16x宽15) */
	tFont.usTextColor = CL_WHITE;		/* 字体颜色设置为白色 */
	tFont.usBackColor = CL_BLUE;	 	/* 文字背景颜色，蓝色 */
	tFont.usSpace = 0;					/* 字符水平间距, 单位 = 像素 */

	/* 显示示波器程序版本 */
	Gui_DrawFont_GBK16(10, 2, "v.0", &tFont);


	strcpy(buf, "CH1 DC");
	sprintf(&buf[7], "3.6V");
	tFont.usTextColor = CL_YELLOW;		/* 设置字体颜色，黄色 */
	tFont.usBackColor = CL_MAGENTA;	 	/* 文字背景颜色，紫色 */
	Gui_DrawFont_GBK16(10, 224, buf, &tFont);
	
	sprintf(buf, "%.2f",tTemp[3]);
  Gui_DrawFont_GBK16(100, 224, buf, &tFont);
	/* 显示时基 */
//	tFont.usTextColor = CL_WHITE;		/* 设置字体颜色，白色 */
//	tFont.usBackColor = CL_BLUE;	 	/* 文字背景颜色，蓝色 */

	/* 显示每格多长时间，即时间分度 */
//	sprintf(buf, "t:%.1fus",	Simple_Tim[g_DsoA.SimpleNum]);
//	Gui_DrawFont_GBK16(230, 224, buf, &tFont);

	/* 显示采样频率 */
	//sprintf(buf, "f:%7dHz",	Simple_Tim[g_DsoA->SimpleNum]);
	//Gui_DrawFont_GBK16(75, 2, buf, &tFont);
}

/*
*********************************************************************************************************
*	函 数 名: DispCh1Wave
*	功能说明: 显示通道1波形
*	形    参：无
*	返 回 值: 无
最小值在上方，输出的波要反转
屏幕如下
0,0----------------- 320
|
|
|
|
240
*********************************************************************************************************
*/
static void Ch1WaveDisp(void)
{
	uint16_t i=0,j=0;		
	uint16_t *px,*pxcl;
	uint16_t *py,*pycl;
	int16_t iTemp;

	if (g_DsoA.BufUsed == 0)
	{
		g_DsoA.BufUsed = 1;
	}
	else
	{
		g_DsoA.BufUsed = 0;
	}

	if (g_DsoA.BufUsed == 0)
	{
		px = g_DsoA.xCh1Buf1;
		py = g_DsoA.yCh1Buf1;

		pxcl = g_DsoA.xCh1Buf2;
		pycl = g_DsoA.yCh1Buf2;
	}
	else
	{
		px = g_DsoA.xCh1Buf2;
		py = g_DsoA.yCh1Buf2;

		pxcl = g_DsoA.xCh1Buf1;
		pycl = g_DsoA.yCh1Buf1;
	}

	/* CH1通道, 输入0V时，ADC采样值 = 1872 ,这是根据多个开发板求到的平均值 */
	j = g_DsoA.ADCTrigCnt;
	for (i=0; i < 300; i++)
	{
		px[i] = 10 + i;
		iTemp = ((u32)g_DsoA.DMA_ADCBuf[j]*100)>>12;

		if (iTemp > 100)
		{
			iTemp = 100;
		}

		py[i] = 220 - iTemp;
		j++;
	}

	/* 下面的代码采取清除上帧波形，然后再重新描绘新波形，避免整屏刷新的闪烁感 */
	/* 清除上帧波形 */
// 	if (g_DsoA.BufUsed == 0)
// 	{
// 		LCD_DrawPoints(g_DsoA.xCh1Buf2, g_DsoA.yCh1Buf2, 300, CL_BLUE);
// 	}
// 	else
// 	{
// 		LCD_DrawPoints(g_DsoA.xCh1Buf1, g_DsoA.yCh1Buf1, 300, CL_BLUE);
// 	}
// 
// 	/* 显示更新的波形 */
	FrameDisp();
	ChInfoDisp();
// 	LCD_DrawPoints((uint16_t *)px, (uint16_t *)py, 300, CL_YELLOW);
	//	DMA_SetCurrDataCounter(DMA1_Channel4,10);
	//		DMA1_Channel4->CNDTR = 10;
	//	DMA1_Channel4->CMAR = (uint32_t)&px[0];
	//	DMA_Cmd(DMA1_Channel4,ENABLE);
	
	for (i=0; i<199; i++)
	{
		Gui_DrawLine(pxcl[i], pycl[i], pxcl[i + 1], pycl[i + 1], CL_BLUE);
		Gui_DrawLine(px[i],	  py[i],   px[i + 1],   py[i + 1],   CL_YELLOW);
	}
}


//触发点找到之后显示波形
//先画帧框图，再用背景色画上一次的波形，最后更新新的波形

void TaskDso(void)
{
	if (g_DsoA.TrigFlag == 1)
	{
		Ch1WaveDisp();
		g_DsoA.TrigFlag = 0;
		
		DMA_Cmd(DMA1_Channel1,DISABLE);
		DMA1_Channel1->CNDTR = DMA_ADCBUFSIZE;
		DMA1_Channel1->CMAR = (uint32_t)&g_DsoA.DMA_ADCBuf;
		DMA_Cmd(DMA1_Channel1,ENABLE);
		TIM_Cmd(TIM2, ENABLE);
	}


}

void InitDso(void)
{
	Lcd_Clear(CL_BLUE);
	DsoInitParam();
	Init_DSO(&g_DsoA);
//	FrameDisp();
//	ChInfoDisp();
}




