#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"

#include "LCD.h"
#include "USART.h"
#include "Button.h"
#include "Calculation.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

char buffer[50] = {'\0'};
int8_t truth_mas[16] = {0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0};
uint8_t signs[4] = {'+','-','*','/'};

uint8_t keyboard[4][3] = {
		{0,61,41},
		{3,6,9},
		{2,5,8},
		{1,4,7}};

void delay(void) {
	volatile uint32_t i;
	for (i = 0; i <= 0x80000; i++);
}

//void clear() {
//	for (unsigned int i = 0; i < sizeof(buffer) - 2; i++) {
//		buffer[i] = ' ';
//	}
//
//	buffer[sizeof(buffer) - 2] = '\r';
//	buffer[sizeof(buffer) - 1] = '\n';
//}

void encoder_init (void) {
	// энкодер
	GPIO_InitTypeDef initStruct;
	initStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_10;
	initStruct.GPIO_Mode = GPIO_Mode_IPD;
	initStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &initStruct);
}

uint8_t get_mas_index(uint8_t curr_a, uint8_t curr_b, uint8_t prev_a, uint8_t prev_b) {
	curr_a = curr_a << 3;
	curr_b = curr_b << 2;
	prev_a = prev_a << 1;

	curr_a = curr_a | curr_b | prev_a | prev_b;
	return curr_a;
}

void clear_expr(char* expr) {
	for (uint8_t i=0; i<16; i++) {
		expr[i] = '\0';
	}
}

int main(void) {

	lcd_init();
	usart_init();
	button_matrix_init();
	encoder_init();

	uint16_t pin;
	uint8_t i,j;
	uint8_t data;

	// энкодер
	uint8_t prev_a = 0;
	uint8_t curr_a = 0;
	uint8_t prev_b = 0;
	uint8_t curr_b = 0;
	int degree = 0;
	int prev_degree = 0;
	uint8_t count;

	char sign;
	char prev_sign = '+';
	uint8_t value;

	char expr[16] = {'\0'};
	uint8_t cursor_pos = 0;

	while (true) {

		if (expr[0] == '*' || expr[0] == '/' || expr[0] == '+' || expr[0] == '-') {
			sprintf(buffer, "%c", sign);
			lcd_write_str(buffer);
		}

		// энкодер
		curr_a = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
		curr_b = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);

		degree += truth_mas[get_mas_index(curr_a, curr_b, prev_a, prev_b)]*15;

		if (degree >= 720) degree = 0;
		if (degree <= 0) degree = 0;
		if (prev_degree != degree) {
			if (degree < 180) sign = '+';
			if (degree > 180 && degree < 360) sign = '-';
			if (degree > 360 && degree < 540) sign = '*';
			if (degree > 540 && degree < 720) sign = '/';
			prev_degree = degree;
			if (prev_sign != sign) {
				if (expr[cursor_pos-1] == '+' || expr[cursor_pos-1] == '-' || expr[cursor_pos-1] == '/' || expr[cursor_pos-1] == '*') {
					expr[cursor_pos-1] = sign;
					lcd_set_cursor(0, cursor_pos-1);
					sprintf(buffer, "%c", sign);
					lcd_write_str(buffer);
				} else {
					expr[cursor_pos] = sign;
//					lcd_set_cursor(0, cursor_pos);
					sprintf(buffer, "%c", sign);
					lcd_write_str(buffer);
					cursor_pos += 1;
//					count = sprintf(buffer, "cursor_pos = %d\r\n", cursor_pos);
//					usart_send(buffer, count);
				}
//				lcd_set_cursor(0, cursor_pos);
//				sprintf(buffer, "%c", sign);
//				lcd_write_str(buffer);
				prev_sign = sign;
			}
		}
		prev_a = curr_a;
		prev_b = curr_b;


		// Устанавливаем HIGH на все ROW
		GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);

		for (j = 1; j < 4; j++) {//COL
			switch(j) {
				case 1:
					pin = GPIO_Pin_7;
					break;
				case 2:
					pin = GPIO_Pin_6;
					break;
				default:
					pin = GPIO_Pin_0;
					break;
			}
			if (!GPIO_ReadInputDataBit(GPIOB, pin)) {
//				clear();
				for (i = 1; i < 5; i++) {//ROW
					GPIO_WriteBit(GPIOB, GPIO_Pin_14, i == 4 ? Bit_RESET : Bit_SET);
					GPIO_WriteBit(GPIOB, GPIO_Pin_13, i == 3 ? Bit_RESET : Bit_SET);
					GPIO_WriteBit(GPIOB, GPIO_Pin_15, i == 2 ? Bit_RESET : Bit_SET);
					GPIO_WriteBit(GPIOB, GPIO_Pin_12, i == 1 ? Bit_RESET : Bit_SET);

					delay();
					if (!GPIO_ReadInputDataBit(GPIOB, pin)) {
						data = keyboard[i-1][j-1];

						if (data == 41) {
							lcd_set_cursor(0, 0); //clear
							cursor_pos = 0;
							lcd_write_cmd(0b00000001);
							delay();
							break;
						}

						if (data == '=') {
//							expr[cursor_pos] = data;
//							cursor_pos += 1;
//							if (expr[cursor_pos] == '+' || expr[cursor_pos] == '-' || expr[cursor_pos] == '/' || expr[cursor_pos] == '*') {
//								lcd_write_str("=");
//								delay();
//								data = calculate(expr);
//								break;
//							}
							cursor_pos = 0;
							lcd_write_cmd(0b00000001);
							sprintf(buffer, "%d", calculate(expr));
							lcd_write_str(buffer);
							break;
						}
						expr[cursor_pos] = data+48;
						cursor_pos += 1;

						sprintf(buffer, "%d", data);

						lcd_write_str(buffer);
						delay();
						break;
					}
				}
			}
		}
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
