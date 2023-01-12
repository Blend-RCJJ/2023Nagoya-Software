#ifndef _LED_H_
#define _LED_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#

class STS3032 {
   public:
    STS3032(HardwareSerial *p);

    HardwareSerial *serialPtr;
    int speed[4] = {0};  // 0 - 100;

   private:
};

#endif