#ifndef _SENSOR_APP_H
#define _SENSOR_APP_H
#define FLOOR_NUMBER 2

#define SPEED 50

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

extern HardwareSerial uart1;
extern HardwareSerial uart3;
extern RTOS_Kit app;
extern Location_Kit location;

extern Adafruit_NeoPixel stripL;
extern Adafruit_NeoPixel stripR;
extern Adafruit_NeoPixel stripUI;
extern Adafruit_NeoPixel stripTop;
extern Adafruit_NeoPixel stripFloor;

extern VL53L0X distanceSensor;
extern BNO055 gyro;
extern WS2812B led;
extern MLT8530 speaker;
extern SWITCHUI ui;
extern Adafruit_NeoPixel stripFloor;
extern WS2812B led;

extern UNITV cameraLeft;
extern UNITV cameraRight;

extern LOADCELL loadcell;
extern HEATSENSOR heatSensor;

extern void mapApp(App);
extern void sideLEDApp(App);
extern void locationApp(App);

int count = 0;
int val0 = 0;
int val6 = 0;
int correction = 0;

void camera(App);
void rightwall(App);
void visualization(App);

void inputMonitoringApp(App) {
    while (1) {
        distanceSensor.getDistance();
        distanceSensor.calcVector();

        gyro.read();
        ui.read();
        loadcell.read();
        heatSensor.read();

        led.setFloorColor(led.white);
        app.delay(5);
        floorSensor.blueVal = analogRead(PC0);

        led.setFloorColor(led.blank);
        app.delay(5);
        floorSensor.greenVal = analogRead(PC0);

        // led.setFloorColor(led.green);
        // app.delay(3);
        // floorSensor.greenVal = analogRead(PC0);

        cameraLeft.read();
        cameraRight.read();
        app.delay(3);
    }
}

void servoApp(App) {
    while (1) {
        servo.drive(servo.velocity, servo.angle + correction);
        app.delay(10);
    }
}

void gridSpecification(App) {
    while (1) {
        if ((distanceSensor.val[0] > distanceSensor.val[3]) &&
            (distanceSensor.val[0] > distanceSensor.val[9])) {
            servo.velocity = SPEED;
            app.delay(4200);
            servo.velocity = 0;
            app.delay(1000);
        } else if (distanceSensor.val[3] > distanceSensor.val[9]) {
            servo.velocity = 0;
            servo.angle += 90;
            app.delay(1000);
            servo.velocity = SPEED;
            app.delay(4200);
            servo.velocity = 0;
            app.delay(1000);
        } else if (distanceSensor.val[3] < distanceSensor.val[9]) {
            servo.velocity = 0;
            servo.angle -= 90;
            app.delay(1000);
            servo.velocity = SPEED;
            app.delay(4200);
            servo.velocity = 0;
            app.delay(1000);
        } else {
            app.delay(10);
        }

        if (distanceSensor.val[0] <= 150) {
            if (distanceSensor.val[3] <= 150) {
                if (distanceSensor.val[9] <= 150) {
                    servo.velocity = 0;
                    servo.angle += 180;
                    app.delay(1000);
                    servo.velocity = SPEED;
                    app.delay(4200);
                    servo.velocity = 0;
                    app.delay(1000);
                }
            }
        }
    }
}

void adjustment(App) {
    while (1) {
        app.delay(10);

        if (count == 1) {
            app.delay(900);
            count = 0;
        }

        while (count == 0) {
            if (distanceSensor.val[0] < 110) {
                servo.velocity = 0;
            }
            if (distanceSensor.val[3] + distanceSensor.val[9] < 300) {
                if (distanceSensor.val[3] > distanceSensor.val[9]) {
                    correction = 7;
                }
                if (distanceSensor.val[9] > distanceSensor.val[3]) {
                    correction = -7;
                }
                app.delay(50);
            } else {
                if (distanceSensor.val[9] < 120) {
                    correction = 7;
                }
                if (distanceSensor.val[3] < 120) {
                    correction = -7;
                }
            }

            if (distanceSensor.val[3] > 300 && distanceSensor.val[9] < 300) {
                if (distanceSensor.val[9] > 140) {
                    correction = -7;
                }

            } else if (distanceSensor.val[9] > 300 &&
                       distanceSensor.val[3] < 200) {
                if (distanceSensor.val[3] > 140) {
                    correction = 7;
                }
            } else {
                correction = 0;
            }

            if ((distanceSensor.val[0] > 200) &&
                (distanceSensor.val[0] < 250)) {
                servo.velocity = SPEED;

                correction = 0;
            }
            app.delay(20);
        }
    }
}
// 2900で1マス

