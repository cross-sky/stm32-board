#include "cominc.h"

//环形 数组结构体实例化两个变量
UartBuf	 UartTxbuf;//环形发送结构体
UartBuf	 UartRxbuf;//环形接收结构体

uint8_t rx_buffer[RX_BUFFER_SIZE];
uint8_t tx_buffer[TX_BUFFER_SIZE];



//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕 ,根据uart端口调整
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 


//输入参数：u32 baudrate   设置RS232串口的波特率
void UART1_Init(uint32_t bound)
{
	GPIO_InitTypeDef	gpio_inits;
	USART_InitTypeDef	usart_inits;
	NVIC_InitTypeDef	nvic_inits;

	RCC_APB2PeriphClockCmd(USARTy_GPIO_CLK|USARTy_CLK, ENABLE);//根据uart与gpio时钟线设置
	USART_DeInit(USARTy);
	//pA.9 复用推挽输出 USART1_TX;
	gpio_inits.GPIO_Pin	= USARTy_TxPin;
	gpio_inits.GPIO_Speed = GPIO_Speed_2MHz;
	gpio_inits.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USARTy_GPIO, &gpio_inits);

	//PA.10 浮空输入 usart1_RX;
	gpio_inits.GPIO_Pin = USARTy_RxPin;
	gpio_inits.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USARTy_GPIO, &gpio_inits);

	usart_inits.USART_BaudRate = bound;
	usart_inits.USART_WordLength = USART_WordLength_8b;
	usart_inits.USART_StopBits = USART_StopBits_1;
	usart_inits.USART_Parity = USART_Parity_No;
	usart_inits.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_inits.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_Init(USARTy, &usart_inits);
	//第2组：最高2位用于指定抢占式优先级-PreemptionPriority，最低2位用于指定响应优先级-SubPriority
	//“组”优先级别>“抢”占优先级别>“副”优先级别
	//数值越小所代表的优先级就越高,有必要设置组0
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	nvic_inits.NVIC_IRQChannel = USARTy_IRQn;
	nvic_inits.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_inits.NVIC_IRQChannelSubPriority = 8;
	nvic_inits.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&nvic_inits);

	USART_ITConfig(USARTy, USART_IT_RXNE, ENABLE);  //接收缓冲区非空中断使能
	USART_Cmd(USARTy, ENABLE);

	UartTxbuf.Wd_Index = 0;
	UartTxbuf.Rd_Index = 0;
	UartTxbuf.Mask = TX_BUFFER_SIZE - 1;
	UartTxbuf.pbuf = &tx_buffer[0];

	UartRxbuf.Wd_Index = 0;
	UartRxbuf.Rd_Index = 0;
	UartRxbuf.Mask = RX_BUFFER_SIZE -1;
	UartRxbuf.pbuf = &rx_buffer[0];

//	printf("sysclock: %dMHz \r\n", cycleCounter());
#ifdef Debug
	printf("uart1 bound: %d \r\n", bound);
#endif
}


//unsigned char DataToSend   要发送的字节数据 RS232发送一个字节
void UART1_Put_char(uint8_t dataToSend)
{
	UartBuf_WD(&UartTxbuf, dataToSend);
	USART_ITConfig(USARTy, USART_IT_TXE, ENABLE);

}

//将一个字节写入一个环形结构体中
void UartBuf_WD(UartBuf *ringBuf, uint8_t dataIn)
{
	ringBuf->pbuf[ringBuf->Wd_Index & ringBuf->Mask] = dataIn;
	ringBuf->Wd_Index++;
}


////读取环形数据中的一个字节
uint8_t UartBuf_RD(UartBuf *ringBuf)
{
	uint8_t temp;
	temp = ringBuf->pbuf[ringBuf->Rd_Index &ringBuf->Mask];
	ringBuf->Rd_Index++;
	return temp;
}


//环形数据区的可用字节长度，当写指针写完一圈，追上了读指针 (wd+mask-id)%mask
//那么证明数据写满了，此时应该增加缓冲区长度，或者缩短外围数据处理时间
uint16_t UartBuf_Cnt(UartBuf *ringBuf)
{
	return (ringBuf->Mask +ringBuf->Wd_Index - ringBuf->Rd_Index) % ringBuf->Mask;
}

void UartBufClear(UartBuf *ringBuf)
{
	ringBuf->Rd_Index = ringBuf->Wd_Index =0;
}

void UartSendBuffer(uint8_t *dat, uint8_t len)
{
	uint8_t i;
	for (i=0; i<len; i++)
	{
		UartBuf_WD(&UartTxbuf, *dat);
		dat++;
	}
	USART_ITConfig(USARTy, USART_IT_TXE, ENABLE);//
}

volatile uint8_t UdataTemp;//串口接收临时数据字节

void USARTy_IRQHandler(void)
{
//	uint8_t i;
	if (USART_GetITStatus(USARTy, USART_IT_TXE) != RESET)
	{
		USART_SendData(USARTy, UartBuf_RD(&UartTxbuf));
		if(UartBuf_Cnt(&UartTxbuf) == 0 )
			USART_ITConfig(USARTy, USART_IT_TXE, DISABLE);
	}

	else if(USART_GetITStatus(USARTy, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USARTy, USART_IT_RXNE);//清除接收中断标志
		UdataTemp = (uint8_t)USART_ReceiveData(USARTy);

		UartBuf_WD(&UartRxbuf, UdataTemp);//写串口接收缓冲数组

	}

}

void RTtest(void)
{
	uint8_t i, cnTemp;
	cnTemp = UartBuf_Cnt(&UartRxbuf);
	if (cnTemp == 5)
	{
		for (i=0; i<cnTemp; i++)
		{
			UartBuf_WD(&UartTxbuf, UartBuf_RD(&UartRxbuf));
		}
		
			USART_ITConfig(USARTy, USART_IT_TXE, ENABLE);
	}
//	UartBufClear(&UartRxbuf);


}
