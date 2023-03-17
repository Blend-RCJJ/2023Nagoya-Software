#include "./SLAM-Kit.h"

extern HardwareSerial uart1;

SLAM_Kit::SLAM_Kit(/* args */) {
}

void SLAM_Kit::updateOdometory(int rightWheelSpeed, int leftWheelSpeed,
                               int angle) {
    double vec = (rightWheelSpeed + leftWheelSpeed) / 2.0;
    double vecX = vec * sin(radians(angle));
    double vecY = vec * cos(radians(angle));

    coordinateX += vecX * period * velocityConstant;
    coordinateY += vecY * period * velocityConstant;
}

void SLAM_Kit::updateObservationData(int *vecX, int *vecY, int angle) {
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

    if (widthX < 310) {
        trustX = true;
    } else {
        trustX = false;
    }

    if (widthY < 310) {
        trustY = true;
    } else {
        trustY = false;
    }

    if (trustX) {
        coordinateX = round(coordinateX / 300.0) * 300.0  + abs(vecX[(minIndex + 9) % 12]) - 130;
    }

    if (trustY) {
        coordinateY = round(coordinateY / 300.0) * 300.0  + abs(vecY[(minIndex + 6) % 12]) - 130;
    }

    x = round(coordinateX / 300.0);
    y = round(coordinateY / 300.0);
}
