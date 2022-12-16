#ifndef _SENSOR_APP_H
#define _SENSOR_APP_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

extern HardwareSerial uart1;
extern RTOS_Kit app;

extern VL53L0X distanceSensor;
extern BNO055 gyro;
extern WS2812B led;
extern MLT8530 speaker;
// extern SMS_STS st;
extern SWITCHUI ui;

void inputMonitoringApp(App) {
    while (1) {
        distanceSensor.getDistance();
        distanceSensor.calcVector();

        gyro.read();
        ui.read();

        app.delay(10);
    }
}

void left(void) {
    while (gyro.deg <= 80) {
        st.WriteSpe(1, 5000, 0);
        st.WriteSpe(2, 5000, 0);
        st.WriteSpe(3, 5000, 0);
        st.WriteSpe(4, 5000, 0);
        delay(1000);

        break;
    }
    while ((distanceSensor.val[0] <= 120) && (gyro.deg <= 170)) {
        st.WriteSpe(1, 5000, 0);
        st.WriteSpe(2, 5000, 0);
        st.WriteSpe(3, 5000, 0);
        st.WriteSpe(4, 5000, 0);
        delay(1000);

        break;
    }
    while ((distanceSensor.val[0] <= 120) && (gyro.deg <= 260)) {
        st.WriteSpe(1, 5000, 0);
        st.WriteSpe(2, 5000, 0);
        st.WriteSpe(3, 5000, 0);
        st.WriteSpe(4, 5000, 0);
        delay(1000);

        break;
    }
    while ((distanceSensor.val[0] <= 120) && (gyro.deg <= 350)) {
        st.WriteSpe(1, 5000, 0);
        st.WriteSpe(2, 5000, 0);
        st.WriteSpe(3, 5000, 0);
        st.WriteSpe(4, 5000, 0);
        delay(1000);

        break;
    }
}

void DriveLeft(App) {
    pinMode(PB12, OUTPUT);

    while (1) {
        if (distanceSensor.val[0] <= 120 && distanceSensor.val[0] != 0) {
            left();

            digitalWrite(PB12, LOW);
        } else {
            digitalWrite(PB12, HIGH);

            st.WriteSpe(1, -5000, 0);
            st.WriteSpe(2, -5000, 0);
            st.WriteSpe(3, 5000, 0);
            st.WriteSpe(4, 5000, 0);
        }
    }
}

void right(void) {
    while (gyro.deg <= 80) {
        st.WriteSpe(1, -5000, 0);
        st.WriteSpe(2, -5000, 0);
        st.WriteSpe(3, -5000, 0);
        st.WriteSpe(4, -5000, 0);
        delay(1000);

        break;
    }
    while ((distanceSensor.val[0] <= 120) && (gyro.deg <= 170)) {
        st.WriteSpe(1, -5000, 0);
        st.WriteSpe(2, -5000, 0);
        st.WriteSpe(3, -5000, 0);
        st.WriteSpe(4, -5000, 0);
        delay(1000);

        break;
    }
    while ((distanceSensor.val[0] <= 120) && (gyro.deg <= 260)) {
        st.WriteSpe(1, -5000, 0);
        st.WriteSpe(2, -5000, 0);
        st.WriteSpe(3, -5000, 0);
        st.WriteSpe(4, -5000, 0);
        delay(1000);

        break;
    }
    while ((distanceSensor.val[0] <= 120) && (gyro.deg <= 350)) {
        st.WriteSpe(1, -5000, 0);
        st.WriteSpe(2, -5000, 0);
        st.WriteSpe(3, -5000, 0);
        st.WriteSpe(4, -5000, 0);
        delay(1000);

        break;
    }
}

void DriveRight(App) {
    pinMode(PB12, OUTPUT);

    while (1) {
        if (distanceSensor.val[0] <= 120 && distanceSensor.val[0] != 0) {
            right();

            digitalWrite(PB12, LOW);
        } else {
            digitalWrite(PB12, HIGH);

            st.WriteSpe(1, -5000, 0);
            st.WriteSpe(2, -5000, 0);
            st.WriteSpe(3, 5000, 0);
            st.WriteSpe(4, 5000, 0);
        }
    }
}

void straight(void) {
    st.WriteSpe(1, -5000, 0);
    st.WriteSpe(2, -5000, 0);
    st.WriteSpe(3, 5000, 0);
    st.WriteSpe(4, 5000, 0);
}

void propagateRight(App) {
    while (1) {
        straight();
        if (distanceSensor.val[3] >= 100) {
            st.WriteSpe(1, 5000, 0);
            st.WriteSpe(2, 5000, 0);
            st.WriteSpe(3, -5000, 0);
            st.WriteSpe(4, -5000, 0);
            delay(1000);
        } else {
            straight();
        }
    }
}

// void TurnLeft(App) {
//     while (1) {
//         distanceSensor.val[12];
//         gyro.deg;

//         if (distanceSensor.val[0] <= 90) {
//             st.WriteSpe(1, -5000, 0);
//             st.WriteSpe(4, 5000, 0);
//             st.WriteSpe(2, -5000, 0);
//             st.WriteSpe(3, 5000, 0);  // 時計回りに回転
//             if (gyro.deg == 90) {
//                 break;
//             }
//         }
//     }
// }

#endif