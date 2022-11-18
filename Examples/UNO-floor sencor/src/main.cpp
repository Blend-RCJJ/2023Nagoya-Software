#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

const int ledPin    = 6;
const int numOfLeds = 3;

int val[3];

const int sensor = A1;

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(numOfLeds, ledPin, NEO_GRB + NEO_KHZ800);

void setup() {
    // Initialize the NeoPixels
    strip.begin();
    strip.show();

    pinMode(sensor, INPUT);
    Serial.begin(115200);
}

long color[3] = {strip.Color(255, 0, 0), strip.Color(0, 210, 0),
                 strip.Color(0, 0, 220)};

void loop() {
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < numOfLeds; i++) {
            strip.setPixelColor(i, color[j]);
        }
        strip.setBrightness(40);
        strip.show();

        delay(5);
        val[j] = analogRead(sensor);
        // Serial.print(val[j]);
        // Serial.print("\t");
    }

    if (val[0] < val[1]) {
        if (val[0] < val[2]) {
            Serial.print("赤");
        } else {
            Serial.print("青");
        }
    } else {
        if (val[1] < val[2]) {
            Serial.print("緑");
        } else {
            Serial.print("青");
        }
    }

    int cutoff = 130;
    if(val[0]<=cutoff){
        if(val[1]<=cutoff){
            if(val[2]<=cutoff){
                Serial.print("\tやっぱり白かも");
            }
        }
    }

    Serial.println("");
}