#ifndef _SENSOR_APP_H
#define _SENSOR_APP_H
#define FLOOR_NUMBER 2

#define SPEED 50

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

extern HardwareSerial uart1;
extern HardwareSerial uart3;
extern RTOS_Kit app;
extern Location_Kit location;

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

extern MAP_Kit mapData[100];

extern void mapApp(App);
extern void sideLEDApp(App);
extern void locationApp(App);

int count      = 0;
int val0       = 0;
int val6       = 0;
int correction = 0;

void camera(App);
void rightwall(App);
void visualization(App);

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
        app.delay(3);
    }
}

void servoApp(App) {
    while (1) {
        servo.drive(servo.velocity, servo.angle + correction);
        app.delay(10);
    }
}

void gridSpecification(App) {
    while (1) {
        if ((distanceSensor.val[0] > distanceSensor.val[3]) &&
            (distanceSensor.val[0] > distanceSensor.val[9])) {
            servo.velocity = SPEED;
            app.delay(4200);
            servo.velocity = 0;
            app.delay(1000);
        } else if (distanceSensor.val[3] > distanceSensor.val[9]) {
            servo.velocity = 0;
            servo.angle += 90;
            app.delay(1000);
            servo.velocity = SPEED;
            app.delay(4200);
            servo.velocity = 0;
            app.delay(1000);
        } else if (distanceSensor.val[3] < distanceSensor.val[9]) {
            servo.velocity = 0;
            servo.angle -= 90;
            app.delay(1000);
            servo.velocity = SPEED;
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
                    servo.velocity = SPEED;
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
        app.delay(1);

        if (count == 1) {
            app.delay(1200);
            count = 0;
        }

        while (count == 0) {
            if (distanceSensor.val[0] < 110) {
                servo.velocity = 0;
            }
            if (distanceSensor.val[3] + distanceSensor.val[9] < 300) {
                if (distanceSensor.val[3] > distanceSensor.val[9]) {
                    correction = 5;
                }
                if (distanceSensor.val[9] > distanceSensor.val[3]) {
                    correction = -5;
                }
                app.delay(50);
            } else {
                if (distanceSensor.val[9] < 100) {
                    correction = 5;
                }
                if (distanceSensor.val[3] < 100) {
                    correction = -5;
                }
            }

            if (distanceSensor.val[3] > 300 && distanceSensor.val[9] < 300) {
                if (distanceSensor.val[9] > 120) {
                    correction = -5;
                }

            } else if (distanceSensor.val[9] > 300 &&
                       distanceSensor.val[3] < 200) {
                if (distanceSensor.val[3] > 120) {
                    correction = 5;
                }
            } else {
                correction = 0;
            }

            if ((distanceSensor.val[0] > 200) &&
                (distanceSensor.val[0] < 250)) {
                servo.velocity = SPEED;

                correction = 0;
            }
            app.delay(20);
        }
    }
}
// 2900で1マス

void rightGrid(App) {
    app.delay(100);
    while (1) {
        if ((distanceSensor.val[0] < 200) && (distanceSensor.val[3] < 250)) {
            app.delay(1000);
            servo.velocity = 0;
            count          = 1;
            servo.stop();
            servo.angle -= 90;
            app.delay(1800);
        } else if (distanceSensor.val[3] > 300) {
            servo.velocity = 0;
            count          = 1;
            servo.stop();
            servo.angle += 90;
            app.delay(1000);
            servo.velocity = SPEED;
            app.delay(2604);
            servo.velocity = 0;
            app.delay(2000);
        } else {
            app.delay(1000);
            servo.velocity = SPEED;
            app.start(adjustment);
            app.delay(2604);
            servo.velocity = 0;
            app.delay(2000);
        }

        if (location.x == 0 && location.y == 0 && distanceSensor.val[0] < 150) {
            if (millis() > 30000) {
                servo.velocity = 0;
                app.stop(servoApp);
                app.stop(adjustment);
                servo.stop();
                app.stop(camera);
                app.stop(visualization);
                led.setTopColor(led.white);
                led.setLeftColor(led.white);
                led.setRightColor(led.white);
                led.setUIColor(led.white);
                led.show();
            } else {
                app.delay(1);
            }
        }
    }
}

void rightWall(App) {
    app.delay(500);
    while (1) {
        servo.velocity = SPEED;
        app.delay(10);
        // app.delay(2700);
        // servo.velocity = 0;
        // app.delay(1000);

        if (location.x == 0 && location.y == 0 && distanceSensor.val[0] < 180) {
            if (millis() > 30000) {
                servo.velocity = 0;
                app.stop(servoApp);
                app.stop(adjustment);
                servo.stop();
                app.stop(camera);
                app.stop(visualization);
                led.setTopColor(led.white);
                led.setLeftColor(led.white);
                led.setRightColor(led.white);
                led.setUIColor(led.white);
                led.show();
                app.delay(1000);

                // speaker.matsukenShogun();
            } else {
                app.delay(10);
            }
        }

        while (count == 1) {
            app.delay(900);
            count = 0;
        }

        while (count == 0) {
            val6  = distanceSensor.val[6];
            val0  = distanceSensor.val[0];
            count = 2;
            app.delay(10);
        }

        if (distanceSensor.val[3] > 200) {
            count = 2;
            if (val6 > 600) {
                if ((val0 - 150) > distanceSensor.val[0]) {
                    servo.velocity = 0;
                    servo.stop();
                    app.delay(500);
                    servo.angle += 90;
                    count = 0;
                    app.delay(500);
                    servo.velocity = SPEED;
                    app.delay(1700);
                }
            } else if ((val6 + 150) < distanceSensor.val[6]) {
                servo.velocity = 0;
                servo.stop();
                app.delay(500);
                servo.angle += 90;
                count = 0;
                app.delay(500);
                servo.velocity = SPEED;
                app.delay(1700);
            }

        } else {
            count = 0;
            app.delay(10);
        }

        if (distanceSensor.val[0] < 110 && gyro.slope == 0) {
            servo.velocity = 0;
            servo.stop();
            app.delay(500);
            servo.angle -= 90;
            count = 1;
            app.delay(500);
        }

        if ((!location
                  .mapData[location.x + MAP_ORIGIN + 1][location.y + MAP_ORIGIN]
                  .isPassed) &&
            (!location
                  .mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN + 1]
                  .isPassed)) {
            app.delay(50);  // １つ先のx も　y も未探索なら放置
        }

        if ((!location
                  .mapData[location.x + MAP_ORIGIN + 1][location.y + MAP_ORIGIN]
                  .isPassed) &&
            (location
                 .mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN + 1]
                 .isPassed) &&
            (location
                 .mapData[location.x + MAP_ORIGIN - 1][location.y + MAP_ORIGIN]
                 .isPassed) &&
            (location
                 .mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN - 1]
                 .isPassed)) {
            if (distanceSensor.val[3] < 200) {
                servo.velocity = 0;
                servo.stop();
                app.delay(500);
                servo.angle -= 90;
                count = 1;
                app.delay(500);
                servo.velocity = SPEED;
                app.delay(1700);
            }
        }
    }
}

