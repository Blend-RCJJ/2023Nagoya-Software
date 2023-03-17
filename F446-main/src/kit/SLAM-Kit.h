#ifndef _SLAM_KIT_H
#define _SLAM_KIT_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class SLAM_Kit {
   public:
    SLAM_Kit();

    // double velocityConstant = 0.00192;
    double velocityConstant = 0.00192;

    double coordinateX = 0;  // 時刻t時の予測された絶対座標（ワールド座標系）
    double coordinateY = 0;  // 時刻t時の予測された絶対座標（ワールド座標系）

    int period = 10;
    int minIndex = 0;

    int rightWheelSpeed = 0;
    int leftWheelSpeed = 0;

    int offset[4] = { 105, 165, 130, 130 };
    int widthY = 0;
    int widthX = 0;

    int x = 0;
    int y = 0;

    bool trustX = false;
    bool trustY = false;

    void updateOdometory(int rightWheelSpeed, int leftWheelSpeed, int angle);
    void updateObservationData(int *vecX, int *vecY, int angle);
};

#endif