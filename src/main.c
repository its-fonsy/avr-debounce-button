#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "button.h"

#define LED_PIN PB5
#define BUTTON_PIN PB4

void button_init_gpio();
void timer1_init();
uint8_t button_read_pin();

button_t btn;

int main(void)
{
    /* Set led pin as output and turn it off */
    DDRB |= _BV(LED_PIN);
    PORTB &= ~(_BV(LED_PIN));

    /* Configure and initialize button */
    btn.read_pin = &button_read_pin;
    button_init_gpio();
    button_init(&btn);

    /* Configure and start the timer */
    timer1_init();

    sei();

    while (1) {

        btn.lock = BUTTON_LOCKED;
        if (btn.status == BUTTON_PRESSED) {
            PORTB ^= _BV(LED_PIN);
            btn.status = BUTTON_PRESS_ACK;
        }
        btn.lock = BUTTON_UNLOCKED;

        _delay_ms(5);
    }
}

/* Configure Timer 1
 *   - PRESCALER = 1024
 *   - MODE = CTC
 *   - INTERRUPT = On match
 */
void timer1_init()
{
    OCR1A = 78;
    TIMSK1 = (1 << OCIE1A);
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
}

void button_init_gpio()
{
    /* Set pin as input */
    DDRB &= ~(_BV(BUTTON_PIN));

    /* Activate PULL-UP resistor */
    PORTB |= _BV(BUTTON_PIN);
}

uint8_t button_read_pin() { return bit_is_set(PINB, BUTTON_PIN) ? 0x01 : 0x00; }

ISR(TIMER1_COMPA_vect) { button_update(&btn); }
