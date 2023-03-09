#include "bno055.h"

BNO055::BNO055(Adafruit_BNO055 *p) {
    sensorPtr = p;
}

void BNO055::init(void) {
    sensorPtr->begin();

    sensor_t sensor;
    sensorPtr->getSensor(&sensor);

    uint8_t system_status, self_test_results, system_error;
    system_status = self_test_results = system_error = 0;
    sensorPtr->getSystemStatus(&system_status, &self_test_results,
                               &system_error);

    sensorPtr->setExtCrystalUse(true);
}

int BNO055::read(void) {
    sensors_event_t event;
    sensorPtr->getEvent(&event);
    if (isGyroDisabled) {
        magnetic = event.magnetic.x;
        deg = (int)(magnetic - offset + 360) % 360;
    } else {
        deg = event.orientation.x;
    }

    return deg;
}