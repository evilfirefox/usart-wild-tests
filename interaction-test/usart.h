#include "avr/io.h"

void usart_init(unsigned int ubrr);

void usart_transmit(unsigned char data);

unsigned char usart_recieve(void);

void usart_enable_rx_interrupt(void);
