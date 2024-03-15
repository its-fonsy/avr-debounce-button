#define F_CPU 16000000UL // 16 MHz Crystal

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>

#include "usart.h"
#include "button.h"

int main(void)
{
	usart_init();
	button_init();

	sei();

	char c[16];
	uint8_t b = BTN_PRESSED_NONE;
	uint8_t i = 0;

	while(1)
	{
		b = pressed_button();
		switch(b)
		{
			case BTN_PRESSED_0:
				i++;
				utoa(i, c, 10);
				strcat(c, "\r\n");
				serial_write(c);
				break;
			case BTN_PRESSED_1:
				i--;
				utoa(i, c, 10);
				strcat(c, "\r\n");
				serial_write(c);
				break;
			case BTN_PRESSED_NONE:
				break;
		}
		_delay_ms(10);
	}
}
