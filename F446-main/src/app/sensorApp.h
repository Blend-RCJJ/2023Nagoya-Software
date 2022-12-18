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
extern SWITCHUI ui;
extern Adafruit_NeoPixel stripFloor;
extern WS2812B led;

extern UNITV cameraLeft;
extern UNITV cameraRight;
int angle = 0;

void inputMonitoringApp(App) {
    while (1) {
        distanceSensor.getDistance();
        distanceSensor.calcVector();

        gyro.read();
        ui.read();

        led.setFloorColor(led.red);
        delay(3);
        floorSensor.redVal = analogRead(PC0);

        led.setFloorColor(led.green);
        delay(3);
        floorSensor.greenVal = analogRead(PC0);

        led.setFloorColor(led.blue);
        delay(3);
        floorSensor.blueVal = analogRead(PC0);

        cameraLeft.read();
        cameraRight.read();

        // uart1.print(floorSensor.redVal);
        // uart1.print("\t");
        // uart1.print(floorSensor.greenVal);
        // uart1.print("\t");
        // uart1.print(floorSensor.blueVal);
        // uart1.println("\t");
        uart1.print(floorSensor.redVal);
        uart1.print("\t");
        uart1.print(floorSensor.greenVal);
        uart1.print("\t");
        uart1.print(floorSensor.blueVal);
        uart1.println("\t");

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

//         digitalWrite(PB12, LOW);
//     } else if (distanceSensor.val[0] <= 120 &&
//                (((gyro.deg >= 80) && (gyro.deg <= 100))) &&
//                distanceSensor.val[0] != 0) {
//         servo.drive(0, 180);
//         app.delay(550);
//     } else if (distanceSensor.val[0] <= 120 &&
//                (((gyro.deg >= 170) && (gyro.deg <= 190))) &&
//                distanceSensor.val[0] != 0) {
//         servo.drive(0, 270);
//         app.delay(550);
//     } else if (distanceSensor.val[0] <= 120 &&
//                (((gyro.deg >= 260) && (gyro.deg <= 280))) &&
//                distanceSensor.val[0] != 0) {
//         servo.drive(0, 0);
//         app.delay(550);
//     } else {
//         servo.directDrive(0, -50);
//         servo.directDrive(1, -50);
//         servo.directDrive(2, 50);
//         servo.directDrive(3, 50);
//         digitalWrite(PB12, HIGH);
//     }

// }

void largeDrive(App) {
    while (1) {
        if (angle >= 360) {
            angle = 0;
        }
        if (angle <= -360) {
            angle = 0;
        }
        if (distanceSensor.val[3] > 2000) {
            distanceSensor.val[3] = 2000;
        }

        if (distanceSensor.val[9] > 2000) {
            distanceSensor.val[9] = 2000;
        }
        if (distanceSensor.val[9] <= 110) {
            servo.drive(20, angle + 30);
        }
        if (distanceSensor.val[3] <= 110) {
            servo.drive(20, angle - 30);
        }

        if (distanceSensor.val[0] <= 120) {
            if ((distanceSensor.val[3] < distanceSensor.val[9]) &&
                distanceSensor.val[0] <= 120) {
                angle -= 90;

                servo.drive(0, angle);
                app.delay(600);
            }
        }
        if ((distanceSensor.val[3] > distanceSensor.val[9]) &&
            distanceSensor.val[0] <= 120) {
            angle += 90;
            servo.drive(0, angle);
            app.delay(600);
        }

        servo.drive(50, angle);
    }
}

//[3]がでかくなる時→右　前方接近→[3]or[9]の値がでかい方向に曲がる

void onlyRight(App) {
    while (1) {
        if (angle >= 360) {
            angle = 0;
        }
        if (angle <= -360) {
            angle = 0;
        }
        if (distanceSensor.val[3] > 2000) {
            distanceSensor.val[3] = 2000;
        }

        if (distanceSensor.val[9] > 2000) {
            distanceSensor.val[9] = 2000;
        }
        if (distanceSensor.val[9] <= 110) {
            servo.drive(20, angle + 30);
        }
        if (distanceSensor.val[3] <= 110) {
            servo.drive(20, angle - 30);
        }

        if (distanceSensor.val[0] <= 120) {
            if (distanceSensor.val[3] < distanceSensor.val[9]) {
                angle += 90;

                servo.drive(0, angle);
                app.delay(600);
            } else if (distanceSensor.val[3] > distanceSensor.val[9]) {
                angle += 90;
                servo.drive(0, angle);
                app.delay(600);
            }
        }

        servo.drive(50, angle);
    }
}

void onlyLeft(App) {
    while (1) {
        if (angle >= 360) {
            angle = 0;
        }
        if (angle <= -360) {
            angle = 0;
        }
        if (distanceSensor.val[3] > 2000) {
            distanceSensor.val[3] = 2000;
        }

        if (distanceSensor.val[9] > 2000) {
            distanceSensor.val[9] = 2000;
        }
        if (distanceSensor.val[9] <= 110) {
            servo.drive(20, angle + 30);
        }
        if (distanceSensor.val[3] <= 110) {
            servo.drive(20, angle - 30);
        }

        if (distanceSensor.val[0] <= 120) {
            if (distanceSensor.val[3] < distanceSensor.val[9]) {
                angle -= 90;

                servo.drive(0, angle);
                app.delay(600);
            } else if (distanceSensor.val[3] > distanceSensor.val[9]) {
                angle -= 90;
                servo.drive(0, angle);

                servo.drive(0, angle);
                app.delay(600);
            }
        }

        servo.drive(50, angle);
    }
}

void oooon(App) {
    while (1) {
        servo.driveAngularVelocity(0, 100);
        app.delay(200);
        servo.driveAngularVelocity(0, -100);
        app.delay(200);
    }
}

void right(App) {
    while (1) {
        angle += 90;
        angle %= 360;
        servo.driveAngularVelocity(0, angle);
        app.delay(2000);
    }
}

void left(App) {
    while (1) {
        angle -= 90;
        angle %= 360;
        servo.driveAngularVelocity(0, angle);
        app.delay(2000);
    }
}

void random(App) {
    while (1) {
        app.delay(1);
    }
}

// void propageRight(App) {
//     while (1) {
//         static int angle = 0;

//         if (angle >= 360) {
//             angle = 0;
//         }
//         if (angle <= -360) {
//             angle = 0;
//         }
//         if (distanceSensor.val[3] > 2000) {
//             distanceSensor.val[3] = 2000;
//         }

//         if (distanceSensor.val[3] >= 120) {
//             for (int i = 0; i < 3; i++) {
//                 servo.driveAngularVelocity(50, 0);
//                 app.delay(500);
//                 angle += 30;
//             }
//             servo.drive(0, angle);

//             app.delay(600);
//         }
//         if (distanceSensor.val[3] <= 120) {
//             servo.drive(50, angle);
//             app.delay(100);
//         }
//         if (distanceSensor.val[0] <= 120) {
//             if ((distanceSensor.val[3] < distanceSensor.val[9]) &&
//                 distanceSensor.val[0] <= 120) {
//                 angle -= 90;

//                 servo.drive(0, angle);
//                 app.delay(600);
//             }

//             if ((distanceSensor.val[3] > distanceSensor.val[9]) &&
//                 distanceSensor.val[0] <= 120) {
//                 angle += 90;
//                 servo.drive(0, angle);
//                 app.delay(600);
//             }
//         }
//         // servo.drive(50,angle);
//     }
// }

#endif