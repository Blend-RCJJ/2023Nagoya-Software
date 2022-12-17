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
    void start(int numOfLED);
    void leftBootLED(int LEDOfLeft);
    void rightBootLED(int LEDOfRight);
    void tktk(int number);

    void bootIllumination(void);

    void show(void);

    int maxBrightness;

    unsigned long colorRGB(int red, int green, int blue);
    unsigned long colorHSV(int hue, int saturation, int brightness);

    unsigned long red = colorHSV(0,255,255);
    unsigned long green = colorHSV(90,255,255);
    unsigned long blue = colorHSV(180,255,255);
    unsigned long pink = colorHSV(0,125,255);
    unsigned long yellow = colorHSV(45,255,255);
    unsigned long white = colorHSV(255,0,255);

    void setUIColor(unsigned long color);
    void setLeftColor(unsigned long color);
    void setRightColor(unsigned long color);
    void setTopColor(unsigned long color);

    void setUIBrightness(int brightness);
    void setRightBrightness(int brightness);
    void setLeftBrightness(int brightness);
    void setTopBrightness(int brightness);

   private:
};

#endif