#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "stdlib.h"
#include "usart.h"
#include "rs485.h"

#define IO_PACKET_SIZE 18

void proto_send(uint8_t recipient, uint8_t command, uint8_t *data);

void proto_recieve(volatile uint8_t *buffer);

#endif
