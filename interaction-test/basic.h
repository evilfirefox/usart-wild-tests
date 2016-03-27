#ifndef _BASIC_H_
#define _BASIC_H_

#include "avr/io.h"

#ifndef ON 
#define ON true 
#endif

#ifndef OFF 
#define OFF false
#endif

void write_pin(volatile uint8_t *port, uint8_t pin, uint8_t value);

uint8_t read_pin(uint8_t port, uint8_t pin);

#endif
