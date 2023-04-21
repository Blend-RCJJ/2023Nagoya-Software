#ifndef _SENSOR_APP_H
#define _SENSOR_APP_H
#define FLOOR_NUMBER 2

#define SPEED 50
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

bool HidariWALL = false;

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
extern void Astar(App);

int count                                               = 0;
int val0                                                = 0;
int val6                                                = 0;
int correction                                          = 0;
bool NorthWall                                          = false;
bool EastWall                                           = false;
bool SouthWall                                          = false;
bool WestWall                                           = false;
// bool coordinate_Wall[MAP_ORIGIN * 2][MAP_ORIGIN * 2][4] = {0};

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
            }
            if (distanceSensor.val[9] > 300 && distanceSensor.val[3] < 300) {
                if (distanceSensor.val[3] > 140) {
                    correction = 7;
                }
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

// void extensionRight(App) {
//     while (1) {
//         if (gyro.deg > 350 || gyro.deg < 10) {
//         }
//         if (gyro.deg > 80 && gyro.deg < 100) {
//         }
//         if (gyro.deg > 170 && gyro.deg < 190) {
//             if (location
//                     .mapData[location.x + MAP_ORIGIN]
//                             [location.y + MAP_ORIGIN - 1]
//                     .isPassed) {
//                 servo.angle = 90;
//             }
//         }
//         if (gyro.deg > 260 && gyro.deg < 280) {
//         }
//     }
// }

void wallCondition(App) {
    app.delay(1000);
    while (1) {
        // coordinate_Wall[location.x + MAP_ORIGIN][location.y +MAP_ORIGIN][NORTH] = NorthWall;
        // coordinate_Wall[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN][EAST]  = EastWall;
        // coordinate_Wall[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN][SOUTH] = SouthWall;
        // coordinate_Wall[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN][WEST]  = WestWall;
        app.delay(100);
        if (gyro.deg > 350 || gyro.deg < 10) {
            if (distanceSensor.val[0] > 250) {
                NorthWall = false;
            } else {
                NorthWall = true;
            }

            if (distanceSensor.val[3] > 250) {
                EastWall = false;
            } else {
                EastWall = true;
            }

            if (distanceSensor.val[6] > 250) {
                SouthWall = false;
            } else {
                SouthWall = true;
            }

            if (distanceSensor.val[9] > 250) {
                WestWall = false;
            } else {
                WestWall = true;
            }
        } else if (gyro.deg > 80 && gyro.deg < 100) {
            if (distanceSensor.val[9] > 280) {
                NorthWall = false;
            } else {
                NorthWall = true;
            }

            if (distanceSensor.val[0] > 280) {
                EastWall = false;
            } else {
                EastWall = true;
            }

            if (distanceSensor.val[3] > 280) {
                SouthWall = false;
            } else {
                SouthWall = true;
            }

            if (distanceSensor.val[6] > 280) {
                WestWall = false;
            } else {
                WestWall = true;
            }
        } else if (gyro.deg > 170 && gyro.deg < 190) {
            if (distanceSensor.val[6] > 280) {
                NorthWall = false;
            } else {
                NorthWall = true;
            }

            if (distanceSensor.val[9] > 280) {
                EastWall = false;
            } else {
                EastWall = true;
            }

            if (distanceSensor.val[0] > 280) {
                SouthWall = false;
            } else {
                SouthWall = true;
            }

            if (distanceSensor.val[3] > 280) {
                WestWall = false;
            } else {
                WestWall = true;
            }
        } else if (gyro.deg > 260 && gyro.deg < 280) {
            if (distanceSensor.val[3] > 280) {
                NorthWall = false;
            } else {
                NorthWall = true;
            }

            if (distanceSensor.val[6] > 280) {
                EastWall = false;
            } else {
                EastWall = true;
            }

            if (distanceSensor.val[9] > 280) {
                SouthWall = false;
            } else {
                SouthWall = true;
            }

            if (distanceSensor.val[0] > 280) {
                WestWall = false;
            } else {
                WestWall = true;
            }
        }else{
            app.delay(100);
        }
    }
}

void rightWall(App) {
    while (1) {
        // led.setTopColor(led.blue);
        // led.show();
        servo.velocity = SPEED;
        app.delay(10);

        while (count == 1) {
            app.delay(1200);
            count = 0;
        }

        if (distanceSensor.val[3] > 300 && distanceSensor.val[4] > 250 &&
            !gyro.slope) {
            servo.velocity = 0;
            servo.stop();
            app.delay(500);
            count = 1;
            servo.angle += 90;
            servo.velocity = 0;
            servo.stop();
            app.delay(500);
            servo.velocity = SPEED;
            app.delay(1800);
        }

        if (distanceSensor.val[0] < 140 && !gyro.slope) {
            servo.velocity = 0;
            servo.stop();
            app.delay(500);
            count = 1;
            servo.angle -= 90;
            app.delay(500);
        }
    }
}

void leftWall(App) {
    app.delay(500);
    while (1) {
        servo.velocity = SPEED;
        app.delay(10);

        while (count == 1) {
            app.delay(1200);
            count = 0;
        }

        if (distanceSensor.val[9] > 300 && distanceSensor.val[8] > 250 &&
            !gyro.slope) {
            count = 2;

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

        if (distanceSensor.val[0] < 140 && !gyro.slope) {
            servo.velocity = 0;
            servo.stop();
            app.delay(500);
            count = 1;
            servo.angle += 90;
            app.delay(500);
        }
    }
}

void hitAvoid(App) {
    static bool oldStatus = false;
    while (1) {
        app.delay(20);
        if (loadcell.status == RIGHT) {
            app.stop(servoApp);
            // app.stop(searchRight);
            app.stop(adjustment);
            count = 1;
            servo.driveAngularVelocity(-30, 45);
            app.delay(500);
            servo.driveAngularVelocity(-30, -45);
            app.delay(500);

            oldStatus = false;
        }
        if (loadcell.status == LEFT) {
            app.stop(servoApp);
            // app.stop(searchRight);
            app.stop(adjustment);
            count = 1;
            servo.driveAngularVelocity(-30, -45);
            app.delay(500);
            servo.driveAngularVelocity(-30, 45);
            app.delay(500);

            oldStatus = false;
        }
        if (!oldStatus) {
            servo.velocity = SPEED;
            // app.start(rightWall);
            // app.start(searchRight);
            app.start(servoApp);
            app.start(adjustment);

            oldStatus = true;
        }
    }
}

void monitor(App) {
    while (1) {
        // uart3.print(location.x);
        // uart3.print(",");
        // uart3.print(location.y);

        // if (NorthWall) {
        //     uart3.print("北　");
        // }

        // if (EastWall) {
        //     uart3.print("東　");
        // }

        // if (SouthWall) {
        //     uart3.print("南　");
        // }

        // if (WestWall) {
        //     uart3.print("西　");
        // }

        // if (NorthWall || EastWall || SouthWall || WestWall) {
        //     uart3.println("に壁");
        //     app.delay(100);
        // } else {
        //     uart3.println("壁がありません");
        //     app.delay(100);
        // }

        uart3.print(distanceSensor.val[3]);
        uart3.print(" ");
        uart3.print(distanceSensor.val[4]);
        uart3.print(" ");
        uart3.print(distanceSensor.val[5]);
        uart3.println(" ");
        app.delay(500);
    }
}

void black(App) {
    bool oldstatus = true;
    while (1) {
        if (floorSensor.blueVal > 500) {
            app.stop(rightWall);
            app.stop(leftWall);
            app.stop(adjustment);
            servo.velocity = -SPEED;
            app.delay(1500);
            servo.velocity = 0;
            servo.stop();
            app.delay(500);

            servo.angle -= 90;
            servo.velocity = SPEED;
            app.delay(1500);
            oldstatus = false;
        }
        if (!oldstatus) {
            servo.velocity = SPEED;
            app.start(rightWall);
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
                if (distanceSensor.val[3] < 160) {
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
                        servo.rescueKit(3, RIGHT);
                    } else if (buff == 'S') {
                        servo.rescueKit(2, RIGHT);
                    } else if (buff == 'R' || buff == 'Y') {
                        servo.rescueKit(1, RIGHT);
                    }

                    app.delay(5);

                    location
                        .mapData[location.x + MAP_ORIGIN]
                                [location.y + MAP_ORIGIN]
                        .isVictimDetected = true;

                    servo.velocity = SPEED;
                    app.start(rightWall);
                    app.start(servoApp);
                    app.start(adjustment);

                    app.delay(5);

                    led.setUIColor(led.blank);
                    led.show();
                } else {
                    ignoreTimer = millis();
                }
            }

            if (cameraLeft.data != 'N') {  // 反応
                if (distanceSensor.val[9] < 160) {
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

                    servo.velocity = SPEED;
                    app.start(rightWall);
                    app.start(servoApp);
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
            if (heatSensor.r && distanceSensor.val[3] < 160) {
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

            if (heatSensor.l && distanceSensor.val[9] < 160) {
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

            servo.velocity = SPEED;
            app.start(rightWall);
            app.start(servoApp);
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
    static bool mochinyamu = false;
    while (1) {
        if (ui.toggle == false) {
            // speaker.validationSound();
            led.setTopColor(led.green);
            led.show();
            servo.velocity = 0;
            app.stop(servoApp);
            app.stop(rightWall);
            app.stop(adjustment);
            app.stop(visualization);
            app.stop(black);
            app.stop(camera);
            app.stop(hitAvoid);
            // app.stop(searchRight);

            app.stop(locationApp);
            app.stop(mapApp);
            app.stop(victimApp);
            app.stop(sideLEDApp);
            // app.stop(locationMapping);
            servo.stop();
            mochinyamu = false;
        } else {
            if (!mochinyamu) {
                speaker.checkSound();
                app.start(servoApp);
                app.start(rightWall);
                app.start(adjustment);
                app.start(visualization);
                app.start(black);
                app.start(camera);
                app.start(hitAvoid);
                // app.start(searchRight);

                app.start(victimApp);

                app.start(locationApp);
                app.start(mapApp);
                app.start(sideLEDApp);
                // app.start(locationMapping);
                gyro.setOffset();

                mochinyamu = true;

                app.delay(5000);

                // if (distanceSensor.val[9] < 180) {
                //     HidariWALL = true;
                // } else {
                //     HidariWALL = false;
                // }
            }
            app.delay(10);
        }
        app.delay(10);
    }
}
#endif