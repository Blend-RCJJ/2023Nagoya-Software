// led.h

#ifndef _LED_H_
#define _LED_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel.h>

class WS2812B {
   public:
    WS2812B(int brightness);

    void battery(int percent);

    void show(void);

   private:
};

#endif