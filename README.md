# Debounce two button on AVR

This is my implementation of the [Ganssle debounce
algorithm](https://www.ganssle.com/debouncing-pt2.htm). It basically rely on a
timer that triggers an interrupt every 5ms and store the history of the
switches and return its debounced state. In its barebone implementation when
reading the debounced state inside the main loop the switch seems to be pressed
multiple time when it actually is being pressed once. This is due to the MCU
being to fast checking the debounced state. I added some code that signals when
a switch is pressed and then wait for its release to signal another press.

## Timer configuration

The timer is configured like so

- Prescaler of 1024
- Trigger interrupt when reach OCIE1A

Since I want an interrupt every 5ms so

```math
\text{OC}   = \frac{T_{INT}}{T_{CPU} \cdot \text{PRESCALER}}
            = \frac{T_{INT} \cdot f_{CPU}}{\text{PRESCALER}}
            = \frac{5 \cdot 10^{-3} \cdot 16 \cdot 10^6}{1024}
            = 78.125 \rightarrow 78
```
 
Precisely the interrupt will be triggered

```math
T_{INT} = \frac{\text{OC} \cdot \text{PRESCALER}}{F_{CPU}} = \frac{78 \cdot 1024}{16 \cdot 10^6} = 4.992\,ms 
```
