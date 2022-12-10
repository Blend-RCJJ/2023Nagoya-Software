#include "ws2812b.h"

extern Adafruit_NeoPixel stripL;
extern Adafruit_NeoPixel stripR;
extern Adafruit_NeoPixel stripUI;
extern Adafruit_NeoPixel stripTop;

WS2812B::WS2812B(int brightness) {
    stripR.begin();
    stripR.setBrightness(brightness);
    stripR.show();

    stripL.begin();
    stripL.setBrightness(brightness);
    stripL.show();

    stripUI.begin();
    stripUI.setBrightness(brightness);
    stripUI.show();

    stripTop.begin();
    stripTop.setBrightness(brightness);
    stripTop.show();
}

void WS2812B::battery(int percent) {
    //寝れぬなら　書くまで待とう　プログラム by 家康
}

void WS2812B::show(void) {
    stripR.show();
    stripL.show();
    stripUI.show();
    stripTop.show();
}