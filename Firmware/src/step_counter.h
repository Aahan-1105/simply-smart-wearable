#ifndef STEP_COUNTER_H
#define STEP_COUNTER_H

#include <BMI160Gen.h>
#include <Wire.h>

#define BMI160_I2C_ADDR 0x68
#define BMI160_INT1_PIN 6  // Hardware interrupt pin

#define STEP_WAKE_THRESHOLD 100 // Wake main loop every 100 steps

class StepCounter {
private:
    BMI160GenClass bmi160;

public:
    StepCounter();
    bool begin();
    uint16_t getSteps();
    void reset();
    void setupInterrupt(void (*ISR_callback)());
};

#endif