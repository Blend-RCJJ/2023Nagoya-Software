#include <Arduino.h>

#include "./lib/IO-Kit.h"
#include "./lib/RTOS-Kit.h"

HardwareSerial uart1(PA10, PA9);
HardwareSerial uart2(PA3, PA2);
HardwareSerial uart4(PA1, PA0);
HardwareSerial uart5(PD2, PC12);
HardwareSerial uart6(PC7, PC6);

RTOS_Kit app;

#include "./lib/bno055.h"
#include "./lib/floorSensor.h"
#include "./lib/mlt8530.h"
#include "./lib/switchUI.h"
#include "./lib/vl53l0x.h"
#include "./lib/ws2812b.h"
#include "./lib/floorSensor.h"
#include "./lib/unitV.h"

Adafruit_NeoPixel stripL   = Adafruit_NeoPixel(7, PA15, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripR   = Adafruit_NeoPixel(7, PB13, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripUI  = Adafruit_NeoPixel(24, PB14, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripTop = Adafruit_NeoPixel(24, PC1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripFloor = Adafruit_NeoPixel(3, PB15, NEO_GRB + NEO_KHZ800);

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

VL53L0X distanceSensor(&uart4);
BNO055 gyro(&bno);
WS2812B led(80);
MLT8530 speaker;
SWITCHUI ui;
FLOOR_SENSOR floorSensor;
UNITV cameraLeft(&uart6);
UNITV cameraRight(&uart2);

#include "./lib/sts3032.h"
STS3032 servo(&uart5);

#include "./app/sensorApp.h"

#define SPEED1 -80
#define SPEED2 -80
#define SPEED3 80
#define SPEED4 80

void VictimDectationLED(App) {
    while (1) {
        // static int count = 0;
        // led.setUIColor(led.green);
        // led.setLeftColor(led.green);
        // led.setRightColor(led.green);
        // led.setUIBrightness(127 * sin(count / 50.0) + 127);
        // led.setRightBrightness(127 * sin(count / 50.0) + 127);
        // led.setLeftBrightness(127 * sin(count / 50.0) + 127);
        // led.show();
        // count++;

        app.delay(1);
    }
}  // 被災者発見シグナルApp

void topLED(App) {
    while (1) {
        // led.setTopBrightness(distanceSensor.val[0] / 8);
        // led.setTopColor(led.red);
        // led.show();
        app.delay(100);
    }
}
int appMode = 0;

void isOnBlack(App) {
    while (1) {
        if (((floorSensor.redVal >= 900) && (floorSensor.blueVal >= 900)) &&
            (floorSensor.greenVal >= 900)) {
            // led.setTopColor(led.blue);
            // led.show();
            app.stop(largeDrive);
            app.stop(onlyRight);
            app.stop(onlyLeft);
            servo.driveAngularVelocity(0, 0);
            app.delay(500);

            angle -= 90;
            angle %= 360;
            servo.driveAngularVelocity(-50, 0);
            app.delay(1000);

            unsigned long timer = millis();
            while (millis() <= timer + 1300) {
                servo.drive(0, angle);
                app.delay(1);
            }

            switch (appMode) {
                case 0:
                    app.start(largeDrive);
                    break;
                case 1:

                    app.start(onlyRight);
                    break;
                case 2:

                    app.start(onlyLeft);
                    break;
            }
        } else {
            // led.setTopColor(led.red);
            // led.show();
            app.delay(10);
        }
    }
}

void isOnBlue(App) {
    while (1) {
        if ((floorSensor.blueVal <= floorSensor.greenVal - 100) &&
            (floorSensor.blueVal <= floorSensor.redVal - 100)) {
            // led.setTopColor(led.blue);
            // led.show();
            app.stop(largeDrive);
            app.stop(onlyRight);
            app.stop(onlyLeft);
            servo.driveAngularVelocity(0, 0);
            app.delay(5000);

            unsigned long timer = millis();
            while (millis() <= timer + 1300) {
                servo.drive(0, angle);
                app.delay(1);
            }

            servo.driveAngularVelocity(50, 0);
            app.delay(2000);

            switch (appMode) {
                case 0:
                    app.start(largeDrive);
                    break;
                case 1:

                    app.start(onlyRight);
                    break;
                case 2:

                    app.start(onlyLeft);
                    break;
            }
        } else {
            // led.setTopColor(led.red);
            // led.show();
            app.delay(10);
        }
    }
}

// void LEDtktk(App) {
//     while (1) {
//          if (cameraLeft.isVictimDetected || cameraRight.isVictimDetected) {
//             led.setUIColor(led.yellow);
//             led.setLeftColor(led.yellow);
//             led.setRightColor(led.yellow);
//             led.setTopColor(led.yellow);
//             led.setUIBrightness(255 * (millis() / 100) % 2);
//             led.setRightBrightness(255 * (millis() / 100) % 2);
//             led.setLeftBrightness(255 * (millis() / 100) % 2);
//             led.setTopBrightness(255 * (millis() / 100) % 2);
//             led.show();
//         } else {
//             // app.stop(LEDtktk);

//             led.setUIBrightness(255);
//             led.setRightBrightness(255);
//             led.setLeftBrightness(255);
//             led.setTopBrightness(255);
//             led.setUIColor(led.blue);
//             led.setLeftColor(led.blue);
//             led.setRightColor(led.blue);
//             led.setTopColor(led.blue);
//             led.show();
//         }
//     }
// }

void mainApp(App) {
    // app.start(LEDtktk);
    while (1) {
        appMode = 0;

        app.start(isOnBlack);
        app.start(isOnBlue);
        app.start(largeDrive);
        app.delay(30000);
        app.start(right);
        app.delay(2000);
        app.stop(right);
        // app.start(DriveRight);
        // app.delay(5000);
        // app.stop(DriveRight);

        appMode = 1;
        app.stop(largeDrive);
        app.start(onlyRight);
        app.delay(10000);

        appMode = 2;
        app.stop(onlyRight);
        app.start(onlyLeft);
        app.delay(10000);
        app.stop(onlyLeft);
        app.start(left);
        app.delay(2000);
        app.stop(left);


    }

    while (1) {
        // app.start(oooon);
        // app.delay(10);
        // servo.driveAngularVelocity(0, 80);
        // servo.drive(0, 180);
        // app.start(topLED);
        // uart1.println(distanceSensor.val[0]);
        // uart1.println(gyro.deg);
        //     app.delay(10);
    }
}

void setup() {
    // led.start(24);
    // led.show();

    // led.leftBootLED(7);
    // led.show();

    // led.rightBootLED(7);
    // led.show();

    // led.tktk(100000);
    // led.show();

    uart1.setRx(PA10);
    uart1.setTx(PA9);
    uart1.begin(115200);

    uart2.setRx(PA3);
    uart2.setTx(PA2);
    uart2.begin(115200);

    uart6.setRx(PC7);
    uart6.setTx(PC6);
    uart6.begin(115200);

    led.setLeftColor(led.yellow);
    led.setRightColor(led.yellow);
    led.setUIColor(led.yellow);
    led.show();

    speaker.bootSound();
    led.bootIllumination();

    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();

    gyro.init();

    app.create(mainApp, firstPriority);
    app.create(VictimDectationLED);
    app.create(inputMonitoringApp, firstPriority);
    app.create(largeDrive);
    app.create(onlyRight);
    app.create(onlyLeft);
    // app.create(topLED);
    app.create(isOnBlack);
    app.create(isOnBlue);
    app.create(oooon);
    app.create(right);
    app.create(random);

    app.start(mainApp);
    app.start(inputMonitoringApp);
    app.startRTOS();
}

void loop() {
    // Nothing to do.
}