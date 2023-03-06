#include "./SLAM-Kit.h"

extern HardwareSerial uart1;

SLAM_Kit::SLAM_Kit(/* args */) {
}

void SLAM_Kit::updateObservationData(int *vecY) {
    for (int i = 0; i < 12; i++) {
        if (abs(obsData[i].oldDistance - vecY[i]) > 40) {  // 非連続点判定
            obsData[i].landmark = coordinateY;
            obsData[i].distance = vecY[i];
        }

        // ランドマークからの差分
        obsData[i].diffDistance = vecY[i] - obsData[i].distance;
        obsData[i].oldDistance = vecY[i];
    }
}

void SLAM_Kit::updateCoordinate(int angle) {
    int obsCoordinate[12];
    for (int i = 0; i < 12; i++) {
        obsCoordinate[i] = obsData[i].landmark + obsData[i].diffDistance;
    }

    int sum = 0;

    // uart1.print("weight: ");
    for (int i = 0; i < 12; i++) {
        int weight = min(abs(180 - (angle + i * 30) % 360),
                         min(abs(0 - (angle + i * 30) % 360),
                             abs(360 - (angle + i * 30) % 360)));
        weight = 90 - weight;
        weight /= 10;

        sum += weight;
        obsCoordinate[i] *= weight;

        // uart1.print(weight);
        // uart1.print("\t");
    }

    long newCoordinate = 0;

    for (int i = 0; i < 12; i++) {
        newCoordinate += obsCoordinate[i];
    }
    newCoordinate /= sum;

    coordinateY = newCoordinate;


}