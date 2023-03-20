#ifndef _Location_KIT_H
#define _Location_KIT_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class MAP_Kit {
   public:
    int x = 1000;
    int y = 1000;

    bool north = false;  // 起動時の方角を北とする。
    bool south = false;
    bool east = false;
    bool west = false;

    int counter = 0;     // 通った回数
    int floorColor = 0;  // 床の色
};

class Location_Kit {
   public:
    Location_Kit();

    // double velocityConstant = 0.00192;
    double velocityConstant = 0.0018;

    double coordinateX = 0;  // 時刻t時の予測された絶対座標（ワールド座標系）
    double coordinateY = 0;  // 時刻t時の予測された絶対座標（ワールド座標系）

    int period = 10;
    int minIndex = 0;

    int rightWheelSpeed = 0;
    int leftWheelSpeed = 0;

    int offset[4] = {105, 165, 130, 130};
    int widthY = 0;
    int widthX = 0;

    int x = 0;
    int y = 0;

    bool trustX = false;
    bool trustY = false;

    unsigned long lastCorrection = 0;

    void updateOdometory(int rightWheelSpeed, int leftWheelSpeed, int angle,
                         int slope);
    void updateObservationData(int *vecX, int *vecY, int angle);
};

#endif