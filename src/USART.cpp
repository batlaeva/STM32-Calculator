#include "USART.h"

void usart_send(char* charBuffer, unsigned int count) {
	while (count--) {
		USART_SendData(USART1, *charBuffer++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	}
}

void usart_init(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef initStruct;

//	TX
	initStruct.GPIO_Pin = GPIO_Pin_9;
	initStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	initStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &initStruct);

//	RX
	initStruct.GPIO_Pin = GPIO_Pin_10;
	initStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &initStruct);

//	UART
	USART_InitTypeDef usartStruct;
	usartStruct.USART_BaudRate = 115200;
	usartStruct.USART_WordLength = USART_WordLength_8b;
	usartStruct.USART_StopBits = USART_StopBits_1;
	usartStruct.USART_Parity = USART_Parity_No;
	usartStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &usartStruct);
	USART_Cmd(USART1, ENABLE);
}
