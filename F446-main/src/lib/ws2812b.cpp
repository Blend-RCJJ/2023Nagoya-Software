#include "ws2812b.h"

extern Adafruit_NeoPixel stripL;
extern Adafruit_NeoPixel stripR;
extern Adafruit_NeoPixel stripUI;
extern Adafruit_NeoPixel stripTop;

unsigned long WS2812B::colorRGB(int red, int green, int blue) {
    return stripR.Color(red, green, blue);
}

unsigned long WS2812B::colorHSV(int hue, int saturation, int brightness) {
    return stripR.ColorHSV(hue * 256, saturation, brightness);
}

void WS2812B::setUIColor(unsigned long color) {
    for (int i = 0; i < 24; i++) {
        stripUI.setPixelColor(i, color);
    }
}

void WS2812B::setLeftColor(unsigned long color) {
    for (int i = 0; i < 7; i++) {
        stripL.setPixelColor(i, color);
    }
}

void WS2812B::setRightColor(unsigned long color) {
    for (int i = 0; i < 7; i++) {
        stripR.setPixelColor(i, color);
    }
}

void WS2812B::bootIllumination(void) {
    for (int i = 0; i < 24; i++) {  //上面
        stripTop.setPixelColor(i, colorHSV(map(i, 0, 24, 0, 255), 255, 255));
        stripTop.show();
        delay(20);
    }
    for (int i = 0; i < 24; i++)
    {
        stripTop.setPixelColor(i,0,0,0);
    }
    stripTop.show();
    
}

WS2812B::WS2812B(int brightness) {
    maxBrightness = brightness;

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
    for (int percent = 0; percent <= 100; percent++) {
        if (percent <= 10) {
            stripUI.setPixelColor(0, 255, 0, 0);
            stripUI.setPixelColor(1, 255, 0, 0);

        } else if (percent <= 20) {
            stripUI.setPixelColor(2, 200, 55, 0);
            stripUI.setPixelColor(3, 200, 55, 0);

        } else if (percent <= 30) {
            stripUI.setPixelColor(4, 190, 65, 0);
            stripUI.setPixelColor(5, 180, 75, 0);
            stripUI.setPixelColor(6, 180, 75, 0);

        } else if (percent <= 40) {
            stripUI.setPixelColor(7, 170, 85, 0);
            stripUI.setPixelColor(8, 160, 95, 0);
            stripUI.setPixelColor(9, 160, 95, 0);

        } else if (percent <= 50) {
            stripUI.setPixelColor(10, 140, 115, 0);
            stripUI.setPixelColor(11, 120, 125, 0);
            stripUI.setPixelColor(12, 120, 125, 0);

        } else if (percent <= 60) {
            stripUI.setPixelColor(13, 100, 120, 35);
            stripUI.setPixelColor(14, 90, 110, 55);

        } else if (percent <= 70) {
            stripUI.setPixelColor(15, 80, 100, 75);
            stripUI.setPixelColor(16, 70, 90, 95);

        } else if (percent <= 80) {
            stripUI.setPixelColor(17, 60, 80, 115);
            stripUI.setPixelColor(18, 50, 70, 135);

        } else if (percent <= 90) {
            stripUI.setPixelColor(19, 40, 60, 155);
            stripUI.setPixelColor(20, 30, 50, 175);

        } else if (percent <= 99) {
            stripUI.setPixelColor(21, 20, 40, 195);
            stripUI.setPixelColor(22, 10, 30, 215);

        } else if (percent <= 100) {
            stripUI.setPixelColor(23, 0, 0, 255);
        }
    }

    // 寝れぬなら　書くまで待とう　プログラム by 家康
}

void WS2812B::start(int numOfLED) {
    for (int numOfLED = 0; numOfLED <= 24; numOfLED++) {
        if (numOfLED % 3 == 0) {
            delay(25);
            stripUI.setPixelColor(numOfLED, 255, 0, 0);
            show();
        } else if (numOfLED % 3 == 1) {
            delay(50);
            stripUI.setPixelColor(numOfLED, 0, 255, 0);
            show();
        } else {
            delay(75);
            stripUI.setPixelColor(numOfLED, 100, 100, 100);
            show();
        }
    }
    show();
}

void WS2812B::leftBootLED(int LEDOfLeft) {
    for (int LEDOfLeft = 7; LEDOfLeft >= 0; LEDOfLeft--) {
        if (LEDOfLeft % 3 == 0) {
            delay(25);
            stripL.setPixelColor(LEDOfLeft, 255, 0, 0);
            show();
        } else if (LEDOfLeft % 3 == 1) {
            delay(50);
            stripL.setPixelColor(LEDOfLeft, 0, 255, 0);
            show();
        } else {
            delay(75);
            stripL.setPixelColor(LEDOfLeft, 100, 100, 100);
            show();
        }
    }
    show();
}

void WS2812B::rightBootLED(int LEDofRight) {
    for (int LEDOfRight = 0; LEDOfRight <= 7; LEDOfRight++) {
        if (LEDOfRight % 3 == 0) {
            delay(25);
            stripR.setPixelColor(LEDOfRight, 255, 0, 0);
            show();
        } else if (LEDOfRight % 3 == 1) {
            delay(50);
            stripR.setPixelColor(LEDOfRight, 0, 255, 0);
            show();
        } else {
            delay(75);
            stripR.setPixelColor(LEDOfRight, 100, 100, 100);
            show();
        }
    }
    show();
}

void WS2812B::tktk(int number) {
    while (1) {
        static int number = 0;
        setUIBrightness(127 * sin(number / 50.0) + 127);
        setRightBrightness(127 * sin(number / 50.0) + 127);
        setLeftBrightness(127 * sin(number / 50.0) + 127);
        show();
        number++;
        if (number == 1000000) {
            break;
        }
    }
}

void WS2812B::show(void) {
    stripR.show();
    stripL.show();
    stripUI.show();
    stripTop.show();
}

void WS2812B::setUIBrightness(int brightness) {
    stripUI.setBrightness(brightness * maxBrightness / 255);
}

void WS2812B::setRightBrightness(int brightness) {
    stripR.setBrightness(brightness * maxBrightness / 255);
}

void WS2812B::setLeftBrightness(int brightness) {
    stripL.setBrightness(brightness * maxBrightness / 255);
}