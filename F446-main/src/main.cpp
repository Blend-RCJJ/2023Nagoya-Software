#include <Arduino.h>

#include "./lib/IO-Kit.h"
#include "./lib/RTOS-Kit.h"

HardwareSerial uart1(PA10, PA9);
HardwareSerial uart4(PA1, PA0);
HardwareSerial uart5(PD2, PC12);
HardwareSerial uart6(PC7, PC6);

RTOS_Kit app;

#include "./lib/bno055.h"
#include "./lib/mlt8530.h"
#include "./lib/switchUI.h"
#include "./lib/vl53l0x.h"
#include "./lib/ws2812b.h"

Adafruit_NeoPixel stripL   = Adafruit_NeoPixel(7, PA15, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripR   = Adafruit_NeoPixel(7, PB13, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripUI  = Adafruit_NeoPixel(24, PB14, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripTop = Adafruit_NeoPixel(24, PC1, NEO_GRB + NEO_KHZ800);

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

VL53L0X distanceSensor(&uart4);
BNO055 gyro(&bno);
WS2812B led(80);
MLT8530 speaker;
SWITCHUI ui;

#include "./lib/sts3032.h"

STS3032 servo(&uart5);

#include "./app/sensorApp.h"

#define SPEED1 -80
#define SPEED2 -80
#define SPEED3 80
#define SPEED4 80

void VictimDectationLED(App) {
    while (1) {
        static int count = 0;
        led.setUIColor(led.green);
        led.setLeftColor(led.green);
        led.setRightColor(led.green);
        led.setUIBrightness(127 * sin(count / 50.0) + 127);
        led.setRightBrightness(127 * sin(count / 50.0) + 127);
        led.setLeftBrightness(127 * sin(count / 50.0) + 127);
        led.show();
        count++;

        app.delay(1);
    }
}  // 被災者発見シグナルApp

void topLED(App) {
    while (1) {
        led.setTopBrightness(distanceSensor.val[0] / 8);
        led.setTopColor(led.red);
        led.show();
        app.delay(100);
    }
}

void mainApp(App) {
    while (1) {
        app.start(largeDrive);
        app.delay(60000);
        app.stop(largeDrive);
        app.start(onlyRight);
        app.delay(30000);
        app.stop(onlyRight);
        app.start(onlyLeft);
        app.delay(30000);
        app.stop(onlyLeft);

    app.delay(100);
     }

    while (1) {
        // servo.driveAngularVelocity(0, 80);
        // servo.drive(0, 180);
        // app.start(topLED);
        uart1.println(distanceSensor.val[0]);
        uart1.println(gyro.deg);
        app.delay(10);
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

    // uart6.setRx(PC7);
    // uart6.setTx(PC6);
    // uart6.begin(115200);

    // while (1) {
    //     if (uart6.available() > 0) {
    //         char temp = uart6.read();
    //         uart1.println(temp);

    //     // uart1.println("100");
    // }

    led.setLeftColor(led.blue);
    led.setRightColor(led.blue);
    speaker.bootSound();
    led.bootIllumination();

    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();

    gyro.init();

    app.create(mainApp, firstPriority);
    app.create(VictimDectationLED);
    app.create(inputMonitoringApp, firstPriority);
    app.create(DriveLeft);
    app.create(DriveRight);
    app.create(largeDrive);
    app.create(onlyRight);
    app.create(onlyLeft);
    app.create(topLED);


    app.start(mainApp);
    app.start(inputMonitoringApp);
    app.startRTOS();
}

void loop() {
    // Nothing to do.
}