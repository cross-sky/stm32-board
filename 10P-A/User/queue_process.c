#include "cominc.h"

QueueProcess_t queueProcess={
	0,0
};

//1.init @@@@@@@@
CoreProcess_t coreProcess;

//����/�˳�״̬ �����������0
uint32_t timeFlag=0;

//!!!!!!!ÿ��case ���������һ��break/return ����ᴦ�������������������

#define Time0s	0
#define Time5s	20*5
#define Time20s 20*20
#define Time60s	20*60
#define Time180s	20*180
#define Time360S	20*360

uint8_t xQUESigPop(void)
{
	QueueSig_t sig;

	if (queueProcess.out != queueProcess.in)
	{
		queueProcess.out++;
		if (queueProcess.out>=QueueBuffLen)
		{
			queueProcess.out=0;
		}
		//sig.state=queueProcess.buff[queueProcess.out].state;
		sig.sig=queueProcess.buff[queueProcess.out].sig;
	}
	else
	{
		sig.sig=SIG_NULL;
	}
	return sig.sig;
}

uint8_t xQUESigPush(uint8_t sig)
{
	if ((queueProcess.in+1)&QueueBuffLenMask == queueProcess.out)
	{
		return FALSE;
	}

	if (sig != SIG_NULL)
	{
		queueProcess.in++;
		if (queueProcess.in >= QueueBuffLen)
		{
			queueProcess.in=0;
		}
		queueProcess.buff[queueProcess.in].sig=sig;
		return TRUE;
	}
	return FALSE;
}

/************************************************************************/
/* ���ݻ������ý��������¶�ֵ                                                                     */
/************************************************************************/
void vQUEDefrostInTemper(void)
{
	int16_t tdata;
	if (coreProcess.coreParems.environT > -1)
	{
		tdata=-6*10 ;
	}
	else
	{
		tdata=coreProcess.coreParems.environT*0.77-54.22;
	}

	coreProcess.coreParems.setDefrostInTemper=tdata-coreProcess.coreParems.setDefrostInOffset;

}

void vInpSetCoreParamADC(int16_t *realNtc)
{
	coreProcess.coreParems.machineA.inTemper=*(realNtc+ADC01_AIN);
	coreProcess.coreParems.machineA.evaporateTemper=*(realNtc+ADC02_AMI);
	coreProcess.coreParems.machineA.outTemper=*(realNtc+ADC03_AOUT);
	coreProcess.coreParems.machineA.current=*(realNtc+ADC11_TCA);
	coreProcess.coreParems.waterIn=*(realNtc+ADC04_WIN);
	coreProcess.coreParems.waterOut=*(realNtc+ADC05_WOUT);
	coreProcess.coreParems.environT=*(realNtc+ADC06_ENVT);
	coreProcess.coreParems.waterBank=*(realNtc+ADC07_WTANK);
	coreProcess.coreParems.innerTemper=*(realNtc+ADC13_INTER);

	vQUEDefrostInTemper();
}