void rightGrid(App) {
    app.delay(100);
    while (1) {
        if ((distanceSensor.val[0] < 200) && (distanceSensor.val[3] < 250)) {
            app.delay(1000);
            servo.velocity = 0;
            count = 1;
            servo.stop();
            servo.angle -= 90;
            app.delay(1800);
        } else if (distanceSensor.val[3] > 300) {
            servo.velocity = 0;
            count = 1;
            servo.stop();
            servo.angle += 90;
            app.delay(1000);
            servo.velocity = SPEED;
            app.delay(2604);
            servo.velocity = 0;
            app.delay(2000);
        } else {
            app.delay(1000);
            servo.velocity = SPEED;
            app.start(adjustment);
            app.delay(2604);
            servo.velocity = 0;
            app.delay(2000);
        }

        if (location.x == 0 && location.y == 0 && distanceSensor.val[0] < 150) {
            if (millis() > 30000) {
                servo.velocity = 0;
                app.stop(servoApp);
                app.stop(adjustment);
                servo.stop();
                app.stop(camera);
                app.stop(visualization);
                led.setTopColor(led.white);
                led.setLeftColor(led.white);
                led.setRightColor(led.white);
                led.setUIColor(led.white);
                led.show();
            } else {
                app.delay(1);
            }
        }
    }
}

void rightWall(App) {
    app.delay(500);
    while (1) {
        servo.velocity = SPEED;
        app.delay(10);
        // app.delay(2700);
        // servo.velocity = 0;
        // app.delay(1000);

        if (location.x == 0 && location.y == 0 && distanceSensor.val[0] < 180) {
            if (millis() > 30000) {
                servo.velocity = 0;
                app.stop(servoApp);
                app.stop(adjustment);
                servo.stop();
                app.stop(camera);
                app.stop(visualization);
                led.setTopColor(led.white);
                led.setLeftColor(led.white);
                led.setRightColor(led.white);
                led.setUIColor(led.white);
                led.show();
                app.delay(1000);

                // speaker.matsukenShogun();
            } else {
                app.delay(10);
            }
        }

        while (count == 1) {
            app.delay(900);
            count = 0;
        }

        while (count == 0) {
            val6 = distanceSensor.val[6];
            val0 = distanceSensor.val[0];
            count = 2;
            app.delay(10);
        }

        if (distanceSensor.val[3] > 200 && gyro.slope == 0) {
            count = 2;
            if (val6 > 600) {
                if ((val0 - 130) > distanceSensor.val[0]) {
                    servo.velocity = 0;
                    servo.stop();
                    app.delay(500);
                    servo.angle += 90;
                    count = 0;
                    app.delay(1500);
                    servo.velocity = SPEED;
                    app.delay(1700);
                }
            } else if ((val6 + 130) < distanceSensor.val[6]) {
                servo.velocity = 0;
                servo.stop();
                app.delay(500);
                servo.angle += 90;
                count = 0;
                app.delay(1500);
                servo.velocity = SPEED;
                app.delay(1700);
            }

        } else {
            count = 0;
            app.delay(10);
        }

        if (distanceSensor.val[0] < 110 && gyro.slope == 0) {
            servo.velocity = 0;
            servo.stop();
            app.delay(500);
            servo.angle -= 90;
            count = 1;
            app.delay(1500);
        }
    }
}

