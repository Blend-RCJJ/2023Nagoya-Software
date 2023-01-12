#ifndef _MLT8530_H_
#define _MLT8530_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "IO-Kit.h"

class MLT8530 {
   public:
    Output pin = PB6;

    void setFrequncy(int freq);
    void mute(void);

    void bootSound(void);
};

#endif