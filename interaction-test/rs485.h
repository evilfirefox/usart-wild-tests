#ifndef _RS485_H_
#define _RS485_H_

#include "stdbool.h"
#include "basic.h"
#include "usart.h"
#include "util/delay.h"

void rs485_init(void);

void rs485_switch_transmitter(bool is_on);

void rs485_transmit(uint8_t data);

#endif
