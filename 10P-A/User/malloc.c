#include "cominc.h"

//�ֽڶ����ڴ��
__align(4) uint8_t mem1Base[MEM1_MAX_SIZE];	
//�ڴ��
uint8_t mem1MapBase[MEM1_ALLOC_TABLE_SIZE];
//�ڴ�������С
const uint16_t memTblsize[1]={MEM1_ALLOC_TABLE_SIZE};
//�ڴ���С
const uint16_t memBlksize[1]={MEM1_BLOCK_SIZE};
//�ڴ���С
const uint16_t memSize[1]={MEM1_MAX_SIZE};

MALLOC_DEV mallocDev={
	vMemInit,
	uMemPreused,
	mem1Base,
	mem1MapBase,
	0
};

/************************************************************************/
/* copy memory                                                                     */
/************************************************************************/
void vMemCpy(void *des,void *src, uint16_t n)
{
	uint8_t *xdes=des;
	uint8_t *xsrc=src;
	while(n--){
		*xdes++=*src++;
	}
}

void vMemSet(void *s,uint8_t c,uint16_t count)
{
	uint8_t *xs=s;
	while(count--){
		*xs++=c;
	}
}

void vMemInit(uint8_t memx){
	vMemSet(mallocDev.membase[memx], 0, memSize[memx]); //�ڴ���0
	vMemSet(mallocDev.memmap[memx], 0, memTblsize[memx]); //�˴��ڴ�����С��1�ֽڣ�����ĵĻ�ҪX�ֽڴ�С
	mallocDev.memrdy[memx]=1;	//��ʼ��OK
}


uint8_t uMemPreused(uint8_t memx){
	uint16_t used=0,i;
	for(i=0; i<memTblsize[memx]; i++){
		if (mallocDev.memmap[memx][i])
		{
			used++;
		}
	}
	return (used*100)/memTblsize[memx];
}

uint32_t memo_malloc(uint8_t memx, uint8_t size)
{
	uint16_t offset=0;
	uint16_t nmemb;	//��Ҫ���ڴ���
	uint16_t cmemb;	//�������ڴ����
	uint16_t i;

	if (!mallocDev.memrdy[memx])
	{
		mallocDev.init(memx);
	}

	if (size == 0)
	{
		return MEM_FAIL;
	}

	nmemb=size/memBlksize[memx];
	if (size%memBlksize[memx])
	{
		nmemb++;
	}

	for (offset=memTblsize[memx]-1; offset>=0; offset--)
	{
		if (!mallocDev.memmap[memx][offset])
		{
			cmemb++;
		}else{
			cmemb=0;
		}

		if (cmemb == nmemb)
		{
			for (i=0; i<nmemb; i++)
			{
				mallocDev.memmap[memx][offset+i]=nmemb;
			}
			return offset*memBlksize[memx];
		}
	}
	return MEM_FAIL;
}

uint8_t memo_free(uint8_t memx, uint32_t offset)
{
	int i;
	if (!mallocDev.memrdy[memx])
	{
		mallocDev.init(memx);
		return MEM_FAIL;
	}

	if (offset < memSize[memx])
	{
		int index=offset/memTblsize[memx];
		int nmemb=mallocDev.memmap[memx][index];
		for (i=0; i<nmemb; i++)
		{
			mallocDev.memmap[memx][index+i]=0;
		}
		return MEM_SUC;
	}else{
		return MEM_FAIL;
	}
}

uint8_t vMemFree(uint8_t memx, void *ptr){
	uint32_t offset;
	if (ptr == NULL)
	{
		return MEM_FAIL;
	}

	offset=(uint32_t)ptr-(uint32_t)mallocDev.membase[memx];
	return memo_free(memx,offset);
}

void* pMemMaloc(uint8_t memx, uint8_t size)
{
	return memo_malloc(memx,size);
}

void* pMemRemalloc(uint8_t memx, void *ptr, uint8_t size)
{
	uint32_t offset=memo_malloc(memx,size);
	if (offset)
	{
		return MEM_FAIL;
	}else{
		vMemCpy((void*)mallocDev.membase[memx]+offset,ptr,size);
		vMemFree(memx,ptr);
		return (void*)((uint32_t)mallocDev.membase[memx]+offset);
	}

}

