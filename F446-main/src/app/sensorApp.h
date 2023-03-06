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
    }
}

void slamApp(App) {
    while (1) {
        slam.updateObservationData(distanceSensor.vecY);
        slam.updateCoordinate(gyro.deg);

        app.delay(10);

        // uart3.println(slam.coordinateY);
    }
}

#endif