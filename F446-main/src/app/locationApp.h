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
        // if (millis() - location.lastCorrection <= 100) {
        //     if ((millis() / 50 % 2)) {
        //         led.setLeftColor(led.white);
        //         led.setRightColor(led.white);
        //     } else {
        //         led.setLeftColor(led.blank);
        //         led.setRightColor(led.blank);
        //     }
        // } else {
        //     led.setLeftColor(led.blank);
        //     led.setRightColor(led.blank);
        // }

        for (int i = 13; i < 27; i++) {      // たて
            for (int j = 13; j < 27; j++) {  // 横
                if (location.mapData[j][39 - i].isPassed == true) {
                    uart1.print("●");
                } else {
                    uart1.print("○");
                }
            }

            uart1.println("");
        }
        app.delay(1000);
    }
}

void mapApp(App) {
    while (1) {
        int tempX = constrain(location.x + MAP_ORIGIN, 0, MAP_ORIGIN * 2 - 1);

        int tempY = constrain(location.y + MAP_ORIGIN, 0, MAP_ORIGIN * 2 - 1);

        if (location.mapData[tempX][tempY].isPassed == false) {
            location.mapData[tempX][tempY].isPassed = true;
            location.mapData[tempX][tempY].isDetected = true;
        }

        app.delay(10);
    }
}

void locationApp(App) {
    while (1) {
        for (int i = 0; i < 50; i++) {
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