#include "VL53L0X.h"

extern HardwareSerial uart1;

VL53L0X::VL53L0X(HardwareSerial *p) {
    serialPtr = p;
}

int VL53L0X::getDistance(void) {
    if (serialPtr->available() >= 27) {
        int checkDegit = 0;

        if (serialPtr->read() == 'V') {
            if (serialPtr->read() == 'L') {
                for (int i = 0; i < 12; i++) {
                    char data[2];
                    data[0] = serialPtr->read();
                    data[1] = serialPtr->read();

                    checkDegit += data[0] + data[1];

                    _valTemp[i] = (data[0] << 8) + data[1];
                }
            }
        }

        if (checkDegit % 256 == serialPtr->read()) {
            for (int i = 0; i < 12; i++) {
                val[i] = _valTemp[i];
            }
        }

        while (serialPtr->available() > 0) {
            serialPtr->read();
        }

        return 0;

    } else {
        return 1;
    }
}