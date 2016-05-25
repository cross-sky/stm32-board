#ifndef __TUART2_H
#define	__TUART2_H

#include <stdio.h>
#include"Tdso.h"



#define USART2_Rx				USART2
#define USART2_Rx_GPIO			GPIOA
#define USART2_Rx_CLK			RCC_APB1Periph_USART2
#define USART2Rx_GPIO_CLK		RCC_APB2Periph_GPIOA
#define USART2_RxPin			GPIO_Pin_3

#define USART2_Tx				USART2
#define USART2_Tx_GPIO			GPIOA
#define USART2_Tx_CLK			RCC_APB1Periph_USART2
#define USART2_Tx_GPIO_CLK		RCC_APB2Periph_GPIOA
#define USART2_TxPin			GPIO_Pin_2

#define USART2_Tx_DR_Base		(&(USART2->DR))
#define DMA1_Ch_Usart2_Tx		DMA1_Channel7
#define DMA1_Ch_Usart2_Tx_IRQn	DMA1_Channel7_IRQn

#define USART2_Rx_DR_Base		(&(USART2->DR))
#define DMA1_Ch_Usart2_Rx		DMA1_Channel6
#define DMA1_Ch_Usart2_Rx_IRQn	DMA1_Channel6_IRQn

// #define USART1_Rx				USART1
// #define USART1_Rx_GPIO			GPIOA
// #define USART1_Rx_CLK			RCC_APB2Periph_USART1
// #define USART1_Rx_GPIO_CLK		RCC_APB2Periph_GPIOA
// #define USART1_RxPin			GPIO_Pin_10
// 
// #define USART1_Tx				USART1
// #define USART1_Tx_GPIO			GPIOA
// #define USART1_Tx_CLK			RCC_APB2Periph_USART1
// #define USART1_Tx_GPIO_CLK		RCC_APB2Periph_GPIOA
// #define USART1_TxPin			GPIO_Pin_9
// 
// #define USART1_Tx_DR_Base		0x40013804
// #define DMA1_Ch_Usart1_Tx		DMA1_Channel4
// #define DMA1_Ch_Usart1_Tx_IRQn	DMA1_Channel4_IRQn
// 
// #define USART1_Rx_DR_Base		0x40013804
// #define DMA1_Ch_Usart1_Rx		DMA1_Channel5
// #define DMA1_Ch_Usart1_Rx_IRQn	DMA1_Channel5_IRQn


#define BufferSize             250
#define FIFO_SIZE              200

extern uint8_t TxBuffer1[BufferSize];
extern uint8_t RxBuffer2[FIFO_SIZE]; /* Buffer emulating the FIFO */
extern int8_t RxBuffer2_SW[BufferSize]; /* Buffer used for final data store */
extern u8 USART1_TX_Finish;

typedef struct UART_QNode{
	u8		*Addr_8U;
	u8		length;
	struct	UART_QNode *Next;
}UART_QNode, *UART_QueuePtr;

typedef struct{
	UART_QueuePtr front, rear;
}UART_LinkQueue;

Status UART_ENQueue(UART_LinkQueue *q, u8 *addres_8u, u8 length);
Status UART_DEQueue(UART_LinkQueue *q, UART_QueuePtr e);
void DMASendSet(u8 *addres_8u, u8 length);
void UartDMAQueue(UART_LinkQueue *q,u8 *addres_8u, u8 length);
UART_LinkQueue* UartLinkQueueInit(void);

void UART1_Init(void);
void Usart2_Handle(void);
void Dma1_Channel7Handle(void);
void Dma1_Channel6Handle(void);
void teesst(void);
void DMASendAD(DSO_T *g_DSO);
#endif

