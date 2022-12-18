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

    bool isVictimDetected = false;
    int victimType = 0;

    void read(void);

   private:
    int victimTypeList[10] = {0};
    int victimCount = 0;
    unsigned long lastUpdatingTime = 0;
    unsigned long firstReadTime = 0;
};

#endif