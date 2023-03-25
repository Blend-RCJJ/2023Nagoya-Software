#include <Arduino.h>
#include <EEPROM.h>

#include "./kit/IO-Kit.h"
#include "./kit/Location-Kit.h"
#include "./kit/RTOS-Kit.h"

HardwareSerial uart1(PA10, PA9);
HardwareSerial uart2(PA3, PA2);
HardwareSerial uart3(PC5, PB10);
HardwareSerial uart4(PA1, PA0);
HardwareSerial uart5(PD2, PC12);
HardwareSerial uart6(PC7, PC6);

RTOS_Kit app;
Location_Kit location;

#include "./lib/bno055.h"
#include "./lib/floorSensor.h"
#include "./lib/heatSensor.h"
#include "./lib/loadcell.h"
#include "./lib/mlt8530.h"
#include "./lib/switchUI.h"
#include "./lib/unitV.h"
#include "./lib/vl53l0x.h"
#include "./lib/ws2812b.h"

Adafruit_NeoPixel stripL   = Adafruit_NeoPixel(7, PA15, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripR   = Adafruit_NeoPixel(7, PB13, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripUI  = Adafruit_NeoPixel(24, PB14, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripTop = Adafruit_NeoPixel(24, PC1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripFloor = Adafruit_NeoPixel(4, PB15, NEO_GRB + NEO_KHZ800);

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

VL53L0X distanceSensor(&uart4);
BNO055 gyro(&bno);
WS2812B led(50);
MLT8530 speaker;
SWITCHUI ui;
FLOOR_SENSOR floorSensor;
UNITV cameraLeft(&uart6);
UNITV cameraRight(&uart2);
LOADCELL loadcell;
HEATSENSOR heatSensor;

#include "./lib/sts3032.h"
STS3032 servo(&uart5);

#include "./app/locationApp.h"
#include "./app/sensorApp.h"

int appMode = 0;

void ABARENBO_SHOGUN_MATSUKEN_LOVE(void);

void mainApp(App);
void uartInit(void);

void distanceCalibration(void);

void setup() {
    uartInit();
    speaker.bootSound();

    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();

    loadcell.init();
    heatSensor.init();
    gyro.init();

    speaker.bootSound();

    delay(500);

    // while (1) {
    //     servo.directDrive(0,-60);
    //     servo.directDrive(1,-20);
    //     servo.directDrive(2,60);
    //     servo.directDrive(3,20);
    //     delay(800);

    // }

    gyro.setOffset();

    location.mapData[MAP_ORIGIN][MAP_ORIGIN].isVictimDetected = true;

    // while(1){
    //     gyro.read();
    //     uart1.println(gyro.slope);
    // }

    // ドッキリ！！！！！！！
    // ABARENBO_SHOGUN_MATSUKEN_LOVE();

    // distanceCalibration();

    app.create(mainApp, firstPriority);
    app.create(inputMonitoringApp, secondPriority);
    app.create(servoApp);
    app.create(rightWall);
    app.create(leftWall);
    app.create(adjustment);
    app.create(monitor);
    app.create(black);
    app.create(camera);
    app.create(visualization);
    app.create(lever);
    app.create(hitAvoid);
    app.create(randomSwitching,lowPriority);

    app.create(locationApp, firstPriority);
    app.create(sideLEDApp);
    app.create(mapApp);

    app.create(victimApp);

    app.start(mainApp);
    app.start(inputMonitoringApp);
    app.startRTOS();
}

// Main app.
void mainApp(App) {
    app.start(servoApp);
    app.start(adjustment);
    app.start(rightWall);
    app.start(black);
    app.start(monitor);
    app.start(camera);
    app.start(visualization);
    app.start(randomSwitching);
    app.start(lever);
    app.start(hitAvoid);
    app.start(victimApp);
    ;
    while (1) {
        app.delay(1);
    }
}

void uartInit(void) {
    uart1.setRx(PA10);
    uart1.setTx(PA9);
    uart1.begin(115200);

    uart2.setRx(PA3);
    uart2.setTx(PA2);
    uart2.begin(115200);

    uart6.setRx(PC7);
    uart6.setTx(PC6);
    uart6.begin(115200);

    uart3.setRx(PC5);
    uart3.setTx(PB10);
    uart3.begin(115200);
}

void loop() {
    // Nothing to do.
}

void distanceCalibration(void) {
    distanceSensor.getDistance();

    uart3.println("Distance Sensor Calibration");

    delay(3000);

    for (int i = 0; i < 3; i++) {
        uart3.println(3 - i);
        delay(1000);
    }

    uart3.println("Start");

    int distance[4][10] = {0};
    for (int i = 0; i < 10; i++) {
        distanceSensor.getDistance();
        for (int j = 0; j < 4; j++) {
            distance[j][i] = distanceSensor.val[j * 3];
            delay(100);
        }
    }

    int offset[4] = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 10; j++) {
            offset[i] += distance[i][j];
        }
        offset[i] /= 10;
    }

    // orint offset[]
    for (int i = 0; i < 4; i++) {
        uart3.print(i);
        uart3.print(" : ");
        uart3.println(offset[i]);
        delay(100);
    }
    while (1) {
    }
}

void ABARENBO_SHOGUN_MATSUKEN_LOVE(void) {
    led.setTopColor(led.yellow);
    led.setRightColor(led.yellow);
    led.setLeftColor(led.yellow);
    led.setUIColor(led.yellow);
    led.show();
    delay(2000);
    speaker.matsukenShogun();
    while (1)
        ;
}