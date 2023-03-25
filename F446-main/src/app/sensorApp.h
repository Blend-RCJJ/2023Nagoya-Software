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

extern LOADCELL loadcell;
extern HEATSENSOR heatSensor;

extern void mapApp(App);
extern void sideLEDApp(App);
extern void locationApp(App);

int count      = 0;
int val0       = 0;
int val6       = 0;
int correction = 0;
int Seed       = 1;

void camera(App);
void rightWall(App);
void visualization(App);

void inputMonitoringApp(App) {
    while (1) {
        distanceSensor.getDistance();
        distanceSensor.calcVector();

        gyro.read();
        ui.read();
        loadcell.read();
        heatSensor.read();

        led.setFloorColor(led.white);
        app.delay(5);
        floorSensor.blueVal = analogRead(PC0);

        led.setFloorColor(led.blank);
        app.delay(5);
        floorSensor.greenVal = analogRead(PC0);

        // led.setFloorColor(led.green);
        // app.delay(3);
        // floorSensor.greenVal = analogRead(PC0);

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

void adjustment(App) {
    while (1) {
        app.delay(10);

        if (count == 1) {
            app.delay(900);
            count = 0;
        }

        while (count == 0) {
            if (distanceSensor.val[0] < 110) {
                servo.velocity = 0;
            }
            if (distanceSensor.val[3] + distanceSensor.val[9] < 300) {
                if (distanceSensor.val[3] > distanceSensor.val[9]) {
                    correction = 7;
                }
                if (distanceSensor.val[9] > distanceSensor.val[3]) {
                    correction = -7;
                }
                app.delay(50);
            } else {
                if (distanceSensor.val[9] < 120) {
                    correction = 7;
                }
                if (distanceSensor.val[3] < 120) {
                    correction = -7;
                }
            }

            if (distanceSensor.val[3] > 300 && distanceSensor.val[9] < 300) {
                if (distanceSensor.val[9] > 140) {
                    correction = -7;
                }

            } else if (distanceSensor.val[9] > 300 &&
                       distanceSensor.val[3] < 300) {
                if (distanceSensor.val[3] > 140) {
                    correction = 7;
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

void rightWall(App) {
    app.delay(500);
    while (1) {
     
        if (location.x == 0 && location.y == 0 && distanceSensor.val[0] < 180) {
            if (millis() > 300000) {
                servo.velocity = 0;
                app.stop(servoApp);
                app.stop(adjustment);
                servo.stop();
                app.stop(camera);
                app.stop(visualization);
                led.setTopColor(led.white);
                led.setLeftColor(led.white);
                led.setRightColor(led.white);
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

        if (distanceSensor.val[3] > 200 && gyro.slope == 0) {
            count = 2;
            if (val6 > 600) {
                if ((val0 - 130) > distanceSensor.val[0]) {
                    servo.velocity = 0;
                    servo.stop();
                    app.delay(500);
                    servo.angle += 90;
                    servo.velocity = 0;
                    servo.stop();
                    app.delay(500);
                    count          = 0;
                    servo.velocity = SPEED;
                    app.delay(1800);
                }
            } else if ((val6 + 130) < distanceSensor.val[6]) {
                servo.velocity = 0;
                servo.stop();
                app.delay(500);
                servo.angle += 90;
                servo.velocity = 0;
                servo.stop();
                app.delay(500);
                count          = 0;
                servo.velocity = SPEED;
                app.delay(1800);
            }

        } else {
            count = 0;
            app.delay(10);
        }

        if (distanceSensor.val[0] < 120 && gyro.slope == 0) {
            servo.velocity = 0;
            servo.stop();
            app.delay(500);
            servo.angle -= 90;
            count = 1;
            app.delay(1100);
        }
    }
}

void leftWall(App) {
    app.delay(500);
    while (1) {
        servo.velocity = SPEED;
        app.delay(10);
        // app.delay(2700);
        // servo.velocity = 0;
        // app.delay(1000);

        if (location.x == 0 && location.y == 0 && distanceSensor.val[0] < 180) {
            if (millis() > 300000) {
                servo.velocity = 0;
                app.stop(servoApp);
                app.stop(adjustment);
                servo.stop();
                app.stop(camera);
                app.stop(visualization);
                led.setTopColor(led.white);
                led.setLeftColor(led.white);
                led.setRightColor(led.white);
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

        if (distanceSensor.val[9] > 200 && gyro.slope == 0) {
            count = 2;
            if (val6 > 600) {
                if ((val0 - 130) > distanceSensor.val[0]) {
                    servo.velocity = 0;
                    servo.stop();
                    app.delay(500);
                    servo.angle -= 90;
                    servo.velocity = 0;
                    servo.stop();
                    app.delay(500);
                    count          = 0;
                    servo.velocity = SPEED;
                    app.delay(1800);
                }
            } else if ((val6 + 130) < distanceSensor.val[6]) {
                servo.velocity = 0;
                servo.stop();
                app.delay(500);
                servo.angle -= 90;
                servo.velocity = 0;
                servo.stop();
                app.delay(500);
                count          = 0;
                servo.velocity = SPEED;
                app.delay(1800);
            }

        } else {
            count = 0;
            app.delay(10);
        }

        if (distanceSensor.val[0] < 120 && gyro.slope == 0) {
            servo.velocity = 0;
            servo.stop();
            app.delay(500);
            servo.angle += 90;
            count = 1;
            app.delay(1100);
        }
    }
}

void randomSwitching(App) {
    app.delay(500);
    int oldTime    = millis();
    bool oldstatus = false;
    while (1) {
        if (distanceSensor.val[3] < 280 && distanceSensor.val[9] < 280 &&
            gyro.slope == 0) {
            servo.velocity = SPEED;
            Seed           = millis() % 2;
            app.delay(20);
        } else {
            if (oldTime + 10000 < millis()) {
                if (Seed) {
                    app.stop(leftWall);
                    oldstatus = false;
                    app.delay(50);

                    if (!oldstatus) {
                        servo.velocity = SPEED;
                        app.start(rightWall);
                        oldTime = millis();
                        oldstatus = true;
                    }
                } else {
                    app.stop(rightWall);
                    oldstatus = false;
                    if (!oldstatus) {
                        servo.velocity = SPEED;
                        app.start(leftWall);
                        oldTime = millis();
                        oldstatus = true;
                    }
                }
            }
        }
        app.delay(50);
    }
}

void hitAvoid(App) {
    static bool oldStatus = false;
    while (1) {
        if (loadcell.status == RIGHT) {
            app.stop(servoApp);
            app.stop(rightWall);
            app.stop(leftWall);
            app.stop(adjustment);
            servo.driveAngularVelocity(-30, 45);
            app.delay(500);
            servo.driveAngularVelocity(-30, -45);
            app.delay(500);

            oldStatus = false;
        }
        if (loadcell.status == LEFT) {
            app.stop(servoApp);
            app.stop(rightWall);
            app.stop(leftWall);
            app.stop(adjustment);
            servo.driveAngularVelocity(-30, -45);
            app.delay(500);
            servo.driveAngularVelocity(-30, 45);
            app.delay(500);

            oldStatus = false;
        }
        if (!oldStatus) {
            if (Seed) {
                app.start(rightWall);
            } else {
                app.start(leftWall);
            }
            app.start(servoApp);
            app.start(randomSwitching);
            app.start(adjustment);

            oldStatus = true;
        }

        app.delay(20);
    }
    app.delay(10);
}

void monitor(App) {
    while (1) {
        // uart3.write(cameraRight.data);
        // uart3.println(" ");
        uart1.print(loadcell.load[0]);
        uart1.print(" ");
        uart1.println(loadcell.load[1]);
        // uart3.print(location.x);
        // uart3.print(" ");
        // uart3.print(location.y);
        // uart3.print(" ");
        // uart3.print((int)location.coordinateX);
        // uart3.print(" ");
        // uart3.print((int)location.coordinateY);
        // uart3.print(" ");
        // uart3.print((int)servo.rightWheelSpeed);
        // uart3.println("");

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
        // // }
        // uart3.print(floorSensor.redVal);
        // uart3.print(" ");
        // uart3.print(floorSensor.greenVal);
        // uart3.print(" ");
        // uart3.print(floorSensor.blueVal);
        // uart3.println(" ");

        app.delay(100);

        // uart3.print(distanceSensor.val[0]);
        // uart3.print(" ");
        // uart3.print(distanceSensor.val[1]);
        // uart3.print(" ");
        // uart3.println(distanceSensor.val[2]);
        // app.delay(30);
    }
}

void black(App) {
    bool oldstatus = true;
    while (1) {
        if (floorSensor.blueVal > 500) {
            app.stop(rightWall);
            app.stop(leftWall);
            app.stop(adjustment);
            app.stop(randomSwitching);
            servo.velocity = -SPEED;
            app.delay(1500);
            servo.velocity = 0;
            servo.stop();
            app.delay(500);
            if (Seed) {
                servo.angle -= 90;
            } else {
                servo.angle += 90;
            }
            app.delay(1500);
            oldstatus = false;
        }
        if (!oldstatus) {
            if (Seed) {
                app.start(rightWall);
            } else {
                app.start(leftWall);
            }
            app.start(randomSwitching);
            app.start(adjustment);
            app.delay(5);

            oldstatus = true;
        }
        app.delay(50);
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
    unsigned long ignoreTimer = 0;

    while (1) {
        int rescueKitNum = 0;
        int leftOrRight  = 1;
        if (!location.mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]
                 .isVictimDetected) {
            if (cameraRight.data != 'N' && millis() - ignoreTimer >= 3000) {
                if (distanceSensor.val[3] < 200) {
                    led.setUIColor(led.white);
                    led.show();

                    app.stop(servoApp);
                    app.stop(adjustment);
                    app.stop(rightWall);
                    app.stop(leftWall);
                    servo.velocity = 0;
                    servo.stop();

                    int buff = 0;

                    unsigned long tikaTimer = millis();
                    while (millis() - tikaTimer < 5500) {
                        led.setUIColor(led.white);
                        led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
                        led.show();

                        if (cameraRight.data != 'N') {
                            buff = cameraRight.data;
                        }

                        app.delay(5);
                    }

                    if (buff == 'H') {
                        servo.rescueKit(3, LEFT);
                    } else if (buff == 'S') {
                        servo.rescueKit(2, LEFT);
                    } else if (buff == 'R' || buff == 'Y') {
                        servo.rescueKit(1, LEFT);
                    }

                    app.delay(5);

                    location
                        .mapData[location.x + MAP_ORIGIN]
                                [location.y + MAP_ORIGIN]
                        .isVictimDetected = true;

                    if (Seed) {
                        app.start(rightWall);
                    } else {
                        app.start(leftWall);
                    }
                    app.start(servoApp);
                    app.start(randomSwitching);
                    app.start(adjustment);

                    app.delay(5);

                    led.setUIColor(led.blank);
                    led.show();
                } else {
                    ignoreTimer = millis();
                }
            }

            if (cameraLeft.data != 'N') {  // 反応
                if (distanceSensor.val[9] < 200) {
                    led.setUIColor(led.white);
                    led.show();

                    app.stop(servoApp);
                    app.stop(adjustment);
                    app.stop(rightWall);
                    app.stop(leftWall);
                    servo.velocity = 0;
                    servo.stop();

                    int buff = 0;

                    unsigned long tikaTimer = millis();
                    while (millis() - tikaTimer < 5500) {
                        led.setUIColor(led.white);
                        led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
                        led.show();

                        if (cameraLeft.data != 'N') {
                            buff = cameraLeft.data;
                        }

                        app.delay(5);
                    }

                    if (buff == 'H') {
                        servo.rescueKit(3, LEFT);
                    } else if (buff == 'S') {
                        servo.rescueKit(2, LEFT);
                    } else if (buff == 'R ' || buff == 'Y') {
                        servo.rescueKit(1, LEFT);
                    }

                    app.delay(5);

                    location
                        .mapData[location.x + MAP_ORIGIN]
                                [location.y + MAP_ORIGIN]
                        .isVictimDetected = true;

                    if (Seed) {
                        app.start(rightWall);
                    } else {
                        app.start(leftWall);
                    }
                    app.start(servoApp);
                    app.start(randomSwitching);
                    app.start(adjustment);

                    app.delay(5);

                    led.setUIColor(led.blank);
                    led.show();
                } else {
                    ignoreTimer = millis();
                }
            }
        }

        app.delay(10);
    }
}

void victimApp(App) {
    while (1) {
        bool status      = false;
        int rescueKitNum = 0;
        int leftOrRight  = 0;
        if (!location.mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]
                 .isVictimDetected) {
            if (heatSensor.r && distanceSensor.val[3] < 200) {
                unsigned long timer = millis();
                while (millis() - timer < 5500) {
                    led.setUIColor(led.cyan);
                    servo.velocity = 0;
                    app.stop(servoApp);
                    servo.stop();
                    app.stop(adjustment);
                    app.stop(rightWall);
                    app.stop(leftWall);
                    led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
                    led.show();
                    status = true;

                    app.delay(5);

                    rescueKitNum = 1;
                    leftOrRight  = RIGHT;
                }

                location
                    .mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]
                    .isVictimDetected = true;
            }

            if (heatSensor.l && distanceSensor.val[9] < 200) {
                unsigned long timer = millis();
                while (millis() - timer < 5500) {
                    led.setUIColor(led.cyan);
                    servo.velocity = 0;
                    app.stop(servoApp);
                    servo.stop();
                    app.stop(adjustment);
                    app.stop(leftWall);
                    app.stop(rightWall);
                    led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
                    led.show();
                    status = true;

                    app.delay(5);

                    rescueKitNum = 1;
                    leftOrRight  = LEFT;
                }

                location
                    .mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]
                    .isVictimDetected = true;
            }
        }

        if (status) {
            if (rescueKitNum != 0) {
                servo.rescueKit(rescueKitNum, leftOrRight);
            }

            if (Seed) {
                app.start(rightWall);
            } else {
                app.start(leftWall);
            }
            app.start(servoApp);
            app.start(randomSwitching);
            app.start(adjustment);

            app.delay(5);

            status = false;

            led.setUIColor(led.blank);
            led.show();
        }

        app.delay(10);
    }
}

void lever(App) {
    static bool oldStatus = false;
    while (1) {
        if (ui.toggle == false) {
            // speaker.bootSound();
            led.setTopColor(led.green);
            led.show();
            servo.velocity = 0;
            app.stop(servoApp);
            app.stop(rightWall);
            app.stop(adjustment);
            app.stop(visualization);
            app.stop(black);
            app.stop(camera);
            app.stop(randomSwitching);

            app.stop(locationApp);
            app.stop(mapApp);
            app.stop(victimApp);
            app.stop(sideLEDApp);
            // app.stop(locationMapping);
            servo.stop();
            oldStatus = false;
        } else {
            if (!oldStatus) {
                speaker.bootSound();
                app.start(servoApp);
                app.start(rightWall);
                app.start(adjustment);
                app.start(visualization);
                app.start(black);
                app.start(camera);
                app.start(randomSwitching);

                app.start(victimApp);

                app.start(locationApp);
                app.start(mapApp);
                app.start(sideLEDApp);
                // app.start(locationMapping);
                gyro.setOffset();

                oldStatus = true;
            }
        }
        app.delay(10);
    }
}
#endif