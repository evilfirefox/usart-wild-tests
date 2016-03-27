#include "rs485.h"

void rs485_init(void) {
    DDRD |= (1 << PORTD4); // control pin
    rs485_switch_transmitter(ON);
}

void rs485_switch_transmitter(bool is_on) {
    write_pin(&PORTD, PORTD4, (uint8_t) is_on);
}

void rs485_transmit(uint8_t data) {
    rs485_switch_transmitter(ON);
    usart_transmit(data);
    rs485_switch_transmitter(OFF);
}
