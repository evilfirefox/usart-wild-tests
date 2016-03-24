#include "avr/io.h"

void write_pin(volatile uint8_t *port, uint8_t pin, uint8_t value);

uint8_t read_pin(uint8_t port, uint8_t pin);
