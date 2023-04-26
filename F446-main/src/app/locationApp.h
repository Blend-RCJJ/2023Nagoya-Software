#ifndef _LOCATION_APP_H
#define _LOCATION_APP_H

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

extern void rightWall(App);
extern void leftWall(App);
extern void wallCondition(App);
extern void adjustment(App);
extern void servoApp(App);
extern void victimApp(App);
extern void adjustment(App);

extern void ABARENBO_SHOGUN_MATSUKEN_LOVE(void);

extern bool NorthWall;
extern bool EastWall;
extern bool SouthWall;
extern bool WestWall;

#define MAX_DISTANCE 800
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

void sideLEDApp(App) {
    while (1) {
        if (millis() - location.lastCorrection <= 1000) {
            if ((millis() / 50 % 2) && (millis() / 300 % 4) == 1) {
                led.setLeftColor(led.white);
                led.setRightColor(led.white);
            } else {
                led.setLeftColor(led.blank);
                led.setRightColor(led.blank);
            }
        } else {
            led.setLeftColor(led.blank);
            led.setRightColor(led.blank);
        }

        app.delay(10);
    }
}

void mapApp(App) {
    while (1) {
        int tempX = constrain(location.x + MAP_ORIGIN, 3, MAP_ORIGIN * 2 - 3);

        int tempY = constrain(location.y + MAP_ORIGIN, 3, MAP_ORIGIN * 2 - 3);

        if (location.mapData[tempX][tempY].isPassed == false) {
            location.mapData[tempX][tempY].isPassed        = true;
            location.mapData[tempX][tempY].isDetected      = true;
            location.mapData[tempX][tempY].firstPassedTime = millis();
        }

        if ((millis() - location.mapData[tempX][tempY].firstPassedTime) <
                3000 &&
            (millis() - location.mapData[tempX][tempY].firstPassedTime) >
                1000 &&
            abs(gyro.slope) == 0) {
            int judgeGain[2] = {400, 240};

            if (millis() - location.lastTrustY <= 20000) {
                if (distanceSensor.val[location.minIndex] > judgeGain[0] &&
                    distanceSensor.val[(location.minIndex + 1) % 12] >
                        judgeGain[1] &&
                    distanceSensor.val[(location.minIndex + 15) % 12] >
                        judgeGain[1]) {
                    location.mapData[tempX][tempY + 1].isDetected = true;
                }

                if (distanceSensor.val[(location.minIndex + 6) % 12] >
                        judgeGain[0] &&
                    distanceSensor.val[(location.minIndex + 5) % 12] >
                        judgeGain[1] &&
                    distanceSensor.val[(location.minIndex + 7) % 12] >
                        judgeGain[1]) {
                    location.mapData[tempX][tempY - 1].isDetected = true;
                }
            }

            if (millis() - location.lastTrustX <= 20000) {
                if (distanceSensor.val[(location.minIndex + 3) % 12] >
                        judgeGain[0] &&
                    distanceSensor.val[(location.minIndex + 4) % 12] >
                        judgeGain[1] &&
                    distanceSensor.val[(location.minIndex + 2) % 12] >
                        judgeGain[1]) {
                    location.mapData[tempX + 1][tempY].isDetected = true;
                }
                if (distanceSensor.val[(location.minIndex + 9) % 12] >
                        judgeGain[0] &&
                    distanceSensor.val[(location.minIndex + 10) % 12] >
                        judgeGain[1] &&
                    distanceSensor.val[(location.minIndex + 8) % 12] >
                        judgeGain[1]) {
                    location.mapData[tempX - 1][tempY].isDetected = true;
                }
            }
        }

        app.delay(10);
    }
}

void locationApp(App) {
    while (1) {
        for (int i = 0; i < 100; i++) {
            location.updateOdometory(servo.rightWheelSpeed,
                                     servo.leftWheelSpeed, gyro.deg,
                                     gyro.slope);

            app.delay(location.period);
        }

        location.updateObservationData(distanceSensor.vecX, distanceSensor.vecY,
                                       gyro.deg);
    }
}

void Astar(App) {
    app.delay(500);
    int Ndistance = MAX_DISTANCE;
    int Edistance = MAX_DISTANCE;
    int Sdistance = MAX_DISTANCE;
    int Wdistance = MAX_DISTANCE;  // 値の初期化(最大値に設定)
    bool status   = true;
    app.delay(500);
    const int initialWall[4] = {(NorthWall), (EastWall), (SouthWall),
                                (WestWall)};  //(0,0)の壁の状態を記憶
    while (1) {
        app.delay(100);
        if (millis() > 30000 && servo.velocity == 50) {
            if (status) {
                servo.velocity = 0;
                app.stop(victimApp);
                app.stop(rightWall);
                app.stop(leftWall);
                // app.stop(adjustment);
                app.delay(2000);
                status = false;
            }
            led.setTopColor(led.white);
            led.show();
            app.delay(10);
        MEASURE_DISTANCE:
            if (!location.x && !location.y && initialWall[NORTH] == NorthWall &&
                initialWall[EAST] == EastWall &&
                initialWall[SOUTH] == SouthWall &&
                initialWall[WEST] == WestWall) {
                servo.velocity = 0;
                servo.stop();
                app.stop(rightWall);
                app.stop(servoApp);
                app.stop(adjustment);
                ABARENBO_SHOGUN_MATSUKEN_LOVE();
                app.delay(20000);
            }

            if (!NorthWall) {
                Ndistance = location.x * location.x +
                            (location.y + 1) * (location.y + 1);
            } else {
                Ndistance = MAX_DISTANCE;
            }
            if (!EastWall) {
                Edistance = (location.x + 1) * (location.x + 1) +
                            location.y * location.y;
            } else {
                Edistance = MAX_DISTANCE;
            }
            if (!SouthWall) {
                Sdistance = location.x * location.x +
                            (location.y - 1) * (location.y - 1);
            } else {
                Sdistance = MAX_DISTANCE;
            }
            if (!WestWall) {
                Wdistance = (location.x - 1) * (location.x - 1) +
                            location.y * location.y;
            } else {
                Wdistance = MAX_DISTANCE;
            }
        MOVE_COORDINATE:
            if (Ndistance < Edistance && Ndistance < Sdistance &&
                Ndistance < Wdistance) {
                servo.angle    = 0;
                servo.velocity = 50;
                app.delay(2900);
                servo.velocity = 0;
                app.delay(1000);
                SouthWall = true;
                goto MEASURE_DISTANCE;

            } else if (Sdistance < Edistance && Sdistance < Wdistance) {
                servo.angle    = 180;
                servo.velocity = 50;
                app.delay(2900);
                servo.velocity = 0;
                app.delay(1000);
                NorthWall = true;
                goto MEASURE_DISTANCE;

            } else if (Edistance < Wdistance) {
                servo.angle    = 90;
                servo.velocity = 50;
                app.delay(2900);
                servo.velocity = 0;
                app.delay(1000);
                WestWall = true;
                goto MEASURE_DISTANCE;

            } else {
                servo.angle    = 270;
                servo.velocity = 50;
                app.delay(2900);
                servo.velocity = 0;
                app.delay(1000);
                EastWall = true;
                goto MEASURE_DISTANCE;
            }
        } else {
            app.delay(10);
        }
    }
}

void DFS(App) {
    while (1) {
        app.delay(100);
    }
}

#endif