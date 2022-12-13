#ifndef _SWITCHUI_H_
#define _SWITCHUI_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "IO-Kit.h"

class SWITCHUI {
   public:
    Input buttonPin[2] = {PA4, PA5};
    Input togglePin = PA6;

    bool button[2] = {false, false};
    bool toggle = false;

    void read(void) {
        for (int i = 0; i < 2; i++) {
            button[i] = !buttonPin[i];
        }
        toggle = !togglePin;
    }

   private:
};

#endif