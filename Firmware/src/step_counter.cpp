#include "step_counter.h"

StepCounter::StepCounter() {}

bool StepCounter::beign() {
    bmi160.begin(BMI160GenClass::I2C_MODE, BMI160_I2C_ADDR);

    if (bmi160.getDeviceID() != 0xD1) {
        return false;
    }

    bmi160.setStepDetectionMode(MBI160_STEP_MODE_NORMAL);
    bmi160.setStepCountEnabled(true);

    return true;
}

uint16_t StepCounter::getSteps() {
    return bmi160.getStepCount();
}

void StepCounter::reset() {
    bmi160.resetStepCount();
}

coid StepCounter::setupInterrupt(void (*ISR_callback)()) {
    pinMode(BMI160_INT1_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(BMI160_INT1_PIN), ISR_callback, RISING);

    bmi160.setIntStepEnabled(true);
}