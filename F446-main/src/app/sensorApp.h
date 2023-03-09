#ifndef _SENSOR_APP_H
#define _SENSOR_APP_H
#define FLOOR_NUMBER 2

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

extern HardwareSerial uart1;
extern HardwareSerial uart3;
extern RTOS_Kit app;
extern SLAM_Kit slam;

extern Adafruit_NeoPixel stripL;
extern Adafruit_NeoPixel stripR;
extern Adafruit_NeoPixel stripUI;
extern Adafruit_NeoPixel stripTop;
extern Adafruit_NeoPixel stripFloor;

extern VL53L0X distanceSensor;
extern BNO055 gyro;
extern WS2812B led;
extern MLT8530 speaker;
extern SWITCHUI ui;
extern Adafruit_NeoPixel stripFloor;
extern WS2812B led;

extern UNITV cameraLeft;
extern UNITV cameraRight;

int count = 0;
int val6  = 0;
void inputMonitoringApp(App) {
    while (1) {
        distanceSensor.getDistance();
        distanceSensor.calcVector();

        gyro.read();
        ui.read();

        led.setFloorColor(led.white);
        app.delay(3);
        floorSensor.redVal = analogRead(PC0);

        led.setFloorColor(led.blue);
        app.delay(3);
        floorSensor.greenVal = analogRead(PC0);

        led.setFloorColor(led.green);
        app.delay(3);
        floorSensor.blueVal = analogRead(PC0);

        cameraLeft.read();
        cameraRight.read();
    }
}

void slamApp(App) {
    while (1) {
        slam.updateObservationData(distanceSensor.vecY);
        slam.updateCoordinate(gyro.deg);

        app.delay(10);
    }
}

void servoApp(App) {
    while (1) {
        servo.drive(servo.velocity, servo.angle);
        app.delay(2);
    }
}

void gridSpecification(App) {
    while (1) {
        if ((distanceSensor.val[0] > distanceSensor.val[3]) &&
            (distanceSensor.val[0] > distanceSensor.val[9])) {
            servo.velocity = 50;
            app.delay(4200);
            servo.velocity = 0;
            app.delay(1000);
        } else if (distanceSensor.val[3] > distanceSensor.val[9]) {
            servo.velocity = 0;
            servo.angle += 90;
            app.delay(1000);
            servo.velocity = 50;
            app.delay(4200);
            servo.velocity = 0;
            app.delay(1000);
        } else if (distanceSensor.val[3] < distanceSensor.val[9]) {
            servo.velocity = 0;
            servo.angle -= 90;
            app.delay(1000);
            servo.velocity = 50;
            app.delay(4200);
            servo.velocity = 0;
            app.delay(1000);
        } else {
            app.delay(10);
        }

        if (distanceSensor.val[0] <= 150) {
            if (distanceSensor.val[3] <= 150) {
                if (distanceSensor.val[9] <= 150) {
                    servo.velocity = 0;
                    servo.angle += 180;
                    app.delay(1000);
                    servo.velocity = 50;
                    app.delay(4200);
                    servo.velocity = 0;
                    app.delay(1000);
                }
            }
        }
    }
}

void adjustment(App) {
    while (1) {
        app.delay(10);
        if (distanceSensor.val[0] <= 80) {
            servo.velocity = -30;
        }
        if (distanceSensor.val[9] <= 120) {
            servo.driveAngularVelocity(50, 45);
        }
        if (distanceSensor.val[3] <= 120) {
            servo.driveAngularVelocity(50, -45);
        }
    }
}

void rightWall(App) {
    app.delay(500);
    while (1) {
        servo.velocity = 30;
        app.delay(10);
        while (count == 1) {
            app.delay(1000);
            count = 2;
        }

        while (count == 0) {
            val6  = distanceSensor.val[6];
            count = 2;
            app.delay(10);
        }

        if (distanceSensor.val[3] > 200) {
            count = 2;
            if ((val6 + 120) < distanceSensor.val[6]) {
                servo.velocity = 0;
                servo.angle += 90;
                app.delay(1000);
                servo.velocity = 30;
                app.delay(3500);
                // static int val0 = distanceSensor.val[0];
                // servo.velocity  = 30;
                // if ((val0 - 150) > distanceSensor.val[0]) {
                //     servo.velocity = 0;
                //     servo.angle += 90;
                //     app.delay(1000);
                //     servo.velocity   = 30;
                // }
            }

        } else if ((distanceSensor.val[0] < 180) &&
                   (distanceSensor.val[3] < 230)) {
            servo.velocity = 0;
            app.delay(500);
            servo.angle -= 90;
            count = 1;
            app.delay(500);
        } else {
            count = 0;
            app.delay(10);
        }
    }
}

void leftWall(App) {
    while (1) {
        servo.velocity = 30;
        app.delay(1000);

        if (distanceSensor.val[9] > 200) {
            static int val6 = distanceSensor.val[6];
            if ((val6 + 70) < distanceSensor.val[6]) {
                servo.velocity = 0;
                servo.angle -= 90;
                app.delay(1000);
                servo.velocity = 30;
                app.delay(4200);
                // static int val0 = distanceSensor.val[0];
                // servo.velocity  = 30;
                // if ((val0 - 150) > distanceSensor.val[0]) {
                //     servo.velocity = 0;
                //     servo.angle += 90;
                //     app.delay(1000);
                //     servo.velocity   = 30;
                // }
            }

        } else if ((distanceSensor.val[0] < 180) &&
                   (distanceSensor.val[9] < 230)) {
            app.stop(adjustment);
            servo.velocity = 0;
            app.delay(500);
            servo.angle += 90;
            app.delay(500);
            app.start(adjustment);
        }
    }
}

void monitor(App) {
    while (1) {
        uart3.print(floorSensor.redVal);
        uart3.print(" ");
        uart3.print(val6);
        uart3.print(" ");
        uart3.println(count);
        app.delay(10);
    }
}

void black(App) {
    while (1) {
        if (floorSensor.redVal > 150) {
            count          = 1;
            servo.velocity = -100;
            app.delay(10);
            if (servo.velocity == -100) {
                servo.angle += 180;
                app.delay(10);
            }
        } else {
            app.delay(1);
        }
    }
}

#endif