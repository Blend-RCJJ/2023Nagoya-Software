#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#include <Adafruit_NeoPixel.h>

#include <Arduino.h>

#include "./SCServo/SCServo.h"

SMS_STS st;

HardwareSerial mySerial(PD2, PC12);

void setup() {
    mySerial.setRx(PD2);
    mySerial.setTx(PC12);
    mySerial.begin(1000000);
    st.pSerial = &mySerial;
    delay(1000);
    st.unLockEprom(2);
    st.WheelMode(2);  //恒速模式
    st.EnableTorque(2, 0);
    st.LockEprom(2);
}

void loop() {
    st.WriteSpe(2, 8000,255);
    delay(2000);
    st.WriteSpe(2, -8000,255);
    delay(2000);
}

// Adafruit_NeoPixel stripL = Adafruit_NeoPixel(7, PA15, NEO_GRB + NEO_KHZ800);
// Adafruit_NeoPixel stripR = Adafruit_NeoPixel(7, PB13, NEO_GRB + NEO_KHZ800);

// Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

// #define SPEAKER PB6

// void displaySensorDetails(void) {
//     sensor_t sensor;
//     bno.getSensor(&sensor);
//     Serial.println("------------------------------------");
//     Serial.print("Sensor:       ");
//     Serial.println(sensor.name);
//     Serial.print("Driver Ver:   ");
//     Serial.println(sensor.version);
//     Serial.print("Unique ID:    ");
//     Serial.println(sensor.sensor_id);
//     Serial.print("Max Value:    ");
//     Serial.print(sensor.max_value);
//     Serial.println(" xxx");
//     Serial.print("Min Value:    ");
//     Serial.print(sensor.min_value);
//     Serial.println(" xxx");
//     Serial.print("Resolution:   ");
//     Serial.print(sensor.resolution);
//     Serial.println(" xxx");
//     Serial.println("------------------------------------");
//     Serial.println("");
//     // delay(500);
// }

// /**************************************************************************/
// /*
//     Display some basic info about the sensor status
// */
// /**************************************************************************/
// void displaySensorStatus(void) {
//     /* Get the system status values (mostly for debugging purposes) */
//     uint8_t system_status, self_test_results, system_error;
//     system_status = self_test_results = system_error = 0;
//     bno.getSystemStatus(&system_status, &self_test_results, &system_error);

//     /* Display the results in the Serial Monitor */
//     Serial.println("");
//     Serial.print("System Status: 0x");
//     Serial.println(system_status, HEX);
//     Serial.print("Self Test:     0x");
//     Serial.println(self_test_results, HEX);
//     Serial.print("System Error:  0x");
//     Serial.println(system_error, HEX);
//     Serial.println("");
//     // delay(500);
// }

// void displayCalStatus(void) {
//     /* Get the four calibration values (0..3) */
//     /* Any sensor data reporting 0 should be ignored, */
//     /* 3 means 'fully calibrated" */
//     uint8_t system, gyro, accel, mag;
//     system = gyro = accel = mag = 0;
//     bno.getCalibration(&system, &gyro, &accel, &mag);

//     /* The data should be ignored until the system calibration is > 0 */
//     Serial.print("\t");
//     if (!system) {
//         Serial.print("! ");
//     }

//     /* Display the individual values */
//     Serial.print("Sys:");
//     Serial.print(system, DEC);
//     Serial.print(" G:");
//     Serial.print(gyro, DEC);
//     Serial.print(" A:");
//     Serial.print(accel, DEC);
//     Serial.print(" M:");
//     Serial.print(mag, DEC);
// }

// void setup(void) {
//     Wire.setSDA(PB9);
//     Wire.setSCL(PB8);
//     Wire.begin();

//     Serial.setTx(PA9);
//     Serial.setRx(PA10);
//     Serial.begin(9600);

//     Serial.println("Orientation Sensor Test");
//     Serial.println("");

//     bno.begin();
//     displaySensorDetails();
//     displaySensorStatus();
//     bno.setExtCrystalUse(true);

//     stripR.begin();
//     stripR.setBrightness(40);
//     stripR.show();

//     stripL.begin();
//     stripL.setBrightness(40);
//     stripL.show();

//     pinMode(SPEAKER, OUTPUT);
// }

// void loop(void) {
//     sensors_event_t event;
//     bno.getEvent(&event);
//     Serial.print("X: ");
//     Serial.print(event.orientation.x, 4);
//     Serial.println("");

//     // delay(BNO055_SAMPLERATE_DELAY_MS);
//     for (int j = 0; j < 7; j++) {
//         stripL.setPixelColor(j, stripL.ColorHSV(event.orientation.x * 182, 255, 255));
//         stripR.setPixelColor(j, stripR.ColorHSV(event.orientation.x * 182, 255, 255));
//     }
//     stripR.show();
//     stripL.show();
// }
