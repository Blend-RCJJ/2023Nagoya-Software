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

        app.delay(10);
    }
}

void servoApp(App) {
    while (1) {
        servo.drive(servo.velocity, servo.angle);
        app.delay(2);
    }
}

void gridControlApp(App) {
    while (1) {
        servo.velocity = 50;
        app.delay(100);
        static int val        = 0;
        static int oldVal     = distanceSensor.val[0];

        val = distanceSensor.val[0];

        if (val <= oldVal - 300) {
            servo.velocity = 0;
            app.delay(1000);
            if ((distanceSensor.val[0] > distanceSensor.val[3]) &&
                (distanceSensor.val[0] > distanceSensor.val[9])) {
                app.delay(10);
                oldVal = val;
            } else if (distanceSensor.val[3] > distanceSensor.val[9]) {
                app.delay(10);
                servo.angle += 90;
                oldVal = val;
            } else {
                app.delay(10);
                servo.angle -= 90;
                oldVal = val;
            }
        } else {
            app.delay(10);
        }
    }
}

#endif