void vQUEGetTemperParams(uint8_t *dst)
{
	//�� �� �� ˮ�� �� �� �� �������� adc���� cd4051���� �ڲ��¶�  (��λ��ǰ)

	*dst++=coreProcess.coreParems.machineA.inTemper>>8;
	*dst++=coreProcess.coreParems.machineA.inTemper&0xff;
	*dst++=coreProcess.coreParems.machineA.evaporateTemper>>8;
	*dst++=coreProcess.coreParems.machineA.evaporateTemper&0xff;
	*dst++=coreProcess.coreParems.machineA.outTemper>>8;
	*dst++=coreProcess.coreParems.machineA.outTemper&0xff;


	*dst++=coreProcess.coreParems.waterIn>>8;
	*dst++=coreProcess.coreParems.waterIn&0xff;
	*dst++=coreProcess.coreParems.waterOut>>8;
	*dst++=coreProcess.coreParems.waterOut&0xff;
	*dst++=coreProcess.coreParems.environT>>8;
	*dst++=coreProcess.coreParems.environT&0xff;
	*dst++=coreProcess.coreParems.waterBank>>8;
	*dst++=coreProcess.coreParems.waterBank&0xff;	

	*dst++=0;//ռλ B�� �� ��
	*dst++=0;
	*dst++=0;
	*dst++=0;
	*dst++=0;
	*dst++=0;

	*dst++=coreProcess.coreParems.machineA.current>>8;
	*dst++=coreProcess.coreParems.machineA.current&0xff;

	*dst++=0;
	*dst++=0;

	*dst++=coreProcess.coreParems.innerTemper>>8;
	*dst++=coreProcess.coreParems.innerTemper&0xff;


	*dst++=coreProcess.coreParems.adcDectectState>>8;
	*dst++=coreProcess.coreParems.adcDectectState&0xff;
	*dst++=coreProcess.coreParems.cd4051DectState>>8;
	*dst++=coreProcess.coreParems.cd4051DectState&0xff;

	//����A��״̬
	*dst++=coreProcess.coreParems.machineA.state;

	//return len;
}

void vQueSetCoreParams(int8_t *src)
{
	coreProcess.coreParems.SetWaterTSet= ((int8_t)*(src)) * 10;
	coreProcess.coreParems.setWaterCycleTemper=((int8_t)*(src+1))* 10;
	coreProcess.coreParems.setDefrostInTemper=((int8_t)*(src+2))* 10;
	coreProcess.coreParems.setDefrostRunTimes=((int8_t)*(src+3))*60*20;

	coreProcess.coreParems.setDefrostOutTemper=((int8_t)*(src+4))*10;
	coreProcess.coreParems.setDefrostCycleTimes=((int8_t)*(src+5))*30*60*20;
	coreProcess.coreParems.setDefrostInOffset=((int8_t)*(src+6))*10;
}

void vQUESetADCDectState(uint16_t adcstate)
{
	coreProcess.coreParems.adcDectectState = adcstate;
}

void vQUESetCD4051DectState(uint16_t cd4state)
{
	coreProcess.coreParems.cd4051DectState = cd4state;
}

void vQUESetCurrentParams(int16_t *real)
{
	coreProcess.coreParems.machineA.current=*(real+ADC11_TCA);
	coreProcess.coreParems.innerTemper=*(real+ADC13_INTER);
}

void vqueFunToNewState(uint8_t newstate)
{
	coreProcess.funState=newstate;
}

/************************************************************************/
/* �����¼�����ʱת�룬                                                                     */
/************************************************************************/
uint8_t vqueFunErr(void)
{
	switch(coreProcess.funState)
	{
	case FUN_STATE_INIT:
		{
			//@@@@ֹͣ�̵���������������ͷ���0��
			//ִ��1������,���ص�ǰ״̬
			//500ms��˸
			vTPCChangeTaskRunTime(TASK_LED1ON, 100);
			return FUN_STATE_INIT;
			
		}
	case FUN_STATE_RUN:
		{
			//�޶���
			//���ؿ��źţ���Ҫ�ⲿ�����޴����¼������ⲿ�¼�����ǿ���˳�
			return SIG_NULL;
		}
	case FUN_STATE_EXIT:
		{
			//�˳��󣬰�ǰһ������ָ�븳ֵ����ǰ����ָ��
			//ִ��1������,���ص�ǰ״̬
			//����������1s��˸
			vTPCChangeTaskRunTime(TASK_LED1ON, 200);
			return FUN_STATE_EXIT;
		}
	default:
		{
			//�޶���
			return SIG_NULL;
			//break;
		}
	}
}

