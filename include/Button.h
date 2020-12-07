#ifndef BUTTON_H_
#define BUTTON_H_

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

//int getRowNumber(int pinC);

extern uint16_t pin_col;

void button_matrix_init(void);
uint8_t get_value(void);

#endif
