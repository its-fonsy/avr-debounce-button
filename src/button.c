#include "button.h"

void button_init(button_t* button)
{
    button->history = 0xFF;
    button->status = BUTTON_IDLE;
    button->lock = BUTTON_UNLOCKED;
}

void button_update(button_t* button)
{
    if (button->lock == BUTTON_LOCKED || button->status == BUTTON_PRESSED)
        return;

    button->lock = BUTTON_LOCKED;

    button->history <<= 1;
    button->history |= button->read_pin();

    if (button->status == BUTTON_IDLE && button->history == 0x00)
        button->status = BUTTON_PRESSED;
    else if (button->status == BUTTON_PRESS_ACK && button->history == 0xFF)
        button->status = BUTTON_IDLE;

    button->lock = BUTTON_UNLOCKED;
}
