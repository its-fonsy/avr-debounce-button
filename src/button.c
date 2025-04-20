#include <avr/io.h>

#include "button.h"
#include "hal.h"

void button_init(button_t* button)
{
    /* Set button pin as input */
    reset_pin(*(button->dir_reg), button->pin);

    /* Activate pull up resistor */
    set_pin(*(button->port_reg), button->pin);

    button->history = 0xFF;
    button->status = BUTTON_IDLE;
    button->lock = BUTTON_UNLOCKED;
}

void button_update(button_t* button)
{
    uint8_t pin_val;

    if (button->lock == BUTTON_LOCKED || button->status == BUTTON_PRESSED)
        return;

    button->lock = BUTTON_LOCKED;

    pin_val = status_pin(*(button->pin_reg), button->pin);
    button->history = (button->history << 1) | pin_val;

    switch (button->status) {
    case BUTTON_IDLE:
        if (button->history == 0xFF)
            button->status = BUTTON_PRESSED;
        break;
    case BUTTON_PRESS_ACK:
        if (button->history == 0x00)
            button->status = BUTTON_IDLE;
        break;
    }

    button->lock = BUTTON_UNLOCKED;
}
