#include "protocol.h"

void proto_send(uint8_t recipient, uint8_t command, uint8_t *data) {
    uint8_t *buffer = malloc(3*sizeof(uint8_t)+sizeof(*data));
    *(buffer) = recipient;
    *(buffer+1) = command;
    *(buffer+2) = sizeof(*data);
    int i=0;
    while (*(data+i)) {
        *(buffer+3+i) = *(data+i);
        i++;
    }
    i = 0;
    while (*(buffer+i)) {
        rs485_transmit(*(buffer+i));        
        i++;
    }
}

void proto_recieve(volatile uint8_t *buffer) {
    
}
