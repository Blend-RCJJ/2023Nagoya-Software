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
            location.mapData[tempX][tempY].isPassed = true;
            location.mapData[tempX][tempY].isDetected = true;
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

#endif