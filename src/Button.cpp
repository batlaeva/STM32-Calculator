#include "Button.h"

uint16_t pin_col;
uint16_t pin;
uint8_t data;
uint8_t i,j;
uint8_t cursor_pos = 0;
uint8_t cursor_line = 0;

//uint8_t keyboard[4][3] = {
//		{0,61,41},
//		{3,6,9},
//		{2,5,8},
//		{1,4,7}};

void button_matrix_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	//ROW button matrix 4,3,2,1
	GPIO_InitTypeDef initStruct;
	initStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_15 | GPIO_Pin_12;
	initStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	initStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &initStruct);

	//COL button matrix 1,2,3
	initStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_0;
	initStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &initStruct);
}

//void delay(void) {
//	volatile uint32_t i;
//	for (i = 0; i <= 0x80000; i++);
//}

//void clear() {
//	for (unsigned int i = 0; i < sizeof(buffer) - 2; i++) {
//		buffer[i] = ' ';
//	}
//
//	buffer[sizeof(buffer) - 2] = '\r';
//	buffer[sizeof(buffer) - 1] = '\n';
//}

uint8_t get_value(void) {
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
//			clear();
			for (i = 1; i < 5; i++) {//ROW
				GPIO_WriteBit(GPIOB, GPIO_Pin_14, i == 4 ? Bit_RESET : Bit_SET);
				GPIO_WriteBit(GPIOB, GPIO_Pin_13, i == 3 ? Bit_RESET : Bit_SET);
				GPIO_WriteBit(GPIOB, GPIO_Pin_15, i == 2 ? Bit_RESET : Bit_SET);
				GPIO_WriteBit(GPIOB, GPIO_Pin_12, i == 1 ? Bit_RESET : Bit_SET);

//				delay();
				if (!GPIO_ReadInputDataBit(GPIOB, pin)) {
//					data = keyboard[i-1][j-1];

					cursor_pos += 1;

//						if (cursor_pos == 0 && data == '')

//						if (cursor_pos == 16 && cursor_line == 0) {
//							cursor_line = 1;
//							lcd_set_cursor(cursor_line, cursor_pos-16);
//						}
//						if (cursor_pos == 16 && cursor_line == 1) {
//							cursor_line = 0;
//							lcd_set_cursor(cursor_line, cursor_pos-16);
//						}

//					if (data == 41) {//mc
////							lcd_set_cursor(cursor_line, cursor_pos-1); //clear
////						lcd_write_cmd(0b00000001);
//						delay();
//						return data;
//					}

//					if (data == 61) {
//						lcd_write_str("=");
//						delay();
//						return data;
//					}
//					lcd_write_str(buffer);
//					delay();
					return data;
				}
			}
		}
	}

}

//	return row_col;
//}

//int main(void) {
//
////	uint16_t pin_col;
//
//	while (true) {
//		// Устанавливаем HIGH на все ROW
//		GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_RESET);
//		GPIO_WriteBit(GPIOB, GPIO_Pin_13, Bit_RESET);
//		GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_RESET);
//		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
//
//		for (int j = 1; j < 4; j++) {//COL
//			switch(j) {
//				case 1:
//					pin_col = GPIO_Pin_7;
//					break;
//				case 2:
//					pin_col = GPIO_Pin_6;
//					break;
//				default:
//					pin_col = GPIO_Pin_0;
//					break;
//			}
//			if (!GPIO_ReadInputDataBit(GPIOB, pin_col)) {
//				clear();
//				for (int i = 1; i < 5; i++) {//ROW
//					GPIO_WriteBit(GPIOB, GPIO_Pin_14, i == 4 ? Bit_RESET : Bit_SET);
//					GPIO_WriteBit(GPIOB, GPIO_Pin_13, i == 3 ? Bit_RESET : Bit_SET);
//					GPIO_WriteBit(GPIOB, GPIO_Pin_15, i == 2 ? Bit_RESET : Bit_SET);
//					GPIO_WriteBit(GPIOB, GPIO_Pin_12, i == 1 ? Bit_RESET : Bit_SET);
//
//					if (!GPIO_ReadInputDataBit(GPIOB, pin_col)) {
//						sprintf(buffer, "row: %d, col: %d\r\n", i, j);
//						usart_send(buffer, sizeof(buffer));
//						break;
//					}
//				}
//			}
//		}
//
//		delay();
//	}
//}
