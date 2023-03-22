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

    void read(void) {
        for (int i = 0; i < 2; i++) {
            load[i] = inputPin[i].analog();
        }
        load[1] = 1023 - load[1];

        for (int i = 0; i < 2; i++) {
            load[i] -= offset[i];
            load[i] = constrain(load[i], 0, 800);
            load[i] /= 8;
        }

        if (load[0] > 5 || load[1] > 5) {
            int moment = degrees(atan2(load[0], load[1]));

            if (moment < 35) {
                moment = LEFT;
            } else if (moment > 55) {
                moment = RIGHT;
            } else {
                moment = CENTER;
            }
        } else {
            status = 0;
        }
    }

    void init(void) {
        for (int i = 0; i < 2; i++) {
            offset[i] = inputPin[i].analog();
        }
        offset[1] = 1023 - offset[1];
    }

   private:
};

#endif