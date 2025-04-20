#ifndef _BUTTON_H
#define _BUTTON_H

#define	DEBOUNCE_N_CHECKS	7
#define	BTN_PIN_MASK		0x03

enum {
	BTN_WAIT4PRESS = 0,
	BTN_WAIT4RELEASE,
	BTN_PRESSED_0,
	BTN_PRESSED_1,
	BTN_PRESSED_NONE,
};

void button_init();
uint8_t pressed_button();

#endif // _USART_H
