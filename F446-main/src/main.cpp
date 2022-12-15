#include <Arduino.h>

#include "./lib/IO-Kit.h"
#include "./lib/RTOS-Kit.h"

HardwareSerial uart1(PA10, PA9);
HardwareSerial uart4(PA1, PA0);
HardwareSerial uart5(PD2, PC12);

RTOS_Kit app;

#include "./SCServo/SCServo.h"
#include "./lib/bno055.h"
#include "./lib/mlt8530.h"
#include "./lib/switchUI.h"
#include "./lib/vl53l0x.h"
#include "./lib/ws2812b.h"

Adafruit_NeoPixel stripL = Adafruit_NeoPixel(7, PA15, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripR = Adafruit_NeoPixel(7, PB13, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripUI = Adafruit_NeoPixel(24, PB14, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripTop = Adafruit_NeoPixel(24, PC1, NEO_GRB + NEO_KHZ800);

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

VL53L0X distanceSensor(&uart4);
BNO055 gyro(&bno);
WS2812B led(80);
MLT8530 speaker;
SMS_STS st;
SWITCHUI ui;

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

void TurnLeft(App) {
    while (1) {
        // if (distanceSensor.val[0] <= 90) {
        // st.WriteSpe(1, -5000, 0);
        // st.WriteSpe(4, 5000, 0);
        // st.WriteSpe(2, -5000, 0);
        // st.WriteSpe(3, 5000, 0);
        // delay(100);  // 時計回りに回転
        // if (gyro.deg == 90) {
        //     break;
        // }
        // }
    }
}

void mainApp(App) {
    uart1.println("turnLeftApp開始");
    app.start(TurnLeft);
    app.delay(500);

    while (1) {
        for (int i = 0; i < 12; i++) {
            uart1.print(distanceSensor.val[i]);
            uart1.print("\t");
        }
        // uart1.print("\n");
        uart1.println(gyro.deg);

        app.delay(300);
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

    // while (1) {
    led.bootIllumination();
    //     uart1.println("bootIllumination");
    // }

    uart5.begin(1000000);
    st.pSerial = &uart5;

    st.WheelMode(1);

    for (int i = 1; i <= 4; i++) {
        st.unLockEprom(i);
        st.EnableTorque(i, 1);
        st.LockEprom(i);
    }

    while (1) {
        int val = 8000;
        st.WriteSpe(1, val, 0);
        delay(5);
        st.WriteSpe(2, val, 0);
        delay(5);
        st.WriteSpe(3, val, 0);
        delay(5);
        st.WriteSpe(4, val, 0);
        delay(1000);
        val *= -1;
        st.WriteSpe(1, val, 0);
        delay(5);
        st.WriteSpe(2, val, 0);
        delay(5);
        st.WriteSpe(3, val, 0);
        delay(5);
        st.WriteSpe(4, val, 0);
        delay(1000);
    }

    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();

    gyro.init();

    app.create(mainApp, firstPriority);
    app.create(VictimDectationLED);
    app.create(inputMonitoringApp, firstPriority);
    app.create(TurnLeft);

    app.start(mainApp);
    app.start(inputMonitoringApp);
    app.startRTOS();
}

void loop() {
    // Nothing to do.
}