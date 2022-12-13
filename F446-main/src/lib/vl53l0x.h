#ifndef _VL53L0X_H
#define _VL53L0X_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class VL53L0X {
   public:
    VL53L0X(HardwareSerial *p);

    HardwareSerial *serialPtr;

    int val[12] = {0};

    int vecX[12] = {0};
    int vecY[12] = {0};

    int getDistance();
    void calcVector(void);  // 定義しときました〜

   private:
    int _valTemp[12] = {0};
};
#endif