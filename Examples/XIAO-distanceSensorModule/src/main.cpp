#include <Arduino.h>
#include <Wire.h>

#include "./lib/clsPCA9555.h"

#define PCA9555_ADDR 0x20

void deviceScanner(void);

PCA9555 xshut(PCA9555_ADDR);

void setup() {
    // initialize serial communication and i2c
    Serial.begin(9600);
    Wire.begin();

    while (Serial.read() == -1) {
    }

    // scan for i2c devices
    deviceScanner();
}

void loop() {
    // put your main code here, to run repeatedly:
}

void deviceScanner(void) {
    char error, address;
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");

            nDevices++;
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");

    while (1)
        ;
}