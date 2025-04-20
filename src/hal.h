#ifndef __HAL_H
#define __HAL_H

/* Output IO register operations */

#define set_pin(port, pin) ((port) |= (1 << (pin)))
#define reset_pin(port, pin) ((port) &= ~(1 << (pin)))
#define toggle_pin(port, pin) ((port) ^= (1 << (pin)))

/* Input IO register operations */

#define status_pin(port, pin) (((port) >> (pin)) & 0x01)
#define pin_is_high(port, pin) (status_pin((port), (pin)) == 0x01)
#define pin_is_low(port, pin) (status_pin((port), (pin)) == 0x00)

#endif
