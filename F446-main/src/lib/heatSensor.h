#ifndef _HEAT_SENSOR_H_
#define _HEAT_SENSOR_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "../kit/IO-Kit.h"

class HEATSENSOR {
   public:
    Input inputPin[2] = {PC4, PB1};  // 0が左

    int val[2] = {0};
    int offset[2] = {0};

    bool l = false;
    bool r = false;

    int cutOff = 500;  // 調整ゲー

    unsigned long timer = 0;

    void read(void) {
        for (int i = 0; i < 2; i++) {
            long temp = 0;
            for (int j = 0; j < 100; j++) {
                temp += inputPin[i].raw();
            }
            val[i] = temp / 100;
        }

        l = val[0] > cutOff + 40;
        r = val[1] > cutOff;
    }

    void init(void) {
        for (int i = 0; i < 2; i++) {
            long temp = 0;
            for (int j = 0; j < 1000; j++) {
                temp += inputPin[i].raw();
            }
            offset[i] = temp / 1000;
        }
    }
};
#endif