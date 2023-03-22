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
    Input inputPin[2] = {PB1, PC4};  // 0が左

    int val[2] = {0};
    int offset[2] = {0};

    bool l = false;
    bool r = false;

    int cutOff = 20;  // 調整ゲー

    void read(void) {
        for (int i = 0; i < 2; i++) {
            long temp = 0;
            for (int j = 0; j < 200; j++) {
                temp += inputPin[i].analog();
            }
            val[i] = constrain(temp / 200 - offset[i], 0, 500) / 5;
        }

        l = val[0] > cutOff;
        r = val[1] > cutOff;
    }

    void init(void) {
        for (int i = 0; i < 2; i++) {
            long temp = 0;
            for (int j = 0; j < 1000; j++) {
                temp += inputPin[i].analog();
            }
            val[i] = temp / 1000;
        }
    }
};
#endif