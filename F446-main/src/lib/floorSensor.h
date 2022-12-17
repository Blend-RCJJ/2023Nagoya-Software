#ifndef _FLOOR_SENSOR_H_
#define _FLOOR_SENSOR_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class FLOOR_SENSOR {
   public:
    const int brightness = 200;
    int redVal;
    int greenVal;
    int blueVal;
};

#endif