/************************************************************************/
/* �ڲ��¼��Զ��з��ͳ���                                                                     */
/************************************************************************/
uint8_t vqueFunNoErr(void)
{
	switch(coreProcess.funState)
	{
	case FUN_STATE_INIT:
		{
			//�޶��� ֱ���˳�
			return FUN_STATE_INIT;
		}
	case FUN_STATE_RUN:
		{
			//��Ҫ���ص��˳�֮ǰ�ĺ���@@@@@@@@@
			break;
		}
	case FUN_STATE_EXIT:
		{
			//�޶��� ֱ���˳�
			return FUN_STATE_EXIT;
		}
	default:
		{
			//�޶���
			return SIG_NULL;
		}
	}
	return SIG_NULL;
}

uint8_t vqueFunOff(void)
{
	switch(coreProcess.funState)
	{
	case FUN_STATE_INIT:
		{
			//�޶��� ֱ���˳�
			vRelaySet(Relay10Motor, STATE_OFF);
			vRelaySet(Relay03Pump, STATE_OFF);
			vRelaySet(Relay02Valve4way, STATE_OFF);
			vRelaySet(Relay01Compressor, STATE_OFF);
			return FUN_STATE_INIT;
		}
	case FUN_STATE_RUN:
		{
			//�޶��� ֱ���˳�
			return SIG_NULL;
		}
	case FUN_STATE_EXIT:
		{
			//�޶��� ֱ���˳�
			return FUN_STATE_EXIT;
		}
	default:
		{
			//�޶���
			return SIG_NULL;
		}
	}
}

uint8_t vqueFunOn(void)
{
	switch(coreProcess.funState)
	{
	case FUN_STATE_INIT:
		{
			//�޶��� ֱ���˳�
			//vRelaySet(Relay10Motor, STATE_ON);
			//vRelaySet(Relay02Valve4way, STATE_ON);
			switch(timeFlag)
			{
			case Time5s:
				{
					//vRelaySet(Relay10Motor, STATE_ON);
					vRelaySet(Relay03Pump, STATE_ON);
					break;
				}
			case Time20s:
				{
					//2@@@@@@@@@@@���ˮ���պ�
					vRelaySet(Relay10Motor, STATE_ON);
					break;
				}
			case Time180s:
				{
					//����ѹ����
					vRelaySet(Relay01Compressor,STATE_ON);
					timeFlag=0;
					return FUN_STATE_INIT;
				}
			//case Time360S:
			//	{
			//		timeFlag=0;
			//		return FUN_STATE_INIT;
			//	}
			default:break;
			}

			timeFlag++;
			return SIG_NULL;
		}
	case FUN_STATE_RUN:
		{
			//�޶��� ֱ���˳�
			//�����¶�<�趨ֵ �����˪
			if (coreProcess.coreParems.machineA.evaporateTemper <= coreProcess.coreParems.setDefrostInTemper
				&& timeFlag>= coreProcess.coreParems.setDefrostCycleTimes)
			{
				timeFlag=0;
				xQUESigPush(SIG_DEFROST);
				return SIG_NULL;
			}
			//ˮ���¶�>�趨�¶� ���뱣��
			if (coreProcess.coreParems.waterBank >= coreProcess.coreParems.SetWaterTSet)
			{
				timeFlag=0;
				xQUESigPush(SIG_HOLD);
				return SIG_NULL;
			}
			timeFlag++;
			return SIG_NULL;
		}
	case FUN_STATE_EXIT:
		{
			//�޶��� ֱ���˳�

			switch(timeFlag)
			{
			case Time5s:
				{
					vRelaySet(Relay01Compressor,STATE_OFF);
					break;
				}
			case Time20s:
				{
					vRelaySet(Relay10Motor, STATE_OFF);
					vRelaySet(Relay03Pump, STATE_OFF);
					timeFlag=0;
					return FUN_STATE_EXIT;
				}
			default:break;
			}
			timeFlag++;
			return SIG_NULL;
		}
	default:
		{
			//�޶���
			return SIG_NULL;
		}
	}
}

