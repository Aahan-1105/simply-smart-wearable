#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include "display.h"
#include "step_counter.h"
#include "button.h"
#include "storage.h"

#define DISPLAY_TIMEOUT_MS  5000         // 5 seconds auto-sleep
#define SAVE_INTERVAL_MS    (15 * 60 * 1000) // 15 minutes between flash writes
#define SAVE_STEP_DELTA     100          // Must have at least 100 new steps to trigger save

DisplayManager display;
StepCounter stepCounter;
ButtonDriver userButton;
StorageManager storage;

volatile bool stepDetected = false;
volatile bool buttonPressed = false;

volatile uint16_t stepPulseCount = 0;
volatile uint32_t lastButtonInterruptTime = 0;

bool displayIsOn = true;
uint32_t displayWakeTimestamp = 0;

uint32_t persistentSteps = 0;     // Steps loaded from flash at boot
uint32_t lastSavedTotalSteps = 0; // Total steps at the last flash write
uint32_t lastSaveTimestamp = 0;   // millis() timestamp of last flash write

void handleStepISR() {
    stepPulseCount++;
    if (stepPulseCount >= STEP_WAKE_THRESHOLD) {
        stepPulseCount = 0;
        stepDetected = true;
    }
}

void handleButtonISR() {
    uint32_t currentTime = millis();
    if (currentTime - lastButtonInterruptTime > DEBOUNCE_DELAY_MS) {
        buttonPressed = true;
        lastButtonInterruptTime = currentTime;
    }
}

void wakeDisplay() {
    if (!displayIsOn) {
        display.turnOn();
        displayIsOn = true;
    }
    uint32_t totalSteps = persistentSteps + stepCounter.getSteps();
    display.renderStepCount(totalSteps);
    displayWakeTimestamp = millis();
}

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("==================================");
    Serial.println(" Wearable Step Counter Initialized ");
    Serial.println("==================================");

    // 1. Initialize Storage & Load Saved Steps
    if (!storage.begin()) {
        Serial.println("[ERROR] LittleFS Flash Init Failed!");
    } else {
        persistentSteps = storage.loadStepCount();
        lastSavedTotalSteps = persistentSteps;
        lastSaveTimestamp = millis();
        Serial.print("[OK] Flash Storage Ready. Loaded Steps: ");
        Serial.println(persistentSteps);
    }

    // 2. Initialize Display
    if (!display.begin()) {
        Serial.println("[ERROR] SSD1306 OLED init failed!");
    } else {
        Serial.println("[OK] SSD1306 OLED Ready");
        display.showMessage("Booting...");
    }

    // 3. Initialize IMU Step Engine
    if (!stepCounter.begin()) {
        Serial.println("[ERROR] BMI160 IMU init failed!");
        display.showMessage("IMU Error!");
    } else {
        Serial.println("[OK] BMI160 Step Detector Ready");
        stepCounter.setupInterrupt(handleStepISR);
        display.showMessage("Sensor Ready!");
    }

    // 4. Initialize Button
    userButton.begin(handleButtonISR);

    delay(1000);
    wakeDisplay();
}

void loop() {
    uint32_t currentTotalSteps = persistentSteps + stepCounter.getSteps();

    // 1. Process 100-step batch event
    if (stepDetected) {
        stepDetected = false;
        Serial.print("100-Step Event Triggered! Total: ");
        Serial.println(currentTotalSteps);

        wakeDisplay();
    }

    // 2. Process manual button press
    if (buttonPressed) {
        buttonPressed = false;
        Serial.println("Button Pressed: Waking Display");

        wakeDisplay();
    }

    // 3. Auto-sleep timer evaluation & conditional periodic save
    if (displayIsOn && (millis() - displayWakeTimestamp >= DISPLAY_TIMEOUT_MS)) {
        
        // Only write to flash if 15+ mins have passed AND we have 100+ new steps
        bool timeElapsed = (millis() - lastSaveTimestamp >= SAVE_INTERVAL_MS);
        bool stepsAccumulated = ((currentTotalSteps - lastSavedTotalSteps) >= SAVE_STEP_DELTA);

        if (timeElapsed && stepsAccumulated) {
            Serial.println("Periodic Threshold Met: Writing to Flash...");
            storage.saveStepCount(currentTotalSteps);
            lastSavedTotalSteps = currentTotalSteps;
            lastSaveTimestamp = millis();
        }

        Serial.println("Display Timeout: Entering Sleep");
        display.turnOff();
        displayIsOn = false;
    }

    delay(100); 
}