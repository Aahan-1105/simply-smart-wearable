#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

// Assign to a free GPIO on your nice!nano v2 (e.g., Pin 5 / P0.15)
#define BUTTON_PIN 5            
#define DEBOUNCE_DELAY_MS 50    

class ButtonDriver {
public:
    ButtonDriver();
    void begin(void (*ISR_callback)());
};

#endif