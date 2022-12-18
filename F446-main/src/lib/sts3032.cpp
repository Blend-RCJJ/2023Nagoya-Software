#include "sts3032.h"

#include "./ws2812b.h"
#include "./unitV.h"

SMS_STS serialServo;

extern HardwareSerial uart1;

extern WS2812B led;
extern UNITV cameraLeft;
extern UNITV cameraRight;

STS3032::STS3032(HardwareSerial *ptr) {
    serialPtr = ptr;
    serialPtr->begin(baudRate);
    serialServo.pSerial = serialPtr;

    serialServo.WheelMode(1);

    for (int i = 1; i <= 4; i++) {
        serialServo.unLockEprom(i);
        serialServo.EnableTorque(i, 1);
        serialServo.LockEprom(i);
    }
}

void STS3032::directDrive(int id, int percent, int acceleration) {
    int sendData;
    sendData = percent * maximumSpeed / 100;
    sendData = constrain(sendData, -maximumSpeed, maximumSpeed);

    if (cameraLeft.isVictimDetected || cameraRight.isVictimDetected) {
        led.setUIColor(led.yellow);
        led.setLeftColor(led.yellow);
        led.setRightColor(led.yellow);
        led.setTopColor(led.yellow);
        led.setUIBrightness(255 * (millis() / 100) % 2);
        led.setRightBrightness(255 * (millis() / 100) % 2);
        led.setLeftBrightness(255 * (millis() / 100) % 2);
        led.setTopBrightness(255 * (millis() / 100) % 2);
        led.show();
        serialServo.WriteSpe(id + 1, 0, acceleration);
        return;
    } else {
        // app.stop(LEDtktk);

        led.setUIBrightness(255);
        led.setRightBrightness(255);
        led.setLeftBrightness(255);
        led.setTopBrightness(255);
        led.setUIColor(led.blue);
        led.setLeftColor(led.blue);
        led.setRightColor(led.blue);
        led.setTopColor(led.blue);
        led.show();
    }

    if (digitalRead(PA5)) {
        serialServo.WriteSpe(id + 1, sendData, acceleration);
    } else {
        serialServo.WriteSpe(id + 1, 0, acceleration);
    }
}

void STS3032::driveAngularVelocity(int velocity, int angularVelocity) {
    int data[2];
    data[0] = angularVelocity - velocity;
    data[1] = angularVelocity + velocity;

    for (int i = 0; i < 2; i++) {
        data[i] = constrain(data[i], -100, 100);
    }

    for (int i = 0; i < 2; i++) {
        directDrive(i, data[0]);
    }

    for (int i = 2; i < 4; i++) {
        directDrive(i, data[1]);
    }
}

void STS3032::drive(int velocity, int angle) {
    const double Kp = -4;

    // 0-360変換
    while (angle < 0) {
        angle += 360;
    }
    angle %= 360;

    int angularVelocity = gyro.deg - angle;

    //-180から180変換
    while (angularVelocity < 0) {
        angularVelocity += 360;
    }
    if (angularVelocity > 180) {
        angularVelocity -= 360;
    }

    angularVelocity *= Kp;

    driveAngularVelocity(velocity, angularVelocity);
}