#ifndef __QUEUE_PROCESS_H
#define __QUEUE_PROCESS_H

#define QueueBuffLen 32
#define QueueBuffLenMask 0x1f

typedef uint8_t (*ptrfuntion)(void);

typedef struct{
//	uint8_t code;
	uint8_t sig;
}QueueSig_t;

typedef struct 
{
	uint8_t in;
	uint8_t out;
	QueueSig_t buff[QueueBuffLen];
}QueueProcess_t;

enum{
	SIG_NULL=0x00,		//
	SIG_ON,				//
	SIG_DEFROST,		//除霜
	SIG_HOLD,			//保温
	SIG_LOWT,			//极低温 <-15
	SIG_OFF,			//关机状态
	SIG_ERR,			//错误
	SIG_NOERR,			//无错误
//	FUN_EXIT,			//函数退出
//	FUN_INIT,			//函数初始化
//	FUN_RUN,			//函数运行
	FUN_EXCUTED,		//状态转换完成
	FUN_STATE_INIT,		//函数初始化状态
	FUN_STATE_RUN,		//函数运行状态
	FUN_STATE_EXIT		//函数退出状态
};

typedef struct{
	uint8_t onState;
	uint8_t elecState;
	uint8_t defrostState;
}RunState_t;

typedef struct{
	uint8_t state;				//状态开或关
	int16_t inTemper;			//吸气温度
	int16_t outTemper;			//排气温度
	int16_t evaporateTemper;	//蒸发温度
	int16_t current;
}Machine_t;

typedef struct{
	int16_t SetWaterTSet;		//设置水温
	int16_t setWaterCycleTemper;	//设置水温回差
	int16_t setDefrostInTemper;	//进入除霜温度
	int16_t setDefrostRunTimes;		//除霜时间

	int16_t setDefrostOutTemper;		//退出除霜温度
	int32_t setDefrostCycleTimes;		//除霜周期
	int16_t setDefrostInOffset;			//除霜进入偏差

	int16_t setDefrostMode;			//除霜控制方式	

	int16_t setValveMode;			//电子膨胀阀控制方式
	int16_t setValveManulSteps;		//手动电子膨胀阀步数
	int16_t setSuperHeatLow5;		//环温5℃以下过热度
	int16_t setSuperHeatOver5;		//环温5℃以上过热度
	int16_t setElecHeatOnTemper;	//发热管启动环境设定温度

	int16_t setElecOnState;			//强制启动电加热
	int16_t setCompressorNumbers;	//压缩机数量
	int16_t setCurrentProtectMax;	//电流保护基准
	int16_t setPhaseProtect;		//相位保护

	uint16_t adcDectectState;	//adc错误状态
	uint16_t cd4051DectState;	//cd4051错误状态
	int16_t waterIn;
	int16_t waterOut;
	int16_t waterBank;			//水箱温度
	int16_t humidity;			//湿度
	int16_t humiTemper;			//湿度模块读取的温度
	int16_t environT;			//环温
	int16_t innerTemper;		//内部温度
	uint16_t valvesteps;
	Machine_t machineA;
}CoreParams_t;

typedef struct{
	uint8_t funExcuted;			//状态是否转换完成
	uint8_t funState;			//状态
//	uint8_t funStateCount;		//状态转换次数
	ptrfuntion tempfun;
	ptrfuntion functions;
	ptrfuntion prefunction;
	RunState_t runState;
	CoreParams_t coreParems;
}CoreProcess_t;

uint8_t xQUESigPush(uint8_t sig);
void vQUESetADCDectState(uint16_t adcstate);
void vQUESetCD4051DectState(uint16_t cd4state);
void vQUEGetTemperParams(uint8_t *dst);
void vInpSetCoreParamADC(int16_t *realNtc);

void vTask3QUEProcess(void);
void vQUEInit(void);
void vQueSetCoreParams(int8_t *src);
void vQUEDefrostInTemper(void);
#endif

