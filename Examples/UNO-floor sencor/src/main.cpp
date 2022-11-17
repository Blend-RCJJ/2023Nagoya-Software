#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

const int ledPin = A0;
const int numOfLeds = 4;

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(A0, numOfLeds, NEO_GRB + NEO_KHZ800);

void setup() {
    // Initialize the NeoPixels
    strip.begin();
    strip.show();

    for (int i = 0; i < numOfLeds; i++) {
        strip.setPixelColor(i, 255, 255, 255);
    }
    strip.setBrightness(50);
    strip.show();
}

void loop() {
    // put your main code here, to run repeatedly:
}