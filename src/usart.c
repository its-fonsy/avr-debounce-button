#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "usart.h"

/* uart variables */
char buff[TX_BUFF_SIZE];
uint8_t read_idx = 0;
uint8_t write_idx = 0;

void usart_init()
{
	/* Configuring UART */
	UBRR0H = (BRC >> 8);
	UBRR0L = BRC;
	UCSR0B = (1 << TXEN0) | (1 << TXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void append_serial(char c)
{
	buff[write_idx] = c;
	write_idx++;

	if(write_idx >= TX_BUFF_SIZE)
		write_idx = 0;
}

void serial_write(char c[])
{
	uint8_t i;
	for(i = 0; i < strlen(c); i++)
		append_serial(c[i]);

	if(UCSR0A & (1 << UDRE0))
		UDR0 = 0;
}

ISR(USART_TX_vect)
{
	if(read_idx != write_idx)
	{
		UDR0 = buff[read_idx];
		read_idx++;

		if(read_idx >= TX_BUFF_SIZE)
			read_idx = 0;
	}
}
