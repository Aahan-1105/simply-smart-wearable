#ifndef STORAGE_H
#define STORAGE_H

#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

using namespace Adafruit_LittleFS_Namespace;

class StorageManager {
public:
    StorageManager();
    bool begin();
    uint32_t loadStepCount();
    void saveStepCount(uint32_t steps);
};

#endif