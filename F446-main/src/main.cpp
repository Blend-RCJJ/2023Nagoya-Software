#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <STM32FreeRTOS.h>

#include "./lib/IO-Kit.h"

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

void setup() {
    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();

    gyro.init();

    uart1.setRx(PA10);
    uart1.setTx(PA9);
    uart1.begin(115200);
}

void loop() {
    speaker.setFrequncy(440);
    delay(1000);
    speaker.mute();
    delay(1000);

    int battery = 100;
    led.battery(1);
    led.show();
}