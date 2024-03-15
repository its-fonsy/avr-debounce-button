#ifndef _USART_H
#define _USART_H

/* defines */
#define	BAUDRATE		9600
#define	BRC			((F_CPU / 16 / BAUDRATE) - 1)
#define TX_BUFF_SIZE		128

/* uart functions */
void append_serial(char c);
void serial_write(char c[]);
void usart_init();

#endif // _USART_H
