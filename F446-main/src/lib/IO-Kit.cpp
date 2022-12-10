#include "IO-Kit.h"

Output::Output(int pin) {
    this->_pinNumber = pin;
    pinMode(this->_pinNumber, OUTPUT);
}

void Output::set(bool val) {
#ifdef _STM32_DEF_
    if (!this->_pwmExp) {
        digitalWrite(this->_pinNumber, val);
    } else {
        analogWrite(this->_pinNumber, val * 255);
    }
#else
    digitalWrite(this->_pinNumber, val);
#endif

    this->_pinStatus = val * 255;
}

void Output::setPWM(int val) {
    analogWrite(this->_pinNumber, val);
    this->_pinStatus = val;

#ifdef _STM32_DEF_
    if (!this->_pwmExp) {
        this->_pwmExp = true;
    }
#endif
}

float Output::operator=(float val) {
    val = constrain(val, 0.0, 1.0);
    if (val == 1.0 || val == 0.0) {
        this->set(bool(val));
    } else {
        this->setPWM(round(val * 255));
    }
    return val;
}

int Output::operator<<=(int val) {
    this->setPWM(val);
    return val;
}

Output::operator float() {
    return _pinStatus / 255.0;
}

byte Output::raw(void) {
    return this->_pinStatus;
}

Input::Input(int pin) {
    this->_pinNumber = pin;
    pinMode(this->_pinNumber, INPUT);
}

Input::Input(int pin, bool pullup) {
    this->_pinNumber = pin;
    if (pullup) {
        pinMode(this->_pinNumber, INPUT_PULLUP);
    } else {
        pinMode(this->_pinNumber, INPUT);
    }
}

bool Input::digital(void) {
    return digitalRead(this->_pinNumber);
}

float Input::analog(void) {
    return float(analogRead(this->_pinNumber)) / 1023.0;
}

int Input::raw(void) {
    return analogRead(this->_pinNumber);
}

Input::operator bool() {
    return digital();
}