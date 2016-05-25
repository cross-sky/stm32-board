#ifndef __STM32_UB_ADC_H
#define __STM32_UB_ADC_H

typedef enum{
	ADC01_AIN=0x00,	//A吸气
	ADC02_AMI,		//A蒸发温度
	ADC03_AOUT,		//A排气温度
	ADC04_WIN,		//进水温度
	ADC05_WOUT,		//出水温度
	ADC06_ENVT,		//环温度
	ADC07_WTANK,	//水箱温度
	ADC08_BIN,		//B吸气温度
	ADC09_BMI,		//B蒸发温度
	ADC10_BOUT,		//B排气温度
	ADC11_TCA,		//互感器A
	ADC12_TCB,		//互感器B
	ADC13_INTER,	//内部温度
	ADC14_humidity,	//湿度	b1
	ADC15_humiTemper,//湿度模块测量的温度 b0
	ADCMAX,			//最大值
}ADC_t;

#define ADC_CORE_MAX ADC07_WTANK	//结构体中使用，枚举变量不能放在struct中开辟数组

void vADCInit(void);
//void vADCStartSimpling(void);
void vADCDmaChannel1_ISR(void);
void vADCSimpling(void);



#endif

