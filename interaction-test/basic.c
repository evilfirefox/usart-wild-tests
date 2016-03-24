#include "basic.h"

void write_pin(volatile uint8_t *port, uint8_t pin, uint8_t value) {
    if (0 == value) {
        *port &= ~(1 << pin);
    } else {
        *port |= (1 << pin);
    }
}

uint8_t read_pin(uint8_t port, uint8_t pin) {
    return port && (1 << pin);
}
