#ifndef __MALLOC_H
#define __MALLOC_H

#define		SRAMIN		0		//�ڲ��ڴ��

#define		SRAMBANK	1		//�ڴ����1

#define		MEM1_BLOCK_SIZE		16		//�ڴ���С
#define		MEM1_MAX_SIZE		4*1024	//�ڴ�����
#define		MEM1_ALLOC_TABLE_SIZE	MEM1_Max_size/MEM1_BLOCK_SIZE	//�ڴ���С

typedef struct _m_mallco_dev
{
	void (*init)(uint8_t);			//��ʼ��
	uint8_t	(*perused)(uint8_t);	//�ڴ�ʹ����
	uint8_t	*membase[SRAMBANK];		//�ڴ��
	uint8_t	*memmap[SRAMBANK];		//�ڴ����״̬��(ÿ���������256�ֽ����ݱ�)
	uint8_t memrdy[SRAMBANK];
}MALLOC_DEV;

void vMemSet(void *s,uint8_t c,uint16_t count);	//�����ڴ�
void vMemCpy(void *des,void *src, uint16_t n);	//�����ڴ�
void vMemInit(uint8_t memx);		//��ʼ��
uint8_t uMemPreused(uint8_t memx);		//�ڴ�ʹ����

uint8_t uMemFree(uint8_t memx, void *ptr);		//�ͷ��ڴ�
void* pMemMaloc(uint8_t memx, uint8_t size);		//�����ڴ�
void* pMemRemalloc(uint8_t memx, void *ptr, uint8_t size);	//���·����ڴ�


enum{
	MEM_FAIL=0,
	MEM_SUC=1
};


#endif