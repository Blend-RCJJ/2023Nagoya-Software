#ifndef _SENSOR_APP_H
#define _SENSOR_APP_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

extern HardwareSerial uart1;
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

        // app.delay(10);
    }
}

void slamApp(App) {
    while (1) {
        slam.updatePosition(distanceSensor.val, gyro.deg);
        // app.delay(10);

        // uart1.print("dx:");
        // uart1.print(slam.dx);
        // uart1.print("\tdy:");
        // uart1.print(slam.dy);

        // uart1.print("\tx:");
        // uart1.print(slam.x);
        // uart1.print("\ty:");
        // uart1.print(slam.y);

        uart1.write('F');
        uart1.write('l');
        for (int i = 0; i < 12; i++) {
            uart1.write(highByte(distanceSensor.val[i]));
            uart1.write(lowByte(distanceSensor.val[i]));
        }
        app.delay(5);

        for (int i = 0; i < 6; i++) {
            if (slam.reliableness[i]) {
                stripTop.setPixelColor(i * 2, led.green);
                stripTop.setPixelColor(i * 2 + 12, led.green);
            } else {
                stripTop.setPixelColor(i * 2, led.blank);
                stripTop.setPixelColor(i * 2 + 12, led.blank);
            }
        }

        led.show();

        // uart1.println("");
    }
}

#endif