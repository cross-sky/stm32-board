#ifndef __STM32_UB_ADC_H
#define __STM32_UB_ADC_H

typedef enum{
	ADC01_AIN=0x00,	//A����
	ADC02_AMI,		//A�����¶�
	ADC03_AOUT,		//A�����¶�
	ADC04_WIN,		//��ˮ�¶�
	ADC05_WOUT,		//��ˮ�¶�
	ADC06_ENVT,		//���¶�
	ADC07_WTANK,	//ˮ���¶�
	ADC08_BIN,		//B�����¶�
	ADC09_BMI,		//B�����¶�
	ADC10_BOUT,		//B�����¶�
	ADC11_TCA,		//������A
	ADC12_TCB,		//������B
	ADC13_INTER,	//�ڲ��¶�
	ADC14_humidity,	//ʪ��	b1
	ADC15_humiTemper,//ʪ��ģ��������¶� b0
	ADCMAX,			//���ֵ
}ADC_t;

#define ADC_CORE_MAX ADC07_WTANK	//�ṹ����ʹ�ã�ö�ٱ������ܷ���struct�п�������

void vADCInit(void);
//void vADCStartSimpling(void);
void vADCDmaChannel1_ISR(void);
void vADCSimpling(void);



#endif

