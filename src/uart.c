#include "uart.h"
#include "hal.h"

#include <avr/io.h>
#include <stdarg.h>
#include <stdint.h>

void uart_init(uint32_t baud, uint8_t high_speed)
{

    uint8_t speed = 16;

    union {
        uint16_t value;
        struct {
            uint8_t low;
            uint8_t high;
        } byte;
    } baudrate;

    if (high_speed == UART_HIGH_SPEED_ACTIVE) {
        speed = 8;
        set_pin(UCSR0A, U2X0);
    }

    /* Compute the baudrate using the formula in the datasheet (p.146) */

    baudrate.value = (F_CPU / (speed * baud)) - 1;

    /* Set the baudrate register with the computed baudrate */

    UBRR0H = baudrate.byte.high;
    UBRR0L = baudrate.byte.low;

    /* Enable UART transmission */

    set_pin(UCSR0B, TXEN0);
}

void uart_putc(uint8_t data)
{
    /* Wait for UART to be available */

    while (pin_is_low(UCSR0A, UDRE0))
        ;

    /* Put data into buffer, sends the data */

    UDR0 = data;
}

void uart_printf(char* format, ...)
{
    char* traverse;
    uint32_t int_val;
    uint16_t len;
    uint8_t buff[8];
    char c;

    va_list ptr;
    va_start(ptr, format);

    for (traverse = format; *traverse != '\0'; traverse++) {

        /* Print all character until '%' */

        while (*traverse != '%') {

            /* Return if the string ends */

            if (*traverse == '\0') {
                va_end(ptr);
                return;
            }

            uart_putc(*traverse);
            traverse++;
        }

        traverse++;

        switch (*traverse) {
        case 'c':
            c = va_arg(ptr, int);
            uart_putc(c);
            break;
        case 'd':

            int_val = va_arg(ptr, int);

            /* Fill the buffer with the digits of the integer.
             * Example: int_val = 123 -> buff = {3, 2, 1}
             * */

            len = 0;
            while (int_val != 0) {
                buff[len++] = int_val % 10;
                int_val /= 10;
            }

            /* If length is 0 it means that the int_val is 0 */

            if (len == 0) {
                uart_putc(integer_to_char(0));
                break;
            }

            /* Print the buffer from last to first */

            while (len != 0)
                uart_putc(integer_to_char(buff[--len]));

            break;
        }
    }

    va_end(ptr);
}
