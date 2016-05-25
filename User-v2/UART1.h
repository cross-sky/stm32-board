#ifndef __UART1_H
#define __UART1_H


#include "stm32f10x.h"


#define BoundSet	115200

#define USARTy				USART1
#define USARTy_GPIO			GPIOA
#define USARTy_CLK			RCC_APB2Periph_USART1
#define USARTy_GPIO_CLK		RCC_APB2Periph_GPIOA
#define USARTy_RxPin		GPIO_Pin_10
#define USARTy_TxPin		GPIO_Pin_9
#define USARTy_IRQn			USART1_IRQn
#define USARTy_IRQHandler	USART1_IRQHandler

#define RX_BUFFER_SIZE	32
#define TX_BUFFER_SIZE	32

typedef struct
{
	uint8_t volatile Wd_Index;
	uint8_t volatile Rd_Index;
	uint8_t Mask;
	uint8_t	 *pbuf;
}UartBuf;

void UART1_Init(uint32_t bound);
void UART1_Put_char(uint8_t dataToSend);
void UartBuf_WD(UartBuf *ringBuf, uint8_t dataIn);
void UartSendBuffer(uint8_t *dat, uint8_t len);
void UART1_Put_char(uint8_t dataToSend);
void RTtest(void);
#endif

