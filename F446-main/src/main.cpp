#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "./SCServo/SCServo.h"

SMS_STS st;

HardwareSerial uart1(PA10, PA9);
HardwareSerial uart4(PA1, PA0);

Adafruit_NeoPixel stripL = Adafruit_NeoPixel(7, PA15, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripR = Adafruit_NeoPixel(7, PB13, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripUI = Adafruit_NeoPixel(24, PB14, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel stripTop = Adafruit_NeoPixel(24, PC1, NEO_GRB + NEO_KHZ800);

#include "./lib/vl53l0x.h"
#include "./lib/bno055.h"

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

VL53L0X distanceSensor;
BNO055 gyro;

const int speakerPin = PB6;
// void loop(void) {
//     gyro.read();

//     for (int j = 0; j < 7; j++) {
//         stripL.setPixelColor(j, stripL.ColorHSV(gyro.deg * 182, 255, 255));
//         stripR.setPixelColor(j, stripR.ColorHSV(gyro.deg * 182, 255, 255));
//     }
//     stripR.show();
//     stripL.show();

//     for (int i = 0; i < 24; i++) {
//         stripUI.setPixelColor(i, 0);
//     }

//     for (int j = 0; j < map(gyro.deg, 0, 360, 0, 25); j++) {
//         stripUI.setPixelColor(j, stripL.ColorHSV(map(j, 0, 25, 0, 40000),
//         255, 255));
//     }
//     stripUI.show();
// }

void setup() {
    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();

    stripR.begin();
    stripR.setBrightness(80);
    stripR.show();

    stripL.begin();
    stripL.setBrightness(80);
    stripL.show();

    stripUI.begin();
    stripUI.setBrightness(80);
    stripUI.show();

    stripTop.begin();
    stripTop.setBrightness(120);
    stripTop.show();

    gyro.sensorPtr = &bno;
    gyro.init();

    uart4.setRx(PA1);
    uart4.setTx(PA0);
    uart4.begin(1000000);
    distanceSensor.serialPtr = &uart4;

    uart1.setRx(PA10);
    uart1.setTx(PA9);
    uart1.begin(115200);
}

void loop() {
    distanceSensor.getDistance();
    for (int i = 0; i < 12; i++) {
        uart1.print(distanceSensor.val[i]);
        uart1.print("\t");
    }
    uart1.println();

    for (int i = 0; i < 12; i++) {
        stripTop.setPixelColor(
            i * 2, 0, 0, 255 - constrain(distanceSensor.val[i], 0, 1000) / 4);
        stripTop.setPixelColor(
            i * 2 + 1, 0, 0,
            255 - constrain(distanceSensor.val[i], 0, 1000) / 4);
    }

    stripTop.show();

    delay(10);
}