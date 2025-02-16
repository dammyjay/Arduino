#include "Arduino.h"
#include "L298NDriver.h"


L298NDriver::L298NDriver(){}

void L298NDriver::init(int M1APIN, int M1BPIN, int M2APIN, int M2BPIN, int ENA, int ENB){
    // Left motor pin
    _M1APIN = M1APIN;
    _M1BPIN = M1BPIN;
    // Right motor pin
    _M2APIN = M2APIN;
    _M2BPIN = M2BPIN;

    _ENA = ENA;
    _ENB = ENB;

    pinMode(_M1APIN, OUTPUT);
    pinMode(_M1BPIN, OUTPUT);
    pinMode(_M2APIN, OUTPUT);
    pinMode(_M2BPIN, OUTPUT);

    digitalWrite(_M1APIN, HIGH);
    digitalWrite(_M1BPIN, HIGH);
    digitalWrite(_M2APIN, HIGH);
    digitalWrite(_M2BPIN, HIGH);

    analogWrite(_ENA, 255);
    analogWrite(_ENB, 1);

}

void L298NDriver::stop() {
    digitalWrite(_M1APIN, LOW);
    digitalWrite(_M1BPIN, LOW);
    digitalWrite(_M2APIN, LOW);
    digitalWrite(_M2BPIN, LOW);

    analogWrite(_ENA, 0);
    analogWrite(_ENB, 0);
}

void L298NDriver::move(int speed, bool forward){
    if (forward){
        digitalWrite(_M1APIN, HIGH);
        digitalWrite(_M1BPIN, LOW);
        digitalWrite(_M2APIN, LOW);
        digitalWrite(_M2BPIN, HIGH);
    } else {
        digitalWrite(_M1APIN, LOW);
        digitalWrite(_M1BPIN, HIGH);
        digitalWrite(_M2APIN, HIGH);
        digitalWrite(_M2BPIN, LOW);
    }
    analogWrite(_ENA, speed);
    analogWrite(_ENB, speed);
}

void L298NDriver::turn(int angleVel, bool forward, bool turnLeft){
    if (forward){
        digitalWrite(_M1APIN, HIGH);
        digitalWrite(_M1BPIN, LOW);
        digitalWrite(_M2APIN, LOW);
        digitalWrite(_M2BPIN, HIGH);
    } else {
        digitalWrite(_M1APIN, LOW);
        digitalWrite(_M1BPIN, HIGH);
        digitalWrite(_M2APIN, HIGH);
        digitalWrite(_M2BPIN, LOW);
    }

    if (turnLeft) {
        analogWrite(_ENA, 0.5 * angleVel);
        analogWrite(_ENB, angleVel);
    } else {
        analogWrite(_ENA, angleVel);
        analogWrite(_ENB, 0.5 * angleVel);
    }
}