# Debounce button

This is an implementation of the [Ganssle debounce
algorithm](https://www.ganssle.com/debouncing-pt2.htm). It rely on a timer that
triggers an interrupt every 5ms and updates the status of the button.

This implementation aims to be platform agnostic while maintaining the
simplicity of the original algorithm.

The `src/button.c` and `src/button.h` are the core implementation of the
library, meanwhile `src/main.c` is an example of its usage via an AVR
microcontroller.

## Notes about the example

The microcontroller used in the example is the ATmega328p, and the programmer
is Arduino. The `Makefile` can be used to test the example.

Clone the repository

    git clone git@github.com:its-fonsy/debounce-button.git
    cd debounce-button

then build and flash the microcontroller

    make flash

The example simply toggle the status of a led when the button is pressed.

### Timer configuration

The timer is configured like so

- Prescaler set to 1024;
- Trigger interrupt when reach OCIE1A;

Aiming for an interrupt every 5ms

```math
\text{OC}   = \frac{T_{INT}}{T_{CPU} \cdot \text{PRESCALER}}
            = \frac{T_{INT} \cdot f_{CPU}}{\text{PRESCALER}}
            = \frac{5 \cdot 10^{-3} \cdot 16 \cdot 10^6}{1024}
            = 78.125 \rightarrow 78
```
 
Precisely, the interrupt will be triggered

```math
T_{INT} = \frac{\text{OC} \cdot \text{PRESCALER}}{F_{CPU}} = \frac{78 \cdot 1024}{16 \cdot 10^6} = 4.992\,ms 
```
