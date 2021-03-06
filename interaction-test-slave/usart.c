#include "usart.h"

void usart_init(unsigned int ubrr) {
    // baudrate
    UBRR0H = (unsigned char) (ubrr>>8);
    UBRR0L = (unsigned char) ubrr;
    // enable tx & rx
    UCSR0B |= (1<<RXEN0);
    // 8bit, 2 stops
    UCSR0C |= (1<<USBS0)|(3<<UCSZ00);
}

void usart_transmit(unsigned char data) {
    UCSR0B |= (1<<TXEN0);
    // waiting for buffer to empty
    while (!(UCSR0A & (1<<UDRE0)));
    // send data
    UDR0 = data;
}

void usart_enable_rx_interrupt(void) {
    UCSR0B |= (1<<RXCIE0) | (1<<TXCIE0);
}


