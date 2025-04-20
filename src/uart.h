#ifndef __UART_H_
#define __UART_H_

#include <stdint.h>

#define RX_BUFFER_SIZE 128
#define integer_to_char(a) (a) + 48

enum {
    UART_HIGH_SPEED_DISABLE = 0,
    UART_HIGH_SPEED_ACTIVE = 0,
};

void uart_init(uint32_t baud, uint8_t high_speed);
void uart_putc(uint8_t data);
void uart_printf(char* str, ...);

#endif
