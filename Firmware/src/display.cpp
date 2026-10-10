#include "display.h"

DisplayManager::DisplayManager() 
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

bool DisplayManager::begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
        return false;
    }
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();
    return true;
}

void DisplayManager::renderStepCount(uint32_t steps) {
    display.clearDisplay();

    // Header Label
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("DAILY STEPS");

    // Divider
    display.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    // Large Step Count
    display.setTextSize(2);
    display.setCursor(0, 15);
    display.print(steps);

    display.display();
}

void DisplayManager::showMessage(const char* msg) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 12);
    display.print(msg);
    display.display();
}

void DisplayManager::turnOff() {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
}

void DisplayManager::turnOn() {
    display.ssd1306_command(SSD1306_DISPLAYON);
}