void hitAvoid(App) {
    static bool oldStatus = false;
    while (1) {
        if (distanceSensor.val[0] > 300 && distanceSensor.val[1] < 100 &&
            distanceSensor.val[2] < 150) {
            app.stop(servoApp);
            app.stop(rightWall);
            app.stop(adjustment);
            servo.driveAngularVelocity(-30, 45);
            app.delay(1000);

            oldStatus = false;
        }
        if (distanceSensor.val[0] > 300 && distanceSensor.val[11] < 200 &&
            distanceSensor.val[10] < 100) {
            app.stop(servoApp);
            app.stop(rightWall);
            app.stop(adjustment);
            servo.driveAngularVelocity(-30, -45);
            app.delay(1000);

            oldStatus = false;
        }
        if (!oldStatus) {
            app.start(servoApp);
            app.start(rightWall);
            app.start(adjustment);

            oldStatus = true;
        }

        app.delay(20);
    }
    app.delay(10);
}

void leftWall(App) {
    app.delay(500);
    while (1) {
        servo.velocity = SPEED;
        app.delay(10);

        while (count == 1) {
            app.delay(800);
            count = 2;
        }

        while (count == 0) {
            val6  = distanceSensor.val[6];
            count = 2;
            app.delay(10);
        }

        if (distanceSensor.val[9] > 200) {
            count = 2;
            if (val6 > 600) {
                val6 = distanceSensor.val[0];
                if ((val6 - 140) > distanceSensor.val[0]) {
                    servo.velocity = 0;
                    servo.angle -= 90;
                    app.delay(1000);
                    servo.velocity = 30;
                    app.delay(1000);
                }
            } else if ((val6 + 140) < distanceSensor.val[6]) {
                servo.velocity = 0;
                servo.angle -= 90;
                app.delay(1000);
                servo.velocity = 30;
                app.delay(3500);
            }

        } else if ((distanceSensor.val[0] < 120) &&
                   (distanceSensor.val[9] < 230)) {
            app.stop(adjustment);
            servo.velocity = 0;
            app.delay(500);
            servo.angle += 90;
            count = 1;
            app.delay(500);
            app.start(adjustment);
        } else {
            count = 0;
            app.delay(10);
        }
    }
}

