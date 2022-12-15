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

void Drive(App) {
    while (1) {
        st.WriteSpe(1, SPEED1 * 70, 0);
        st.WriteSpe(4, SPEED4 * 70, 0);
        st.WriteSpe(2, SPEED2 * 70, 0);
        st.WriteSpe(3, SPEED3 * 70, 0);
        app.delay(2000);

        st.WriteSpe(1, -SPEED1 * 70, 0);
        st.WriteSpe(4, SPEED4 * 70, 0);
        st.WriteSpe(2, -SPEED2 * 70, 0);
        st.WriteSpe(3, SPEED3 * 70, 0);  // 時計回りに回転
        app.delay(375);
    }
}

void mainApp(App) {
    uart1.println("DriveApp開始");
    app.start(Drive);
    app.delay(10000);
    app.stop(Drive);

    while (1) {
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

    // while (1) {
    //     led.bootIllumination();
    //     uart1.println("bootIllumination");
    // }

    uart5.begin(1000000);
    st.pSerial = &uart5;
    delay(1000);

    st.unLockEprom(1);
    st.WheelMode(1);
    st.EnableTorque(1, 1);
    st.LockEprom(1);

    Wire.setSDA(PB9);
    Wire.setSCL(PB8);
    Wire.begin();

    gyro.init();

    app.create(mainApp, firstPriority);
    app.create(VictimDectationLED);
    app.create(inputMonitoringApp, firstPriority);
    app.create(Drive);
    app.create(inputMonitoringApp);

    app.start(mainApp);
    app.start(Drive);
    app.start(inputMonitoringApp);
    app.startRTOS();
}

void loop() {
    // Nothing to do.
}