uint8_t vqueFunDefrost(void)
{
	switch(coreProcess.funState)
	{
	case FUN_STATE_INIT:
		{
			switch(timeFlag)
			{
			case Time5s:
				{
					//vRelaySet(Relay10Motor, STATE_ON);
					vRelaySet(Relay03Pump, STATE_ON);
					break;
				}
			case Time20s:
				{
					//2@@@@@@@@@@@���ˮ���պ� ��д.....
					//vRelaySet(Relay10Motor, STATE_ON);
					break;
				}
			case Time60s:
				{
					vRelaySet(Relay02Valve4way,STATE_ON);
					break;
				}
			case Time180s:
				{
					//����ѹ����
					vRelaySet(Relay01Compressor,STATE_ON);
					timeFlag=0;
					return FUN_STATE_INIT;
				}
			/*case Time360S:
				{
					timeFlag=0;
					return FUN_STATE_INIT;
				}*/
			default:break;
			}

			timeFlag++;
			return SIG_NULL;
		}
	case FUN_STATE_RUN:
		{
			//T��>T�趨��|| T��˪ʱ��>��˪�趨ʱ��
			if (coreProcess.coreParems.machineA.evaporateTemper >= coreProcess.coreParems.setDefrostOutTemper
				|| (timeFlag) >= coreProcess.coreParems.setDefrostRunTimes)
			{
				timeFlag=0;
				//Tˮ��>T�趨�����뱣��
				if (coreProcess.coreParems.waterBank >= coreProcess.coreParems.SetWaterTSet)
				{
					xQUESigPush(SIG_HOLD);
					return SIG_NULL;
				}
				else
				{
					xQUESigPush(SIG_ON);
					return SIG_NULL;
				}
			}
			timeFlag++;
			return SIG_NULL;
		}
	case FUN_STATE_EXIT:
		{
			//��ѹ����-60s����ͨ���ϵ�-��ˮ��
			switch(timeFlag)
			{
			case Time5s:
				{
					vRelaySet(Relay01Compressor,STATE_OFF);
					vRelaySet(Relay03Pump, STATE_OFF);
					break;
				}
			case Time60s:
				{
					//vRelaySet(Relay10Motor, STATE_OFF);
					vRelaySet(Relay02Valve4way, STATE_OFF);
					timeFlag=0;
					return FUN_STATE_EXIT;
				}
			}
			timeFlag++;
			return SIG_NULL;
		}
	default:
		{
			//�޶���
			return SIG_NULL;
		}
	}
}

uint8_t vqueFunHold(void)
{
	switch(coreProcess.funState)
	{
	case FUN_STATE_INIT:
		{
			//�޶��� ֱ���˳�
			return FUN_STATE_INIT;
		}
	case FUN_STATE_RUN:
		{
			//Tˮ��<T�趨-�ز�������
			if (coreProcess.coreParems.waterBank <= (coreProcess.coreParems.SetWaterTSet
				-coreProcess.coreParems.setWaterCycleTemper))
			{
				xQUESigPush(SIG_ON);
			}
			return SIG_NULL;
		}
	case FUN_STATE_EXIT:
		{
			//�޶��� ֱ���˳�
			return FUN_STATE_EXIT;
		}
	default:
		{
			//�޶���
			return SIG_NULL;
		}
	}
}

uint8_t vqueFunLowT(void)
{
	switch(coreProcess.funState)
	{
	case FUN_STATE_INIT:
		{
			//�޶��� ֱ���˳�
			return FUN_STATE_INIT;
		}
	case FUN_STATE_RUN:
		{
			//�޶��� ֱ���˳�
			return SIG_NULL;
		}
	case FUN_STATE_EXIT:
		{
			//�޶��� ֱ���˳�
			return FUN_STATE_EXIT;
		}
	default:
		{
			//�޶���
			return SIG_NULL;
		}
	}
}