void monitor(App) {
    while (1) {
        // uart3.write(cameraRight.data);
        // uart3.println(" ");

        uart3.print(location.x);
        uart3.print(" ");
        uart3.print(location.y);
        uart3.print(" ");
        uart3.print((int)location.coordinateX);
        uart3.print(" ");
        uart3.print((int)location.coordinateY);
        uart3.print(" ");
        uart3.print((int)servo.rightWheelSpeed);
        uart3.println("");

        // for (int i = 13; i < 27; i++) {      // たて
        //     for (int j = 13; j < 27; j++) {  // 横
        //         if (location.mapData[j][39 - i].isPassed == true) {
        //             uart1.print("■");
        //         } else if (location.mapData[j][39 - i].isDetected == true) {
        //             uart1.print("□");
        //         } else {
        //             uart1.print(" ");
        //         }
        //     }

        //     uart1.println("");
        // }

        app.delay(1000);

        // uart3.print(distanceSensor.val[0]);
        // uart3.print(" ");
        // uart3.print(distanceSensor.val[1]);
        // uart3.print(" ");
        // uart3.println(distanceSensor.val[2]);
        // app.delay(30);
    }
}

void black(App) {
    while (1) {
        if (floorSensor.redVal > 150) {
            count          = 1;
            servo.velocity = -100;
            led.setTopColor(led.red);
            led.show();
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

void visualization(App) {
    while (1) {
        for (int i = 0; i < 24; i += 2) {
            stripTop.setPixelColor(i, (30000 / distanceSensor.val[i / 2]),
                                   (30000 / distanceSensor.val[i / 2]),
                                   (30000 / distanceSensor.val[i / 2]));
        }

        for (int j = 1; j <= 12; j++) {
            stripTop.setPixelColor(
                2 * j - 1,
                30000 /
                    ((distanceSensor.val[j - 1] + distanceSensor.val[j]) / 2),
                30000 /
                    ((distanceSensor.val[j - 1] + distanceSensor.val[j]) / 2),
                30000 /
                    ((distanceSensor.val[j - 1] + distanceSensor.val[j]) / 2));
        }

        led.show();
        app.delay(10);
    }
}

void camera(App) {
    bool oldstatus = true;
    while (1) {
        while (cameraRight.data == 'H') {
            led.setUIColor(led.pink);
            servo.velocity = 0;
            app.stop(servoApp);
            servo.stop();
            app.stop(adjustment);
            app.stop(rightWall);
            led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
            led.show();
            oldstatus = false;

            app.delay(5);
        }

        while (cameraRight.data == 'S') {
            led.setUIColor(led.blue);
            servo.velocity = 0;
            servo.stop();
            app.stop(servoApp);
            app.stop(adjustment);
            app.stop(rightWall);
            led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
            led.show();
            oldstatus = false;

            app.delay(5);
        }

        while (cameraRight.data == 'U') {
            led.setUIColor(led.white);
            servo.velocity = 0;
            servo.stop();
            app.stop(servoApp);
            app.stop(adjustment);
            app.stop(rightWall);
            led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
            led.show();
            oldstatus = false;

            app.delay(5);
        }

        while (cameraRight.data == 'R') {
            led.setUIColor(led.red);
            servo.velocity = 0;
            servo.stop();
            app.stop(servoApp);
            app.stop(adjustment);
            app.stop(rightWall);
            led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
            led.show();
            oldstatus = false;

            app.delay(5);
        }

        while (cameraRight.data == 'G') {
            led.setUIColor(led.green);
            servo.velocity = 0;
            servo.stop();
            app.stop(servoApp);
            app.stop(adjustment);
            app.stop(rightWall);
            led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
            led.show();
            oldstatus = false;

            app.delay(5);
        }

        while (cameraRight.data == 'Y') {
            led.setUIColor(led.yellow);
            servo.velocity = 0;
            servo.stop();
            app.stop(servoApp);
            app.stop(adjustment);
            app.stop(rightWall);
            led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
            led.show();
            oldstatus = false;

            app.delay(5);
        }

        if (!oldstatus) {
            app.start(servoApp);
            app.start(rightWall);
            app.start(adjustment);
            app.delay(5);

            oldstatus = true;
        }

        led.setUIColor(led.blank);
        led.show();
        app.delay(100);
    }
}

void lever(App) {
    static bool oldStatus = false;
    while (1) {
        if (ui.toggle == false) {
            led.setTopColor(led.green);
            led.show();
            servo.velocity = 0;
            app.stop(servoApp);
            app.stop(rightWall);
            app.stop(adjustment);
            app.stop(visualization);
            app.stop(camera);

            app.stop(locationApp);
            app.stop(mapApp);
            app.stop(sideLEDApp);
            servo.stop();
            oldStatus = false;
        } else {
            if (!oldStatus) {
                app.start(servoApp);
                app.start(rightWall);
                app.start(adjustment);
                app.start(visualization);
                app.start(camera);

                app.start(locationApp);
                app.start(mapApp);
                app.start(sideLEDApp);
                gyro.setOffset();

                oldStatus = true;
            }
        }
        app.delay(10);
    }
}
#endif