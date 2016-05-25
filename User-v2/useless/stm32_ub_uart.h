#ifndef __STM32_UB_UART_H
#define __STM32_UB_UART_H

typedef enum
{
	COM1 = 0	// COM1 (TX=PA9, RX=PA10)
}UART_NAME_t;

#define  UART_ANZ   1 // Anzahl von UART_NAME_t

typedef enum
{
	RX_EMPTY = 0,  // nichts empfangen
	RX_READY,      // es steht was im Empfangspuffer
	RX_FULL        // RX-Puffer ist voll
}UART_RXSTATUS_t;

typedef struct {
	GPIO_TypeDef* PORT;     // Port
	const uint16_t PIN;     // Pin
	const uint32_t CLK;     // Clock
	const uint8_t SOURCE;   // Source
}UART_PIN_t;

typedef struct {
	UART_NAME_t UART_NAME;    // Name
	const uint32_t CLK;       // Clock
//	const uint8_t AF;         // AF
	USART_TypeDef* UART;      // UART
	const uint32_t BAUD;      // Baudrate
	const uint8_t INT;        // Interrupt
	UART_PIN_t TX;            // TX-Pin
	UART_PIN_t RX;            // RX-Pin
}UART_t;

#define RX_BUF_SIZE		50
#define RX_FIRST_CHR	0x20
#define RX_LAST_CHR		0x7e
#define RX_END_CHR		0x0d

typedef struct{
	char	rx_buf[RX_BUF_SIZE];
	u8		wr_ptr;
	UART_RXSTATUS_t status;
}UART_RX_t;

#endif

