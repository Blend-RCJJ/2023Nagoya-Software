#ifndef _SLAM_KIT_H
#define _SLAM_KIT_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class distanceData {
   public:
    int landmark = 0;  // 時刻t時の予測された絶対座標（ワールド座標系）
    int distance = 0;  // 時刻t時の距離（ワールド座標系）
    int diffDistance = 0;  // 時刻t時の距離からの差分（ワールド座標系）
    int oldDistance = 0;  // 直近の距離（ワールド座標系）

    // NOTE:基本的に線形変換済みの値を用いる！！
};

class SLAM_Kit {
   public:
    SLAM_Kit();

    long coordinateX = 0;  // 時刻t時の予測された絶対座標（ワールド座標系）
    long coordinateY = 0;  // 時刻t時の予測された絶対座標（ワールド座標系）

    int rightWheelSpeed = 0;
    int leftWheelSpeed = 0;

    distanceData obsData[12];
    void updateObservationData(int *vecY);
    void updateCoordinate(int angle);
};

#endif