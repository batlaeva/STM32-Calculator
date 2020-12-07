#ifndef LCD_H_
#define LCD_H_

	#include <stm32f10x_rcc.h>
	#include <stm32f10x_gpio.h>

	#include "diag/Trace.h"
	#include "stm32f10x.h"
	#include "stm32f10x_gpio.h"
	#include "stm32f10x_rcc.h"

	#define LCD_PORT GPIOA
	#define LCD_RCC_GPIO RCC_APB2Periph_GPIOA

	#define LCD_E_Pin GPIO_Pin_12
	#define LCD_RS_Pin GPIO_Pin_8

	void delay(unsigned int s);

	const uint8_t lcd_2x16_decode[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	void lcd_init_gpio();
	void lcd_write_data(u16 data);
	void lcd_init();
	void lcd_write_str(char*str);
	void lcd_write_cmd(u16 cmd);
	void lcd_set_cursor(int line, int pos);

#endif
