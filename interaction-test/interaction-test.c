#include "stdint.h"
#include "stdbool.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "avr/wdt.h"
#include "usart.h"

#define FOSC 1843200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

volatile uint8_t has_recieved = 0;
volatile bool is_in_progress = false;
volatile uint8_t data = 0;

// master
int main() {
    SREG |= 0x80;
    usart_init(MYUBRR);
    usart_enable_rx_interrupt();
    usart_transmit(33);
    for(;;) {
        if (has_recieved > 0 && !is_in_progress) {            
            usart_transmit(data + 1);
            is_in_progress = true;
        }
        wdt_reset();        
    }
}

ISR(USART_RX_vect) {
    has_recieved = 1;    
    data = UDR0;    
}


ISR(USART_TX_vect) {
    data = 0;
    has_recieved = 0;
    is_in_progress = false;
    UCSR0B &= ~(1<<TXEN0);
}
