#include "./unitV.h"
#include "./VL53L0X.h"

extern HardwareSerial uart1;
extern VL53L0X distanceSensor;

UNITV::UNITV(HardwareSerial *ptr) {
    serialPtr = ptr;
    serialPtr->begin(115200);
}

void UNITV::read(void) {
}