#include "cominc.h"

#define SEC 0x02
#define MIN 0x03
#define HOUR 0x04
#define DAY 0x05
#define WEEK 0x06
#define MONTH 0x07
#define YEAR 0x08

#define Alarm 0x0a

#define PCF8563ADDR 0xa2

uint8_t const C8563store[4]={0,0,12,2};
uint8_t g853[3] = {0};

char pcf[16];
char pcftim[12];

void Pcf8563SetTime(const uint8_t pcfst[3])
{
	uint8_t i;
	uint8_t bcdtime[3]={0};
	for (i=0; i<3; i++)
	{
		//convert to bcd
		bcdtime[i] = (pcfst[i]/10 << 4) | pcfst[i]%10;
	}

	IICWriteBytes(PCF8563ADDR, SEC, 3, bcdtime);
}

void Pcf8563ReadTime( uint8_t pcfst[3])
{
	uint8_t i;
	uint8_t bcdtime[3]={0};
	
	uint8_t k,l;
	IICReadBytes(PCF8563ADDR, SEC, 3, bcdtime);
	bcdtime[2] = bcdtime[2]&0x3f;
	for (i=0; i<3; i++)
	{
		//bcd convert to 10
		//bcdtime[i] =bcdtime[i]&0x7f;
		k=(bcdtime[i]&0x70)>>4;
		l=bcdtime[i]&0x0f;
		pcfst[i] = k*10 + l;
		//pcfst[i]=((bcdtime[i]&0x70)>>4)*10 + bcdtime[i]&0x0f;//这样有问题？
	}
}

void PCF8563Init(void)
{
	uint8_t len;
	uint8_t falarm=0;
	IICReadByte(PCF8563ADDR, Alarm, &falarm);
	if (falarm != 0x07)
	{
		Pcf8563SetTime(C8563store);
		IICWriteByte(PCF8563ADDR,0x00, 0x00);
		IICWriteByte(PCF8563ADDR,0x0a, 0x07);//alarm 8:00
		IICWriteByte(PCF8563ADDR,0x01,0x12);//alarm enable
		MenuParam.clock.hour = C8563store[2];
		MenuParam.clock.min = C8563store[1];
#ifdef Debug
		sprintf(pcf, "%s","pcf set..\r\n");
		len = strlen(pcf);
		UartDMAQueue(qUartLink,(uint8_t*)pcf,len);
		//printf("pcf seting");
#endif
	}
	else
	{
		//Pcf8563ReadTime(g853);
#ifdef Debug
		//printf("pcf reading");
		sprintf(pcf, "%s","pcf on..\r\n");
		len = strlen(pcf);
		UartDMAQueue(qUartLink,(uint8_t*)pcf,len);
#endif
		Pcf8536RT();
		/*sprintf(pcftim, "%d:%d:%d ",g853[2],g853[1],g853[0]);
		len = strlen(pcftim);
		UartDMAQueue(qUartLink,(uint8_t*)pcftim,len);*/
		//printf("%d:%d:%d",g853[2],g853[1],g853[0]);
	}
}

void Pcf8536RT(void)
{
	uint8_t len;
	Pcf8563ReadTime(g853);

	MenuParam.clock.hour = g853[2];
	MenuParam.clock.min = g853[1];
	

#ifdef Debug
	//printf("%d:%d:%d ",g853[2],g853[1],g853[0]);
	sprintf(pcftim, "%d:%d:%d ",g853[2],g853[1],g853[0]);
	len = strlen(pcftim);
	UartDMAQueue(qUartLink,(uint8_t*)pcftim,len);
#endif

}

