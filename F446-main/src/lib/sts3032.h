#ifndef _STS3032_H_
#define _STS3032_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "./SCServo/SCServo.h"

extern SMS_STS st;

class STS3032 {
   public:
    STS3032(int Servo_Motor);

    unsigned long currentspeed = st.WriteSpe(1, SPEED * 100, 0);

    void Servo_Speed(unsigned long currentspeed);

   private:
};

#endif