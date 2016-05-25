#ifndef __TDSOV2_H
#define __TDSOV2_H

#define ADC1_DR_Address ( (u32)0x4001244C)

#define	DMA_ADCBUFSIZE	600 	/* 采样深度，即最大样本个数 */
#define WAVBUFSIZE		300

typedef struct
{
	u16 DMA_ADCBuf[DMA_ADCBUFSIZE];	 /* 通道1数据缓冲区 */
	u16 xCh1Buf1[310];//显示波形的数组
	u16 yCh1Buf1[310];//显示波形的数组
	u16 xCh1Buf2[310];//显示波形的数组
	u16 yCh1Buf2[310];//显示波形的数组
	u8  BufUsed;

	u16	TrigLevel;//触发电平
	u8 	TrigPs;//触发位置:时间
//	u8	TrigMode;//触发模式:自动,普通,单次
	u8	TrigEdge;//触发边沿:0上升沿,1下降沿
//	u8	TrigCh;//触发通道
//	u32 TrigWait;
	u8	TrigFlag;//被触发的标志

	u8  TimeId;			/* 时基索引, 查表可得到 us单位的时基 */
	u8  SimpleNum;			/* 采样点数量 */
	float  SimpleTim;				/* 采样周期 */
	u16 SimplePeriod;			//Tim1时钟数
	u8  SimplePrescal;			//Tim1时钟分频数;
	
	u16 ADCTrigCnt;			//ADC触发位置
	u8  DMAState;   //dma采集600点完成

}T_DSO;

extern T_DSO g_DsoA;
extern const u16 Simple_Period[];
extern const u8 Simple_Prescal[];
void Dma1_Channel1Handle(void);
void InitDso(void);
void DMAReadAdc(void);
void TaskDso(void);
void UsartDmaTest(void);
#endif

