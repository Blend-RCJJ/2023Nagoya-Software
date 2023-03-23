#include "bno055.h"

BNO055::BNO055(Adafruit_BNO055 *p) {
    sensorPtr = p;
}

void BNO055::init(void) {
    sensorPtr->begin();

    int eeAddress = 0;
    long bnoID;
    bool foundCalib = false;

    EEPROM.get(eeAddress, bnoID);

    adafruit_bno055_offsets_t calibrationData;
    sensor_t sensor;

    // sensorPtr->getSensor(&sensor);

    // if (bnoID != sensor.sensor_id) {
    //     // Serial.println(
    //     //     "\nNo Calibration Data for this sensor exists in EEPROM");
    //     // delay(500);
    //     // while(1){}
    // } else {
    //     // Serial.println("\nFound Calibration for this sensor in EEPROM.");
    //     eeAddress += sizeof(long);
    //     EEPROM.get(eeAddress, calibrationData);
    //     // displaySensorOffsets(calibrationData);
    //     // Serial.println("\n\nRestoring Calibration data to the BNO055...");
    //     sensorPtr->setSensorOffsets(calibrationData);
    //     // Serial.println("\n\nCalibration data loaded into BNO055");
    //     foundCalib = true;
    // }

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
        deg      = (int)(magnetic - offset + 360) % 360;
    } else {
        deg = (int)(event.orientation.x - offset + 360) % 360;
    }

    // if (abs(oldDeg - deg) > 30) {
    //     error = oldDeg - deg;
    // }

    // deg = (int)(deg + error + 360) % 360;

    // oldDeg = deg;

    slope = (int)(event.orientation.z - slopeOffset + 360) % 360;

    if (slope >= 180) {
        slope -= 360;
    }
    slope *= -1;

    if (abs(slope) <= 8) {
        slope = 0;
    }

    return deg;
}

void BNO055::setOffset(void) {
    sensors_event_t event;
    sensorPtr->getEvent(&event);
    offset = event.magnetic.x;
    if (isGyroDisabled) {
        offset = event.magnetic.x;
    } else {
        offset = event.orientation.x;
    }
    slopeOffset = event.orientation.z;
}