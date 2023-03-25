#include "./unitV.h"
#include "./VL53L0X.h"

extern HardwareSerial uart1;
extern HardwareSerial uart3;
extern VL53L0X distanceSensor;

UNITV::UNITV(HardwareSerial *ptr) {
    serialPtr = ptr;
    serialPtr->begin(115200);
}

void UNITV::read(void) {
    if (serialPtr->available() > 0) {
        // this->data = serialPtr->read();
        int temp = serialPtr->read();

        if (temp == 'H' || temp == 'S' || temp == 'U' || temp == 'Y' ||
            temp == 'R' || temp == 'G' || temp == 'N') {
                this->data = temp;
        }

        while (serialPtr->available() > 0) {
            serialPtr->read();
        }
    }
}