void locationMapping(App) {
    app.delay(500);
    while (1) {
        static int condition1 = 0;
        static int condition2 = 0;
        static int condition3 = 0;
        static int condition4 = 0;
        static int condition5 = 0;
        static int sum = 0;

        sum = condition1 + condition2 + condition3 + condition4 + condition5;
        app.delay(50);

        if (distanceSensor.val[3] > 300) {
            condition1 = 1;
            app.delay(100);
        } else {
            condition1 = 0;
        }
        if (location
                .mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN + 1]
                .isPassed) {
            condition2 = 1;
            app.delay(100);
        } else {
            condition2 = 0;
            app.delay(100);
        }
        if (location
                .mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN - 1]
                .isPassed) {
            condition3 = 1;
            app.delay(100);
        } else {
            condition3 = 0;
            app.delay(100);
        }
        if (location
                .mapData[location.x + MAP_ORIGIN + 1][location.y + MAP_ORIGIN]
                .isPassed) {
            condition4 = 1;
            app.delay(100);
        } else {
            condition4 = 0;
            app.delay(100);
        }
        if (location
                .mapData[location.x + MAP_ORIGIN - 1][location.y + MAP_ORIGIN]
                .isPassed) {
            condition5 = 1;
            app.delay(100);
        } else {
            condition5 = 0;
            app.delay(100);
        }

        sum = condition1 + condition2 + condition3 + condition4 + condition5;
        app.delay(50);

        if (sum == 3) {
            count = 1;
            servo.velocity = SPEED;
            app.delay(3000);
            servo.velocity = 0;
            servo.stop();
            app.delay(500);
            servo.angle -= 90;
            app.delay(1000);
            servo.velocity = SPEED;
            app.delay(3000);
        }
    }
}

void hitAvoid(App) {
    static bool oldStatus = false;
    while (1) {
        if (loadcell.status == RIGHT) {
            app.stop(servoApp);
            app.stop(rightWall);
            app.stop(adjustment);
            servo.driveAngularVelocity(-30, 45);
            app.delay(500);
            servo.driveAngularVelocity(0, -45);
            app.delay(600);

            oldStatus = false;
        }
        if (loadcell.status == LEFT) {
            app.stop(servoApp);
            app.stop(rightWall);
            app.stop(adjustment);
            servo.driveAngularVelocity(-30, -45);
            app.delay(500);
            servo.driveAngularVelocity(0, 45);
            app.delay(600);

            oldStatus = false;
        }
        if (!oldStatus) {
            app.start(servoApp);
            app.start(rightWall);
            app.start(adjustment);

            oldStatus = true;
        }

        app.delay(20);
    }
    app.delay(10);
}

void leftWall(App) {
    app.delay(500);
    while (1) {
        servo.velocity = SPEED;
        app.delay(10);

        while (count == 1) {
            app.delay(800);
            count = 2;
        }

        while (count == 0) {
            val6  = distanceSensor.val[6];
            count = 2;
            app.delay(10);
        }

        if (distanceSensor.val[9] > 200) {
            count = 2;
            if (val6 > 600) {
                val6 = distanceSensor.val[0];
                if ((val6 - 140) > distanceSensor.val[0]) {
                    servo.velocity = 0;
                    servo.angle -= 90;
                    app.delay(1000);
                    servo.velocity = 30;
                    app.delay(1000);
                }
            } else if ((val6 + 140) < distanceSensor.val[6]) {
                servo.velocity = 0;
                servo.angle -= 90;
                app.delay(1000);
                servo.velocity = 30;
                app.delay(3500);
            }

        } else if ((distanceSensor.val[0] < 120) &&
                   (distanceSensor.val[9] < 230)) {
            app.stop(adjustment);
            servo.velocity = 0;
            app.delay(500);
            servo.angle += 90;
            count = 1;
            app.delay(500);
            app.start(adjustment);
        } else {
            count = 0;
            app.delay(10);
        }
    }
}

