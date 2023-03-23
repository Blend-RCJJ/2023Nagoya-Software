#ifndef _Location_KIT_H
#define _Location_KIT_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define MAP_ORIGIN 20

class MAP_Kit {
   public:
    bool isPassed = false;
    bool isDetected = false;

    bool northWall = false;  // 起動時の方角を北とする。
    bool southWall = false;
    bool eastWall = false;
    bool westWall = false;

    int floorColor = 0;  // 床の色

    bool isVictimDetected = false;

    unsigned long firstPassedTime = 0;
};

class Location_Kit {
   public:
    Location_Kit();

    MAP_Kit mapData[MAP_ORIGIN * 2][MAP_ORIGIN * 2];
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

    unsigned long lastTrustX = 0;
    unsigned long lastTrustY = 0;

    unsigned long lastCorrection = 0;

    void updateOdometory(int rightWheelSpeed, int leftWheelSpeed, int angle,
                         int slope);
    void updateObservationData(int *vecX, int *vecY, int angle);
};

#endif