#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define OLED_I2C_ADDRESS 0x3C

class DisplayManager {
private:
    Adafruit_SSD1306 display;

public:
    DisplayManager();
    bool begin();
    void renderStepCount(uint32_t steps);
    void showMessage(const char* msg);
    void turnOff();
    void turnOn();
};

#endif