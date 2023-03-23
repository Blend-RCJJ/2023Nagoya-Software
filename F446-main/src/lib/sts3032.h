#ifndef _STS3032_H_
#define _STS3032_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "./SCServo/SCServo.h"
#include "./lib/bno055.h"

extern BNO055 gyro;

class STS3032 {
   public:
    STS3032(HardwareSerial *ptr);
    HardwareSerial *serialPtr;

    const int maximumSpeed = 7000;
    const int baudRate = 1000000;
    void directDrive(int id, int percent, int acceleration = 0);
    void drive(int velocity, int angle);
    void driveAngularVelocity(int velocity, int angularVelocity);

    void stop(void);

    int velocity = 0;
    int angle = 0;

    int rightWheelSpeed = 0;
    int leftWheelSpeed = 0;

    void rescueKit(int num, int position);

   private:
};

#endif