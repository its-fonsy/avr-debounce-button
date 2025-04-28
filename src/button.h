#ifndef __BUTTON_H
#define __BUTTON_H

#include <stdint.h>

enum {
    BUTTON_IDLE,
    BUTTON_PRESSED,
    BUTTON_PRESS_ACK
};

enum {
    BUTTON_LOCKED,
    BUTTON_UNLOCKED
};

typedef struct {
    uint8_t (*read_pin)();
    uint8_t history;
    uint8_t status;
    uint8_t lock;
} button_t;

void button_init(button_t* button);
void button_update(button_t* button);

#endif // __BUTTON_H