void vTask3QUEProcess(void)
{
	uint8_t tSig;
	if (coreProcess.funExcuted == FUN_EXCUTED)
	{
		coreProcess.funExcuted = 0;
		//�ⲿ/�ڲ��¼�����ǿ��ת��
		coreProcess.funState=FUN_STATE_EXIT;

		tSig=xQUESigPop();
		switch(tSig)
		{
		case SIG_ERR:
			{
				//����ǰfun��Err�����˳�
				timeFlag=0;
				if (coreProcess.functions == vqueFunErr)
				{
					coreProcess.funExcuted = FUN_EXCUTED;
					coreProcess.funState=FUN_STATE_RUN;
				}
				else
				{
					coreProcess.prefunction=coreProcess.functions;
					coreProcess.functions=vqueFunErr;
				}
				break;
			}
			//������no err
		case SIG_NOERR:
			{
				//ǰһ��������ΪҪ���еĺ���
				timeFlag=0;
				coreProcess.functions=coreProcess.prefunction;
				coreProcess.prefunction=vqueFunErr;
				break;
			}
		case SIG_OFF:
			{
				timeFlag=0;
				coreProcess.prefunction=coreProcess.functions;
				coreProcess.functions=vqueFunOff;
				break;
			}
		default:
			{
				switch(tSig)
				{
				case SIG_ON:
					{
						timeFlag=0;
						coreProcess.prefunction=coreProcess.functions;
						coreProcess.functions=vqueFunOn;
						break;
					}
				case SIG_DEFROST:
					{
						timeFlag=0;
						coreProcess.prefunction=coreProcess.functions;
						coreProcess.functions=vqueFunDefrost;
						break;
					}
				case SIG_HOLD:
					{
						timeFlag=0;
						coreProcess.prefunction=coreProcess.functions;
						coreProcess.functions=vqueFunHold;
						break;
					}
				case SIG_LOWT:
					{
						timeFlag=0;
						coreProcess.prefunction=coreProcess.functions;
						coreProcess.functions=vqueFunLowT;
						break;
					}
				default:
					{
						coreProcess.funExcuted = FUN_EXCUTED;
						coreProcess.funState=FUN_STATE_RUN;
						break;
					}
				}
				break;
			}
		}
	}

	tSig = coreProcess.tempfun();
	//״̬���е�runʱ Ӧ�����ñ���ת�����
	if (coreProcess.funExcuted != FUN_EXCUTED)
	{
		if (coreProcess.funState==FUN_STATE_RUN)
		{
			coreProcess.funExcuted = FUN_EXCUTED;
		}

	}
	switch(tSig)
	{
	case FUN_STATE_EXIT:
		{
			//�˳�֮ǰ������һ�κ���ת��
			coreProcess.funState= FUN_STATE_INIT;
			coreProcess.tempfun=coreProcess.functions;
			break;
		}
	case FUN_STATE_INIT:
		{
			coreProcess.funState = FUN_STATE_RUN;
			break;
		}
	case FUN_STATE_RUN:
		{
			//�ڲ��¼��˳�-Ԥ�Ʋ���ִ�У���Ϊ�ڲ�ת��Ҳ���¼�����
			//coreProcess.funState = FUN_STATE_EXIT;
			//coreProcess.funExcuted = FUN_EXCUTED;
			break;
		}
	default:
		{
			//��Ҫִ�ж�����ڣ���ת����ֱ���˳�
			break;
		}
	}
	//@@@@@@@@@@@����ִ��
}

void vQUEInit(void)
{
	coreProcess.coreParems.SetWaterTSet= 55*10;
	coreProcess.coreParems.setWaterCycleTemper=5*10;
	coreProcess.coreParems.setDefrostInTemper=-7*10;
	coreProcess.coreParems.setDefrostRunTimes=8*60*20;
	coreProcess.coreParems.setDefrostOutTemper=10*10;

	coreProcess.coreParems.setDefrostCycleTimes=5*30*60*20;
	coreProcess.coreParems.setDefrostInOffset=3*10;

	coreProcess.funExcuted=FUN_EXCUTED;
	coreProcess.funState=FUN_STATE_RUN;
	coreProcess.tempfun=vqueFunOff;
	coreProcess.functions=vqueFunOff;
	coreProcess.prefunction=vqueFunOff;

}

