#include "stdint.h"
#include "stdbool.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "avr/wdt.h"
#include "util/delay.h"
#include "basic.h"
#include "usart.h"

#define FOSC 1843200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

volatile bool has_recieved = false;
volatile bool is_in_progress = false;
volatile uint8_t data = 0;

// slave
int main() {
    SREG |= 0x80;
    usart_init(MYUBRR);
    usart_enable_rx_interrupt();
    DDRD |= (1<< PORTD4);

    for(;;) {
        if (has_recieved && !is_in_progress) {
            //_delay_ms(100);
            write_pin(&PORTD, PORTD4, 1);
            usart_transmit(data+2);
            is_in_progress =true;
        }
        wdt_reset();        
    }
}

ISR(USART_RX_vect) {
    has_recieved = true; 
    data = UDR0;       
}

ISR(USART_TX_vect) {
    write_pin(&PORTD, PORTD4, 0);
    has_recieved = false;
    is_in_progress = false;
    data = 0;
    UCSR0B &= ~(1<<TXEN0);
}
