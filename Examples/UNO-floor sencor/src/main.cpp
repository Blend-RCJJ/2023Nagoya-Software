#include <Arduino.h>
const int sensor = A1;
void setup() {
  pinMode(sensor,INPUT);
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  int i = analogRead(sensor);
  Serial.println(i);
  // put your main code here, to run repeatedly:
}