void monitor(App) {
    while (1) {
        uart1.print(heatSensor.val[0]);
        uart1.print(" ");
        uart1.println(heatSensor.val[1]);

        uart3.print(location.x);
        uart3.print(" ");
        uart3.print(location.y);
        uart3.print(" ");
        uart3.print((int)location.coordinateX);
        uart3.print(" ");
        uart3.print((int)location.coordinateY);
        uart3.print(" ");
        uart3.print((int)servo.rightWheelSpeed);
        uart3.println("");

        // for (int i = 13; i < 27; i++) {      // たて
        //     for (int j = 13; j < 27; j++) {  // 横
        //         if (location.mapData[j][39 - i].isPassed == true) {
        //             uart1.print("■");
        //         } else if (location.mapData[j][39 - i].isDetected == true) {
        //             uart1.print("□");
        //         } else {
        //             uart1.print(" ");
        //         }
        //     }

        //     uart1.println("");
        // // }
        // uart3.print(floorSensor.redVal);
        // uart3.print(" ");
        // uart3.print(floorSensor.greenVal);
        // uart3.print(" ");
        // uart3.print(floorSensor.blueVal);
        // uart3.println(" ");

        app.delay(100);

        // uart3.print(distanceSensor.val[0]);
        // uart3.print(" ");
        // uart3.print(distanceSensor.val[1]);
        // uart3.print(" ");
        // uart3.println(distanceSensor.val[2]);
        // app.delay(30);
    }
}

void black(App) {
    while (1) {
        if (floorSensor.blueVal > 500) {
            count = 1;
            app.stop(servoApp);
            servo.driveAngularVelocity(-SPEED, 0);
            led.setTopColor(led.red);
            led.show();
            app.delay(1500);
            app.start(servoApp);
            servo.velocity = 0;
            servo.stop();
            app.delay(500);
            servo.angle -= 180;
            count = 0;
            app.delay(900);
            servo.velocity = SPEED;
            app.delay(1000);

        } else {
            app.delay(50);
        }
    }
}

void visualization(App) {
    while (1) {
        for (int i = 0; i < 24; i += 2) {
            stripTop.setPixelColor(i, (30000 / distanceSensor.val[i / 2]),
                                   (30000 / distanceSensor.val[i / 2]),
                                   (30000 / distanceSensor.val[i / 2]));
        }

        for (int j = 1; j <= 12; j++) {
            stripTop.setPixelColor(
                2 * j - 1,
                30000 /
                    ((distanceSensor.val[j - 1] + distanceSensor.val[j]) / 2),
                30000 /
                    ((distanceSensor.val[j - 1] + distanceSensor.val[j]) / 2),
                30000 /
                    ((distanceSensor.val[j - 1] + distanceSensor.val[j]) / 2));
        }

        led.show();
        app.delay(10);
    }
}

void camera(App) {
    bool oldstatus = true;
    while (1) {
        // int rescueKitNum = 0;
        // int leftOrRight = 1;
        // if (!location.mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]
        //          .isVictimDetected) {
        //     while (cameraRight.data == 'H') {
        //         led.setUIColor(led.pink);
        //         servo.velocity = 0;
        //         app.stop(servoApp);
        //         servo.stop();
        //         app.stop(adjustment);
        //         app.stop(rightWall);
        //         led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
        //         led.show();
        //         oldstatus = false;

        //         app.delay(5);

        //         rescueKitNum = 3;
        //         leftOrRight = RIGHT;

        //         location
        //             .mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]
        //             .isVictimDetected = true;
        //     }

        //     while (cameraRight.data == 'S') {
        //         led.setUIColor(led.blue);
        //         servo.velocity = 0;
        //         servo.stop();
        //         app.stop(servoApp);
        //         app.stop(adjustment);
        //         app.stop(rightWall);
        //         led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
        //         led.show();
        //         oldstatus = false;

        //         app.delay(5);

        //         rescueKitNum = 2;
        //         leftOrRight = RIGHT;

        //         location
        //             .mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]
        //             .isVictimDetected = true;
        //     }

        //     while (cameraRight.data == 'U') {
        //         led.setUIColor(led.white);
        //         servo.velocity = 0;
        //         servo.stop();
        //         app.stop(servoApp);
        //         app.stop(adjustment);
        //         app.stop(rightWall);
        //         led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
        //         led.show();
        //         oldstatus = false;

        //         app.delay(5);

        //         location
        //             .mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]
        //             .isVictimDetected = true;
        //     }

        //     while (cameraRight.data == 'R') {
        //         led.setUIColor(led.red);
        //         servo.velocity = 0;
        //         servo.stop();
        //         app.stop(servoApp);
        //         app.stop(adjustment);
        //         app.stop(rightWall);
        //         led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
        //         led.show();
        //         oldstatus = false;

        //         rescueKitNum = 3;
        //         leftOrRight = RIGHT;

        //         app.delay(5);

        //         location
        //             .mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]
        //             .isVictimDetected = true;
        //     }

        //     while (cameraRight.data == 'G') {
        //         led.setUIColor(led.green);
        //         servo.velocity = 0;
        //         servo.stop();
        //         app.stop(servoApp);
        //         app.stop(adjustment);
        //         app.stop(rightWall);
        //         led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
        //         led.show();
        //         oldstatus = false;

        //         app.delay(5);
        //     }

        //     while (cameraRight.data == 'Y') {
        //         led.setUIColor(led.yellow);
        //         servo.velocity = 0;
        //         servo.stop();
        //         app.stop(servoApp);
        //         app.stop(adjustment);
        //         app.stop(rightWall);
        //         led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
        //         led.show();
        //         oldstatus = false;

        //         rescueKitNum = 2;
        //         leftOrRight = RIGHT;

        //         app.delay(5);

        //         location
        //             .mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]
        //             .isVictimDetected = true;
        //     }
        // }

        // if (!oldstatus) {
        //     if (rescueKitNum != 0) {
        //         servo.rescueKit(rescueKitNum, leftOrRight);
        //     }

        //     app.start(servoApp);
        //     app.start(rightWall);
        //     app.start(adjustment);
        //     app.delay(5);

        //     oldstatus = true;
        // }

        // led.setUIColor(led.blank);
        // led.show();
        app.delay(100);
    }
}

