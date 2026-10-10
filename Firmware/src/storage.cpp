#include "storage.h"

#define STEP_FILE_PATH "/steps.bin"

StorageManager::StorageManager() {}

bool StorageManager::begin() {
    return InternalFS.begin();
}

uint32_t StorageManager::loadStepCount() {
    if (!InternalFS.exists(STEP_FILE_PATH)) {
        return 0;
    }

    File file(InternalFS);
    if (file.open(STEP_FILE_PATH, FILE_O_READ)) {
        uint32_t steps = 0;
        file.read(&steps, sizeof(steps));
        file.close();
        return steps;
    }

    return 0;
}

void StorageManager::saveStepCount(uint32_t steps) {
    // Overwrite existing persistent step record
    InternalFS.remove(STEP_FILE_PATH);

    File file(InternalFS);
    if (file.open(STEP_FILE_PATH, FILE_O_WRITE)) {
        file.write((uint8_t*)&steps, sizeof(steps));
        file.close();
    }
}