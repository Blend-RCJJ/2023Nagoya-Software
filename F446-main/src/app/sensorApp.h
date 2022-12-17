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

void DriveLeft(App) {
    pinMode(PB12, OUTPUT);

    while (1) {
        if (distanceSensor.val[0] <= 120 && distanceSensor.val[0] != 0) {
            servo.drive(0, 90);

            digitalWrite(PB12, LOW);
        } else {
            digitalWrite(PB12, HIGH);
            servo.directDrive(0, 100);
            servo.directDrive(1, 100);
            servo.directDrive(2, 100);
            servo.directDrive(3, 100);
        }
    }
}

void DriveRight(App) {
    pinMode(PB12, OUTPUT);

    while (1) {
        if ((distanceSensor.val[0] <= 120 && distanceSensor.val[0] != 0) &&
            gyro.deg < 90) {
            servo.drive(0, 90);
            app.delay(300);

            digitalWrite(PB12, LOW);
        } else if (distanceSensor.val[0] <= 120 &&
                   (((gyro.deg >= 80) && (gyro.deg <= 100))) &&
                   distanceSensor.val[0] != 0) {
            servo.drive(0, 180);
            app.delay(550);
        } else if (distanceSensor.val[0] <= 120 &&
                   (((gyro.deg >= 170) && (gyro.deg <= 190))) &&
                   distanceSensor.val[0] != 0) {
            servo.drive(0, 270);
            app.delay(550);
        } else if (distanceSensor.val[0] <= 120 &&
                   (((gyro.deg >= 260) && (gyro.deg <= 280))) &&
                   distanceSensor.val[0] != 0) {
            servo.drive(0, 0);
            app.delay(550);
        } else {
            servo.directDrive(0, -80);
            servo.directDrive(1, -80);
            servo.directDrive(2, 80);
            servo.directDrive(3, 80);
            digitalWrite(PB12, HIGH);
        }
    }
}

void propagateRight(App) {
    while (1) {
        static int angle = 0;
        if (angle >= 360) {
            angle = 0;
        }
        if (angle <= -360) {
            angle = 0;
        }
        if(distanceSensor.val[3] > 2000){
            distanceSensor.val[3] = 2000;
        }

        if(distanceSensor.val[6] > 2000){
            distanceSensor.val[6] = 2000;
        }
        // if (distanceSensor.val[0] >= 120 && distanceSensor.val[3] >= 100) {
        //     angle += 90;
        //     servo.drive(0, angle);
        //     app.delay(2000);

        // } else {
        //     servo.directDrive(0, -80);
        //     servo.directDrive(1, -80);
        //     servo.directDrive(2, 80);
        //     servo.directDrive(3, 80);
        // }

        if (distanceSensor.val[0] <= 120) {
            if (distanceSensor.val[3] < distanceSensor.val[9]) {
                angle -= 90;
                
        servo.drive(0, angle);
                app.delay(600);
            } else if (distanceSensor.val[3] > distanceSensor.val[9]) {
                angle += 90;
                servo.drive(0, angle);
                
        servo.drive(0, angle);
                app.delay(600);
            }
        } else {
            // servo.directDrive(0, -80);
            // servo.directDrive(1, -80);
            // servo.directDrive(2, 80);
            // servo.directDrive(3, 80);
        }

        servo.drive(100, angle);
    }
}  //[3]がでかくなる時→右　前方接近→[3]or[9]の値がでかい方向に曲がる

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