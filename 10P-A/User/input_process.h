#ifndef __INPUT_PROCESS_H
#define __INPUT_PROCESS_H

#define INPUT_COUNT_MAX 5
#define INPUT_TIMES_MAX 20
typedef struct{
	uint8_t countCd4051;
	uint8_t countNtc;
	uint16_t dataCd4051[INPUT_COUNT_MAX];
	uint16_t dataNtc[INPUT_COUNT_MAX][ADCMAX];
	uint16_t finaNtc[ADCMAX];
	int16_t realNtc[ADCMAX];
	uint16_t finaCD4051;
}InputProcess_t;

extern InputProcess_t dataInput;

uint8_t uINPCd405xExcuted(void);
void vINPSendCd405xData(uint16_t data);
uint16_t* uINPGetADCaddress(void);
uint8_t uINPADCExcuted(void);
void vINPADCCompleteSampling(void);

void Task2InputProcess(void);
void Task1RegularSimpling(void);

#endif
