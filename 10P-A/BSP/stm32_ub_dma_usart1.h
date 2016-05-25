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

#define Bounds	9600

#define RTxBUF_MAX_COUNTER 5
#define RXBUF_SIZE	39
#define TXBUF_SIZE	79

//#define UartBuffLen	10
typedef struct{
	uint8_t length;
	uint8_t buff[RXBUF_SIZE];
}UartNodeRx_t;

typedef struct{
	uint8_t in;
	uint8_t out;
	UartNodeRx_t node[RTxBUF_MAX_COUNTER];
}UartProcess_t;

//第0位开始
#define RTX_START_BIT	0
typedef enum{
	FrameStart1Bit=0x00,
	FrameStart2Bit,
	FrameDataLenBit,
	FrameFunCodeBit
}RTxEnum;

#define FrameStart1Code 0xfe
#define FrameStart2Code 0xef
#define FrameEndCode	0xca
#define FunCode1		0xf1
#define FunCode2		0xf2
#define FunCode3		0xf3

void vUartRxPopProcess(void);
void UART1_Init(void);
void Usart1IdlHandle_ISR(void);
void vUartDmaRxHandle_ISR(void);
void vUartDmaTxHandler_ISR(void);

#endif


