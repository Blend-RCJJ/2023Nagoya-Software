#include "sts3032.h"

#define SPEED 50  // 速度(0~100%)

void STS3032::Servo_Speed(unsigned long currentspeed) {
    currentspeed = SPEED * 100;
    st.WriteSpe(1,currentspeed,0);
}