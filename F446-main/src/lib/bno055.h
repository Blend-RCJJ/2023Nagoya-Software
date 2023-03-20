#ifndef _BNO055_H
#define _BNO055_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

class BNO055 {
   public:
    BNO055(Adafruit_BNO055 *p);
    Adafruit_BNO055 *sensorPtr;
    
    int deg = 0;
    int magnetic = 0;
    int offset = 0;

    int error = 0;
    int oldDeg = 0;

    bool isGyroDisabled = true;

    void init(void);
    void setOffset(void);
    int read(void);

   private:
};

#endif