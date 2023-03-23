#ifndef _LOADCELL_H_
#define _LOADCELL_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "../kit/IO-Kit.h"

#define LEFT 1
#define CENTER 2
#define RIGHT 3

class LOADCELL {
   public:
    Input inputPin[2] = {PC2, PC3};  // 0が左

    int load[2] = {0, 0};

    int offset[2];

    int status = 0;
    int moment = 0;

    void read(void) {
        for (int i = 0; i < 2; i++) {
            load[i] = inputPin[i].raw();
        }
        load[1] = 1023 - load[1];

        for (int i = 0; i < 2; i++) {
            load[i] -= offset[i];
            load[i] = constrain(load[i], 0, 800);
            load[i] /= 8;
        }

        if (load[0] > 10 || load[1] > 10) {
            moment = degrees(atan2(load[0], load[1]));

            if (moment < 45) {
                status = LEFT;
            } else {
                status = RIGHT;
            }
        } else {
            status = 0;
        }
    }

    void init(void) {
        for (int i = 0; i < 2; i++) {
            offset[i] = inputPin[i].raw();
        }
        offset[1] = 1023 - offset[1];
    }

   private:
};

#endif