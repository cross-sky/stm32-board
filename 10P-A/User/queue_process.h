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
	SIG_DEFROST,		//��˪
	SIG_HOLD,			//����
	SIG_LOWT,			//������ <-15
	SIG_OFF,			//�ػ�״̬
	SIG_ERR,			//����
	SIG_NOERR,			//�޴���
//	FUN_EXIT,			//�����˳�
//	FUN_INIT,			//������ʼ��
//	FUN_RUN,			//��������
	FUN_EXCUTED,		//״̬ת�����
	FUN_STATE_INIT,		//������ʼ��״̬
	FUN_STATE_RUN,		//��������״̬
	FUN_STATE_EXIT		//�����˳�״̬
};

typedef struct{
	uint8_t state;				//״̬�����
	int16_t inTemper;			//�����¶�
	int16_t outTemper;			//�����¶�
	int16_t evaporateTemper;	//�����¶�
	int16_t current;
}Machine_t;

typedef struct{
	int16_t SetWaterTSet;		//����ˮ��
	int16_t setWaterCycleTemper;	//����ˮ�»ز�
	int16_t setDefrostInTemper;	//�����˪�¶�
	int16_t setDefrostRunTimes;		//��˪ʱ��

	int16_t setDefrostOutTemper;		//�˳���˪�¶�
	int32_t setDefrostCycleTimes;		//��˪����
	int16_t setDefrostInOffset;			//��˪����ƫ��

	int16_t setDefrostMode;			//��˪���Ʒ�ʽ	


	int16_t setValveMode;			//�������ͷ����Ʒ�ʽ
	int16_t setValveManulSteps;		//�ֶ��������ͷ�����
	int16_t setSuperHeatLow5;		//����5�����¹��ȶ�
	int16_t setSuperHeatOver5;		//����5�����Ϲ��ȶ�
	int16_t setElecHeatOnTemper;	//���ȹ����������趨�¶�

	int16_t setElecOnState;			//ǿ�����������
	int16_t setCompressorNumbers;	//ѹ��������
	int16_t setCurrentProtectMax;	//����������׼
	int16_t setPhaseProtect;		//��λ����

	uint16_t adcDectectState;	//adc����״̬
	uint16_t cd4051DectState;	//cd4051����״̬
	int16_t waterIn;
	int16_t waterOut;
	int16_t waterBank;			//ˮ���¶�
	int16_t humidity;			//ʪ��
	int16_t environT;			//����
	int16_t innerTemper;		//�ڲ��¶�
	uint16_t valvesteps;
	Machine_t machineA;
}CoreParams_t;

typedef struct{
	uint8_t funExcuted;			//״̬�Ƿ�ת�����
	uint8_t funState;			//״̬
//	uint8_t funStateCount;		//״̬ת������
	ptrfuntion tempfun;
	ptrfuntion functions;
	ptrfuntion prefunction;
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

