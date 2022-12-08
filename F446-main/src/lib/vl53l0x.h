#ifndef _VL53L0X_H
#define _VL53L0X_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class VL53L0X {
   public:
    VL53L0X();

    HardwareSerial *serialPtr;

    int distance[12] = {0};
    int getDistance();

   private:
};

#endif