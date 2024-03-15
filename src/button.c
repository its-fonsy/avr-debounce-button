#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "button.h"
#include "usart.h"

uint8_t db_pre_s = 0;	/* Debounced pressed state */
uint8_t db_rel_s = 0;	/* Debounced release state */

void button_init()
{
	/* Set pin 13 as output, use pull-up resistor for pin 8 and 9 */

	DDRB = (1 << PINB5);
	PORTB = (1 << PINB0) | (1 << PINB1);

	/* Configure Timer 1 */

	OCR1A = 78;
	TIMSK1 = (1<<OCIE1A);				/* Enable compare match interrupt */
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);	/* Configure Timer1 prescaler and CTC mode */
}

uint8_t pressed_button()
{
	static uint8_t state = BTN_WAIT4PRESS;

	if ((state == BTN_WAIT4PRESS) && (db_pre_s != 0))
	{
		state = BTN_WAIT4RELEASE;
		if(db_pre_s == 1)
			return BTN_PRESSED_0;
		else
			return BTN_PRESSED_1;

	}
	else if((state == BTN_WAIT4RELEASE) && (db_rel_s == 0))
	{
		state = BTN_WAIT4PRESS;
	}

	return BTN_PRESSED_NONE;
}

ISR(TIMER1_COMPA_vect)
{
	static uint8_t bounce_history[DEBOUNCE_N_CHECKS] = { 0 };
	static uint8_t h_idx = 0;
	uint8_t i;
	uint8_t raw;
	uint8_t db_pres;
	uint8_t db_rel;

	raw = PINB;
	bounce_history[h_idx] = ~raw & BTN_PIN_MASK;

	h_idx++;
	if(h_idx >= DEBOUNCE_N_CHECKS)
	{
		h_idx = 0;
	}

	db_pres = 0xFF;
	db_rel = 0x00;
	for(i = 0; i < DEBOUNCE_N_CHECKS; i++)
	{
		db_pres &= bounce_history[i];
		db_rel |= bounce_history[i];
	}

	db_pre_s = db_pres;
	db_rel_s = db_rel;
}
