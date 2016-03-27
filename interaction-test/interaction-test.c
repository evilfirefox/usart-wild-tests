#include "stdint.h"
#include "stdbool.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "avr/wdt.h"
#include "stdlib.h"

#define F_CPU 8
#include "util/delay.h"

#include "basic.h"
#include "usart.h"
#include "protocol.h"

#define FOSC 8000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
#define IO_BUFFER_SIZE 40

volatile uint8_t has_recieved = 0;
volatile bool is_in_progress = false;
uint8_t *io_buffer;
volatile uint8_t current_pos = -1;
uint8_t *data = (uint8_t[4]) {1,2,3,4};

volatile bool is_packet_complete = false;

void init() {
    SREG |= 0x80;
    io_buffer = (uint8_t*) malloc(IO_BUFFER_SIZE * sizeof(uint8_t));
    usart_init(MYUBRR);
    usart_switch_rx_interrupt(ON);
    usart_switch_tx_interrupt(ON);
}

// master
int main() {
    init();   
    proto_send(0x10, 0x15, data);
    for(;;) {
        if (is_packet_complete) {
            proto_recieve(io_buffer);
            current_pos = -1;
        }
        wdt_reset();        
    }
}

ISR(USART_RX_vect) {
    has_recieved = 1;
    current_pos++;
    if (current_pos > IO_BUFFER_SIZE) {
        current_pos = 0;
    }
    if (current_pos == IO_PACKET_SIZE) {
        is_packet_complete = true;
    }
    *(io_buffer+current_pos) = UDR0;
}


ISR(USART_TX_vect) {    
}
