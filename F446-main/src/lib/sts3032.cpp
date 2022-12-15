#include "sts3032.h"

SMS_STS serialServo;

extern HardwareSerial uart1;

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
    serialServo.WriteSpe(id + 1, sendData, acceleration);
    // uart1.println(sendData);
}