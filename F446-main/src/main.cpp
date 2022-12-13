#include <Arduino.h>

#include "./lib/IO-Kit.h"
#include "./lib/RTOS-Kit.h"

HardwareSerial uart1(PA10, PA9);
HardwareSerial uart4(PA1, PA0);

RTOS_Kit app;

#include "./SCServo/SCServo.h"
#include "./lib/vl53l0x.h"
#include "./lib/bno055.h"
#include "./lib/ws2812b.h"
#include "./lib/mlt8530.h"
#include "./lib/switchUI.h"

Adafruit_NeoPixel stripL = Adafruit_NeoPixel(7, PA15, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripR = Adafruit_NeoPixel(7, PB13, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripUI = Adafruit_NeoPixel(24, PB14, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripTop = Adafruit_NeoPixel(24, PC1, NEO_GRB + NEO_KHZ800);

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

VL53L0X distanceSensor(&uart4);
BNO055 gyro(&bno);
WS2812B led(50);
MLT8530 speaker;
SMS_STS st;
SWITCHUI ui;

#include "./app/sensorApp.h"

TaskHandle_t buzzerAppHandler;

void buzzerApp(App) {
    // while (1) {
    //     speaker.setFrequncy(440);
    //     app.delay(100);
    //     speaker.mute();
    //     app.delay(100);
    // }
}

void blinkApp(App) {
    pinMode(PB12, OUTPUT);
    while (1) {
        digitalWrite(PB12, HIGH);
        app.delay(150);
        digitalWrite(PB12, LOW);
        app.delay(150);
    }
}

void mainApp(App) {
    while (1) {
        if (ui.button[0]) {
            led.setLeftColor(led.red);
        } else if (ui.button[1]) {
            led.setLeftColor(led.green);
        } else if (ui.toggle) {
            led.setLeftColor(led.blue);
        } else {
            led.setLeftColor(led.yellow);
        }
        led.show();
    }
}

void setup() {
    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();

    gyro.init();

    uart1.setRx(PA10);
    uart1.setTx(PA9);
    uart1.begin(115200);

    app.create(mainApp, firstPriority);
    app.create(blinkApp);
    app.create(inputMonitoringApp, firstPriority);

    app.start(mainApp);
    app.start(inputMonitoringApp);
    app.startRTOS();
}

void loop() {
    // Nothing to do.
}