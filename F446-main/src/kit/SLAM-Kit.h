#ifndef _SLAM_KIT_H
#define _SLAM_KIT_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class mapData {
   public:
    bool isDiscoverd = false;
    bool wall[4] = {false, false, false, false};  // 前右後左
};

class SLAM_Kit {
   public:
    mapData map[40][40];

    SLAM_Kit();

    void updatePosition(int *dataPtr, int gyro);
    void reliablenessEvaluation(void);
    void integration(void);

    int x = 0;
    int y = 0;
    int dx = 0;
    int dy = 0;
    int dxRaw = 0;
    int dyRaw = 0;
    bool reliableness[6];  // 向かい合う組をペアとして計算する

    int route[200][2] = {0};
    int routeIndex = 0;

   private:
    int distance[12];
    int difference[12];
    int angle;

    const int allowanceValue = 30;  // 許容値
};

#endif