void victimApp(App) {
    while (1) {
        bool status = false;
        int rescueKitNum = 0;
        int leftOrRight = 0;
        if (!location.mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]
                 .isVictimDetected) {
            if (heatSensor.r) {
                unsigned long timer = millis();
                while (millis() - timer < 6000) {
                    led.setUIColor(led.cyan);
                    servo.velocity = 0;
                    app.stop(servoApp);
                    servo.stop();
                    app.stop(adjustment);
                    app.stop(rightWall);
                    led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
                    led.show();
                    status = true;

                    app.delay(5);

                    rescueKitNum = 1;
                    leftOrRight = RIGHT;
                }

                location
                    .mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]
                    .isVictimDetected = true;
            }

            if (heatSensor.l) {
                unsigned long timer = millis();
                while (millis() - timer < 6000) {
                    led.setUIColor(led.cyan);
                    servo.velocity = 0;
                    app.stop(servoApp);
                    servo.stop();
                    app.stop(adjustment);
                    app.stop(rightWall);
                    led.setUIBrightness(127 * sin(millis() / 200.0) + 127);
                    led.show();
                    status = true;

                    app.delay(5);

                    rescueKitNum = 1;
                    leftOrRight = LEFT;
                }

                location
                    .mapData[location.x + MAP_ORIGIN][location.y + MAP_ORIGIN]
                    .isVictimDetected = true;
            }
        }

        if (status) {
            if (rescueKitNum != 0) {
                servo.rescueKit(rescueKitNum, leftOrRight);
            }

            app.start(servoApp);
            app.start(rightWall);
            app.start(adjustment);
            app.delay(5);

            status = false;
        }

        led.setUIColor(led.blank);
        led.show();

        app.delay(10);
    }
}

void lever(App) {
    static bool oldStatus = false;
    while (1) {
        if (ui.toggle == false) {
            // speaker.bootSound();
            led.setTopColor(led.green);
            led.show();
            servo.velocity = 0;
            app.stop(servoApp);
            app.stop(rightWall);
            app.stop(adjustment);
            app.stop(visualization);
            app.stop(black);
            app.stop(camera);

            app.stop(locationApp);
            app.stop(mapApp);
            app.stop(victimApp);
            app.stop(sideLEDApp);
            // app.stop(locationMapping);
            servo.stop();
            oldStatus = false;
        } else {
            if (!oldStatus) {
                speaker.bootSound();
                app.start(servoApp);
                app.start(rightWall);
                app.start(adjustment);
                app.start(visualization);
                app.start(black);
                app.start(camera);

                app.start(victimApp);

                app.start(locationApp);
                app.start(mapApp);
                app.start(sideLEDApp);
                // app.start(locationMapping);
                gyro.setOffset();

                oldStatus = true;
            }
        }
        app.delay(10);
    }
}
#endif