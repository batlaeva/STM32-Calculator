#ifndef USART_H_
#define USART_H_

	#include "diag/Trace.h"
	#include "stm32f10x.h"
	#include "stm32f10x_gpio.h"
	#include "stm32f10x_rcc.h"
	#include "stm32f10x_usart.h"

	void usart_send(char* charBuffer, unsigned int count);
	void usart_init(void);

#endif
