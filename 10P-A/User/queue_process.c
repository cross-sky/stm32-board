#include "cominc.h"

QueueProcess_t queueProcess={
	0,0
};

//1.init @@@@@@@@
CoreProcess_t coreProcess;

//进入/退出状态 结束后必须清0
uint32_t timeFlag=0;

//!!!!!!!每个case 后面必须配一个break/return 否则会处理出错！！！！！！！！

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
/* 根据环温设置进入蒸发温度值                                                                     */
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
	//进 出 环 水箱 吸 排 蒸 电流互感 adc错误 cd4051错误 内部温度  (高位在前)

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

	*dst++=0;//占位 B吸 蒸 排
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

	//返回A机状态
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
/* 错误事件发生时转入，                                                                     */
/************************************************************************/
uint8_t vqueFunErr(void)
{
	switch(coreProcess.funState)
	{
	case FUN_STATE_INIT:
		{
			//@@@@停止继电器输出，电子膨胀阀调0？
			//执行1个周期,返回当前状态
			//500ms闪烁
			vTPCChangeTaskRunTime(TASK_LED1ON, 100);
			return FUN_STATE_INIT;
			
		}
	case FUN_STATE_RUN:
		{
			//无动作
			//返回空信号，需要外部发出无错误事件，由外部事件进行强制退出
			return SIG_NULL;
		}
	case FUN_STATE_EXIT:
		{
			//退出后，把前一个函数指针赋值到当前运行指针
			//执行1个周期,返回当前状态
			//返回正常的1s闪烁
			vTPCChangeTaskRunTime(TASK_LED1ON, 200);
			return FUN_STATE_EXIT;
		}
	default:
		{
			//无动作
			return SIG_NULL;
			//break;
		}
	}
}

/************************************************************************/
/* 内部事件以队列发送出来                                                                     */
/************************************************************************/
uint8_t vqueFunNoErr(void)
{
	switch(coreProcess.funState)
	{
	case FUN_STATE_INIT:
		{
			//无动作 直接退出
			return FUN_STATE_INIT;
		}
	case FUN_STATE_RUN:
		{
			//需要返回到退出之前的函数@@@@@@@@@
			break;
		}
	case FUN_STATE_EXIT:
		{
			//无动作 直接退出
			return FUN_STATE_EXIT;
		}
	default:
		{
			//无动作
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
			//无动作 直接退出
			vRelaySet(Relay10Motor, STATE_OFF);
			vRelaySet(Relay03Pump, STATE_OFF);
			vRelaySet(Relay02Valve4way, STATE_OFF);
			vRelaySet(Relay01Compressor, STATE_OFF);
			return FUN_STATE_INIT;
		}
	case FUN_STATE_RUN:
		{
			//无动作 直接退出
			return SIG_NULL;
		}
	case FUN_STATE_EXIT:
		{
			//无动作 直接退出
			return FUN_STATE_EXIT;
		}
	default:
		{
			//无动作
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
			//无动作 直接退出
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
					//2@@@@@@@@@@@检测水流闭合
					vRelaySet(Relay10Motor, STATE_ON);
					break;
				}
			case Time180s:
				{
					//开启压缩机
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
			//无动作 直接退出
			//蒸发温度<设定值 进入除霜
			if (coreProcess.coreParems.machineA.evaporateTemper <= coreProcess.coreParems.setDefrostInTemper
				&& timeFlag>= coreProcess.coreParems.setDefrostCycleTimes)
			{
				timeFlag=0;
				xQUESigPush(SIG_DEFROST);
				return SIG_NULL;
			}
			//水箱温度>设定温度 进入保温
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
			//无动作 直接退出

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
			//无动作
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
					//2@@@@@@@@@@@检测水流闭合 待写.....
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
					//开启压缩机
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
			//T蒸>T设定：|| T除霜时间>除霜设定时间
			if (coreProcess.coreParems.machineA.evaporateTemper >= coreProcess.coreParems.setDefrostOutTemper
				|| (timeFlag) >= coreProcess.coreParems.setDefrostRunTimes)
			{
				timeFlag=0;
				//T水箱>T设定：进入保温
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
			//关压缩机-60s后四通阀断电-关水泵
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
			//无动作
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
			//无动作 直接退出
			return FUN_STATE_INIT;
		}
	case FUN_STATE_RUN:
		{
			//T水箱<T设定-回差：进入加热
			if (coreProcess.coreParems.waterBank <= (coreProcess.coreParems.SetWaterTSet
				-coreProcess.coreParems.setWaterCycleTemper))
			{
				xQUESigPush(SIG_ON);
			}
			return SIG_NULL;
		}
	case FUN_STATE_EXIT:
		{
			//无动作 直接退出
			return FUN_STATE_EXIT;
		}
	default:
		{
			//无动作
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
			//无动作 直接退出
			return FUN_STATE_INIT;
		}
	case FUN_STATE_RUN:
		{
			//无动作 直接退出
			return SIG_NULL;
		}
	case FUN_STATE_EXIT:
		{
			//无动作 直接退出
			return FUN_STATE_EXIT;
		}
	default:
		{
			//无动作
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
		//外部/内部事件进行强制转换
		coreProcess.funState=FUN_STATE_EXIT;

		tSig=xQUESigPop();
		switch(tSig)
		{
		case SIG_ERR:
			{
				//若当前fun是Err，则退出
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
			//？？？no err
		case SIG_NOERR:
			{
				//前一个函数置为要运行的函数
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
	//状态运行到run时 应该设置本次转换完成
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
			//退出之前进行下一次函数转换
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
			//内部事件退出-预计不会执行，因为内部转换也以事件产生
			//coreProcess.funState = FUN_STATE_EXIT;
			//coreProcess.funExcuted = FUN_EXCUTED;
			break;
		}
	default:
		{
			//需要执行多个周期，无转换，直接退出
			break;
		}
	}
	//@@@@@@@@@@@函数执行
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

