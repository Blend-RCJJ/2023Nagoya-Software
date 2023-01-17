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
#include "./lib/unitV.h"
#include "./lib/vl53l0x.h"
#include "./lib/ws2812b.h"

Adafruit_NeoPixel stripL   = Adafruit_NeoPixel(7, PA15, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripR   = Adafruit_NeoPixel(7, PB13, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripUI  = Adafruit_NeoPixel(24, PB14, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripTop = Adafruit_NeoPixel(24, PC1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripFloor = Adafruit_NeoPixel(3, PB15, NEO_GRB + NEO_KHZ800);

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

VL53L0X distanceSensor(&uart4);
BNO055 gyro(&bno);
WS2812B led(50);
MLT8530 speaker;
SWITCHUI ui;
FLOOR_SENSOR floorSensor;
UNITV cameraLeft(&uart6);
UNITV cameraRight(&uart2);

#include "./lib/sts3032.h"
STS3032 servo(&uart5);

#include "./app/sensorApp.h"

int appMode = 0;

void mainApp(App);

void setup() {
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
    app.create(inputMonitoringApp, firstPriority);
    app.create(servoApp);
    app.create(gridControlApp);

    app.start(mainApp);
    app.start(inputMonitoringApp);
    app.startRTOS();

    app.delay(100);
}

// Main app.
void mainApp(App) {
    while (1) {
        app.start(servoApp);
        app.start(gridControlApp);
        app.delay(10);
    }
}

void loop() {
    // Nothing to do.
}