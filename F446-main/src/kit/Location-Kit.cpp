#include "./Location-Kit.h"

extern HardwareSerial uart1;

Location_Kit::Location_Kit(/* args */) {
}

void Location_Kit::updateOdometory(int rightWheelSpeed, int leftWheelSpeed,
                                   int angle, int slope) {
    double vec = (rightWheelSpeed + leftWheelSpeed) / 2.0;
    double vecX = vec * sin(radians(angle));
    double vecY = vec * cos(radians(angle));

    coordinateX += vecX * period * velocityConstant * cos(radians(slope));
    coordinateY += vecY * period * velocityConstant * cos(radians(slope));
}

void Location_Kit::updateObservationData(int *vecX, int *vecY, int angle) {
    int sensorX = 0;
    int sensorY = 0;

    int minY = 360;  // 0°に一番近いセンサの誤差

    for (int i = 0; i < 12; i++) {
        if (min(abs((360 - i * 30) - angle),
                360 - abs((360 - i * 30) - angle)) < minY) {
            minY = min(abs((360 - i * 30) - angle),
                       360 - abs((360 - i * 30) - angle));
            minIndex = i;
        }
    }

    widthX =
        abs(vecX[(minIndex + 3) % 12]) + abs(vecX[(minIndex + 9) % 12]) + 40;
    widthY = abs(vecY[minIndex]) + abs(vecY[(minIndex + 6) % 12]) + 40;

    trustX = false;
    trustY = false;

    if (280 < widthX && widthX < 310) {
        trustX = true;
        coordinateX = round(coordinateX / 300.0) * 300.0 +
                      abs(vecX[(minIndex + 9) % 12]) - 130;

        lastCorrection = millis();
    }
    if (560 < widthX && widthX < 630) {
        trustX = true;

        if (abs(vecX[(minIndex + 3) % 12]) < abs(vecX[(minIndex + 9) % 12])) {
            coordinateX = round(coordinateX / 300.0) * 300.0 + 
                          (300 - abs(vecX[(minIndex + 3) % 12]) - 40) - 130;
        } else {
            coordinateX = round(coordinateX / 300.0) * 300.0 +
                        abs(vecX[(minIndex + 9) % 12]) - 130;
        }

        lastCorrection = millis();
    }

    if (280 < widthY && widthY < 310) {
        trustY = true;
        coordinateY = round(coordinateY / 300.0) * 300.0 +
                      abs(vecY[(minIndex + 6) % 12]) - 130;
        lastCorrection = millis();
    }

    if (560 < widthY && widthY < 630) {
        trustY = true;

        if (abs(vecY[minIndex]) < abs(vecY[(minIndex + 6) % 12])) {
            coordinateY = round(coordinateY / 300.0) * 300.0 + 
                          (300 - abs(vecY[minIndex]) - 40) - 130;
        } else {
            coordinateY = round(coordinateY / 300.0) * 300.0 +
                        abs(vecY[(minIndex + 6) % 12]) - 130;
        }

        lastCorrection = millis();
    }

    x = round(coordinateX / 300.0);
    y = round(coordinateY / 300.0);
}

// void MAP::