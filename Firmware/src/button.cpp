#include "button.h"

ButtonDriver::ButtonDriver() {}

void ButtonDriver::begin(void (*ISR_callback)()) {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR_callback, FALLING);
}