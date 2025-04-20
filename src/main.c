#define F_CPU 16000000UL // 16 MHz Crystal

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "button.h"
#include "uart.h"

int main(void) {
  uart_init(115200, UART_HIGH_SPEED_ACTIVE);

  button_init();

  sei();

  char c[16];
  uint8_t b = BTN_PRESSED_NONE;
  uint8_t i = 0;

  while (1) {
    b = pressed_button();
    switch (b) {
    case BTN_PRESSED_0:
      i++;
      break;
    case BTN_PRESSED_1:
      i--;
      break;
    case BTN_PRESSED_NONE:
      break;
    }
    _delay_ms(10);
  }
}
