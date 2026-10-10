#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include "display.h"
#include "step_counter.h"

// put function declarations here:
int myFunction(int, int);

void setup() {
  Serial.begin(115200);
  // Wait for USB if plugged in
  uint32_t timeout = millis();
  while (!Serial && (millis() - timeout < 3000)) {
    delay(10);
  }

  Serial.println("simply_wearable initialized");
}

void loop() {
  Serial.println("Heartbeat...");
}
