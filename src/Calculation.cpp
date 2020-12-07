#include "Calculation.h"
#include "USART.h"

//uint8_t i;
char buffer1[50] = {'\0'};

int calculate(char* expr) {
	int a = -99, b = -99;
	uint8_t count;
	uint8_t des = 1;
	uint8_t pos_sign;
	int8_t result = 0;
	char sign;
	for (uint8_t i = 0; i < 16; i++) {
		if (expr[i] >= 48 && expr[i] <= 57) {
//			count = sprintf(buffer1, "%d", expr[i]-48);
//			usart_send(buffer1, count);
			if (expr[i] == 0) {

			}else{
			if (a == -99) {
				a = (expr[i]-48);
				des *= 10;
			} else {
				a = a*des + (expr[i]-48);
				des *= 10;
			}}
			count = sprintf(buffer1, "%d\r\n", a);
			usart_send(buffer1, count);
		} else if (expr[i] >= 42 && expr[i] <= 47){
//			count = sprintf(buffer1, "%c", expr[i]);
//						usart_send(buffer1, count);
			sign = expr[i];
			pos_sign = i;
			break;
		}
	}

	count = sprintf(buffer1, "a = %d\r\n", a);
	usart_send(buffer1, count);
	count = sprintf(buffer1, "sign = %c\r\n", sign);
	usart_send(buffer1, count);
	des = 1;
	for (uint8_t i = pos_sign+1; i < 16; i++) {
		if (expr[i] >= 48 && expr[i] <= 57) {
//			count = sprintf(buffer1, "%d", expr[i]-48);
//						usart_send(buffer1, count);
			if (b == -99) {
				b = (expr[i]-48);
				des *= 10;
			} else {
				b = b*des + (expr[i]-48);
				des *= 10;
			}
		}
//else if (expr[i] >= 42 && expr[i] <= 47){
//			sign = expr[i];
//			pos_sign = i;
//			break;
//		}
	}

	count = sprintf(buffer1, "b = %d\r\n", b);
	usart_send(buffer1, count);

	switch (sign) {
		case '+':
			result = a+b;
			break;
		case '-':
			result = a-b;
			break;
		case '/':
			if (b == 0) {
				return 0;
			}
			result = a/b;
			break;
		case '*':
			result = a*b;
			break;
		default:
			break;
	}
	count = sprintf(buffer1, "= %d\r\n", result);
	usart_send(buffer1, count);
	return result;
}
