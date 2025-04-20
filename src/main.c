#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "button.h"
#include "hal.h"
#include "uart.h"

button_t btn;

int main(void)
{
    set_pin(DDRB, PB5);

    btn.port_reg = &PORTB;
    btn.dir_reg = &DDRB;
    btn.pin_reg = &PINB;
    btn.pin = PB4;

    uart_init(115200, UART_HIGH_SPEED_ACTIVE);
    button_init(&btn);

    /* Configure Timer 1
     *   - PRESCALER = 1024
     *   - MODE = CTC
     *   - INTERRUPT = On match
     */
    OCR1A = 1;
    TIMSK1 = (1 << OCIE1A);
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);

    sei();

    while (1) {

        btn.lock = BUTTON_LOCKED;
        if (btn.status == BUTTON_PRESSED) {
            toggle_pin(PORTB, PB5);
            btn.status = BUTTON_PRESS_ACK;
        }
        btn.lock = BUTTON_UNLOCKED;

        _delay_ms(5);
    }
}

ISR(TIMER1_COMPA_vect)
{
    button_update(&btn);
}
