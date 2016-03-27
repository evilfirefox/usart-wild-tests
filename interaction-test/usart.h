#ifndef _USART_H_
#define _USART_H_

#include "stdbool.h"
#include "avr/io.h"
#include "basic.h"

void usart_init(unsigned int ubrr);

void usart_transmit(unsigned char data);

unsigned char usart_recieve(void);

void usart_switch_rx_interrupt(bool state);

void usart_switch_tx_interrupt(bool state);

#endif
