#include "VL53L0X.h"

VL53L0X::VL53L0X() {
    serialPtr = NULL;
}

int VL53L0X::getDistance(void) {
    if (serialPtr->available() >= 24) {
        for (int i = 0; i < 12; i++) {
            distance[i] = (serialPtr->read() << 8) | serialPtr->read();
        }

        return 0;

    } else {
        return 1;
    }
}