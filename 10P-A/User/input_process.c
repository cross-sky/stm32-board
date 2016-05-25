#include "cominc.h"

#define ADC_DATA_MIN 10		//NTC open
#define ADC_DATA_MAX 4000	// NTC short

uint8_t err;//ȫ�ִ���

InputProcess_t dataInput={
	0,
	0,
	{0},
	{{0},{0}},
	{0}
};

void vINPSendCd405xData(uint16_t data)
{
	dataInput.dataCd4051[dataInput.countCd4051]=data;
	dataInput.countCd4051++;
}

void vINPADCCompleteSampling(void)
{
	dataInput.countNtc++;
}

uint16_t* uINPGetADCaddress(void)
{
	//dataInput.countNtc++;
	//t=data[i++]; ����ȡdata[i]��ֵ��i++
	return &(dataInput.dataNtc[dataInput.countNtc][0]);
}

uint8_t uINPGetCd405xCount(void)
{
	return dataInput.countCd4051;
}

uint8_t uINPCd405xExcuted(void)
{
	if (dataInput.countCd4051>=INPUT_COUNT_MAX)
	{
		return FALSE;
	}
	else
		return TRUE;
}

uint8_t uINPADCExcuted(void)
{
	if (dataInput.countNtc >= INPUT_COUNT_MAX)
	{
		return FALSE;
	}
	else
		return TRUE;
}

uint8_t uInProcessGetNtcCount(void)
{
	return dataInput.countNtc;
}

/************************************************************************/
/* �����ߵ�ѹ��ˮ������                                                                     */
/************************************************************************/
void vINPgetCoreInputxErr(void)
{
	uint8_t i;
	
	uint16_t t=dataInput.finaCD4051;
	for (i=0;i<CORE_CDIN;i++)
	{
		//��������ʱ�Ǳպϵģ������ǵ�ͨ״̬����⵽�ߵ�ƽ
		//�͵�ƽ�쳣,
		if (t&0x01 == 0)
		{
			//err=rwCdabc[0][i].name;
			//@@@@@@@@@���з��ʹ���
			//xQUESigPush(SIG_ERR);
			err=1;
			return;
		}
		t>>=1;
	}
	//@@@@@@@@@@@���з����޴���
	//xQUESigPush(SIG_NOERR);
	//return 0;
}

void vINPCd4051Process(void)
{
	uint8_t i,j,k;
	uint8_t count=0;
	uint16_t data=0;
	if (dataInput.countCd4051<INPUT_COUNT_MAX)
	{
		//
		return;
	}
	else
	{
		for (i=0; i<2; i++)
		{
			for (j=0;j<8;j++)
			{
				//0-7cd4051A�� 8-15CD4051B
				if (rwCdabc[i][j].nhard!=CDunused)
				{
					for (k=0;k<INPUT_COUNT_MAX;k++)
					{
						count +=(dataInput.dataCd4051[k]>>(j+i<<3))&0x01;
					}

					if (count >= INPUT_COUNT_MAX-1)
					{
						//1-����
						count=1;
					}else{
						count=0;
					}
					data|=count<<i;

					count=0;
				}
			}
		}

		//@@@@@@@@@ 1.save data;2.set dataInput.countCd4051=0
		dataInput.finaCD4051=data;
		dataInput.countCd4051=0;

		vINPgetCoreInputxErr();
		vQUESetCD4051DectState(data);
	}

}

void vINPGetAdcErr(void)
{
	uint8_t i;
	uint16_t data;
	uint16_t adcdect;
	for(i=0;i<ADC_CORE_MAX;i++)
	{
		data=dataInput.finaNtc[i];
		if (data<ADC_DATA_MIN || data>ADC_DATA_MAX)
		{
			//@@@@@
			//1.send error;2.return
			adcdect |= 0x01<<i;
			err=1;
			break;
		}
	}
	
	//@@@@
	//
	vQUESetADCDectState(adcdect);

	//if (adcdect != 0)
	//{
	//	xQUESigPush(SIG_ERR);
	//}
	//else
	//	xQUESigPush(SIG_NOERR);
}

void vInpConvertRealNtc(void)
{
	uint16_t searchValue,index;
	float temp;
	uint8_t i;
	//���3������NTC������Ҫ����ת��
	for(i=0;i<10; i++)
	{
		searchValue=dataInput.finaNtc[i];
		if (i==ADC03_AOUT)
		{
			index=uADCSearchData50K(searchValue);
			dataInput.realNtc[i]=iADCTemperCalc50K(index,searchValue);
		}
		else
		{
			index=uADCSearchData10K(searchValue);
			dataInput.realNtc[i]=iADCTemperCalc10K(index,searchValue);
		}
				
	}
	dataInput.realNtc[ADC11_TCA]=dataInput.finaNtc[ADC11_TCA]*0.0061;

	temp=(float)dataInput.finaNtc[ADC13_INTER]*0.00080;
	temp = (1.43-temp)/0.0043+25;
	dataInput.realNtc[ADC13_INTER]=temp*10;

	//adc14-15
	dataInput.realNtc[ADC14_humidity]=0.24414*dataInput.finaNtc[ADC14_humidity];
	searchValue=dataInput.finaNtc[ADC15_humiTemper];
	index=uADCSearchData50K(searchValue);
	dataInput.realNtc[ADC15_humiTemper]=iADCTemperCalc10K(index,index);

}

void vINPAdcProcess(void)
{
	uint8_t i,j;
	uint16_t data=0;
	if (dataInput.countNtc<INPUT_COUNT_MAX)
	{
		return;
	}
	else{
		for (j=0;j<ADCMAX;j++)
		{
			for (i=0; i<INPUT_COUNT_MAX; i++)
			{
				data += dataInput.dataNtc[i][j];
			}
			dataInput.finaNtc[j] = data/INPUT_COUNT_MAX;
			data = 0;
		}
	}

	//@@@@@@@ ���Ҷ�Ӧ���¶�ֵ

	vINPGetAdcErr();//1.���Ҵ���

	dataInput.countNtc=0;
	vInpConvertRealNtc();//2.ת��ntc
	vInpSetCoreParamADC(&dataInput.realNtc[0]);//3.ת�������õ��ڴ����

	//@@@@@@@@@@@�����¶�>110 Ҫ���ʹ��󣬴�д

	//��ʱ�������ڵ���
	//if (err==1)
	//{
	//	xQUESigPush(SIG_ERR);
	//}
	//else
	//	xQUESigPush(SIG_NOERR);

}

void Task1RegularSimpling(void)
{
	vCd4051Detect();
	vADCSimpling();
}

void Task2InputProcess(void)
{
	err=0;
	vINPCd4051Process();
	vINPAdcProcess();
	vUartRxPopProcess();

	//if (err==1)
	//{
	//	xQUESigPush(SIG_ERR);
	//}
	//else
	//	xQUESigPush(SIG_NOERR);
}

