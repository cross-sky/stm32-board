#ifndef _STM32_UB_DMA_USART1_H
#define _STM32_UB_DMA_USART1_H

#define USART1_Rx				USART1
#define USART1_Rx_GPIO			GPIOA
#define USART1_Rx_CLK			RCC_APB2Periph_USART1
#define USART1_Rx_GPIO_CLK		RCC_APB2Periph_GPIOA
#define USART1_RxPin			GPIO_Pin_10

#define USART1_Tx				USART1
#define USART1_Tx_GPIO			GPIOA
#define USART1_Tx_CLK			RCC_APB2Periph_USART1
#define USART1_Tx_GPIO_CLK		RCC_APB2Periph_GPIOA
#define USART1_TxPin			GPIO_Pin_9
//这是一个双向寄存器，包含了TDR和RDR，对它读操作，读取的是RDR寄存器的值，对它的写操作，实际上是写到TDR寄存器的
#define USART1_Tx_DR_Base		0x40013804
#define DMA1_Ch_Usart1_Tx		DMA1_Channel4
#define DMA1_Ch_Usart1_Tx_IRQn	DMA1_Channel4_IRQn

#define USART1_Rx_DR_Base		0x40013804
#define DMA1_Ch_Usart1_Rx		DMA1_Channel5
#define DMA1_Ch_Usart1_Rx_IRQn	DMA1_Channel5_IRQn

#define FIFO_SIZE              64
#define TxFIFOSIZE              128//0x7f

#define Bounds	9600

typedef struct UART_QNode{
	uint8_t		*Addr_8U;
	uint8_t		length;
	struct	UART_QNode *Next;
}UART_QNode, *UART_QueuePtr;

typedef struct{
	UART_QueuePtr front, rear;
}UART_LinkQueue;

typedef struct 
{
	uint8_t Txbuff[TxFIFOSIZE];
	uint8_t in;
	uint8_t out;
}stuTxBuf;

void UartDMAQueue(UART_LinkQueue *q,uint8_t *addres_8u, uint8_t length);
void UART1_Init(void);

void Usart1_handle(void);
void Dma1ChRxHandle(void);
void Dma1ChTxHandle(void);
void UartDmaSend(void);

void Usart1HandleV2(void);
void Dma1ChTxHandleV2(void);
void UartDmaSendV2(void);
extern UART_LinkQueue *qUartLink;;
#endif


