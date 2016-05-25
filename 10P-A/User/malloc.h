#ifndef __MALLOC_H
#define __MALLOC_H

#define		SRAMIN		0		//内部内存池

#define		SRAMBANK	1		//内存块数1

#define		MEM1_BLOCK_SIZE		16		//内存块大小
#define		MEM1_MAX_SIZE		4*1024	//内存数量
#define		MEM1_ALLOC_TABLE_SIZE	MEM1_Max_size/MEM1_BLOCK_SIZE	//内存表大小

typedef struct _m_mallco_dev
{
	void (*init)(uint8_t);			//初始化
	uint8_t	(*perused)(uint8_t);	//内存使用率
	uint8_t	*membase[SRAMBANK];		//内存池
	uint8_t	*memmap[SRAMBANK];		//内存管理状态表(每次最大申请256字节数据表)
	uint8_t memrdy[SRAMBANK];
}MALLOC_DEV;

void vMemSet(void *s,uint8_t c,uint16_t count);	//设置内存
void vMemCpy(void *des,void *src, uint16_t n);	//复制内存
void vMemInit(uint8_t memx);		//初始化
uint8_t uMemPreused(uint8_t memx);		//内存使用率

uint8_t uMemFree(uint8_t memx, void *ptr);		//释放内存
void* pMemMaloc(uint8_t memx, uint8_t size);		//分配内存
void* pMemRemalloc(uint8_t memx, void *ptr, uint8_t size);	//重新分配内存


enum{
	MEM_FAIL=0,
	MEM_SUC=1
};


#endif