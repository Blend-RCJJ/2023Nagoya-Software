#ifndef _UNITV_H_
#define _UNITV_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define VICTIM_H 'H'
#define VICTIM_U 'U'
#define VICTIM_S 'S'
#define VICTIM_RED 'R'
#define VICTIM_GREEN 'G'
#define VICTIM_YELLOW 'B'
#define NO_VICTIM 'N'

class UNITV {
   public:
    HardwareSerial *serialPtr;
    UNITV(HardwareSerial *ptr);

    bool isVictimDetected = true;
    int victimType = 0;
    int data = 'N';

    void read(void);

   private:
};

#endif