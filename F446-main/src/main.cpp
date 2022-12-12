#include <Arduino.h>

#include "./lib/IO-Kit.h"
#include "./lib/RTOS-Kit.h"

HardwareSerial uart1(PA10, PA9);
HardwareSerial uart4(PA1, PA0);

#include "./SCServo/SCServo.h"
#include "./lib/vl53l0x.h"
#include "./lib/bno055.h"
#include "./lib/ws2812b.h"
#include "./lib/mlt8530.h"

Adafruit_NeoPixel stripL = Adafruit_NeoPixel(7, PA15, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripR = Adafruit_NeoPixel(7, PB13, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripUI = Adafruit_NeoPixel(24, PB14, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripTop = Adafruit_NeoPixel(24, PC1, NEO_GRB + NEO_KHZ800);

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

VL53L0X distanceSensor(&uart4);
BNO055 gyro(&bno);

WS2812B led(50);

SMS_STS st;

MLT8530 speaker;

RTOS_Kit app;

TaskHandle_t buzzerAppHandler;

void buzzerApp(App) {
    while (1) {
        speaker.setFrequncy(440);
        app.delay(100);
        speaker.mute();
        app.delay(100);
    }
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
    uart1.println("Hello World!");
    app.delay(3000);

    uart1.println("buzzerApp再開");
    app.start(buzzerApp);
    app.delay(3000);

    uart1.println("buzzerApp停止, blinkApp再開");
    app.stop(buzzerApp);
    app.start(blinkApp);
    app.delay(3000);

    uart1.println("All of apps Stop");
    app.stop(buzzerApp);
    app.stop(blinkApp);
    app.delay(3000);

    uart1.println("All of apps Start");
    app.start(buzzerApp);
    app.start(blinkApp);

    while (1) {
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
    app.create(buzzerApp);

    app.start(mainApp);
    app.startRTOS();
}

void loop() {
    // Nothing to do.
}